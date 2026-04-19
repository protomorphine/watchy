#include "Watcher.h"
#include "WatchedItem.h"

#include "sys/inotify.h"
#include "unistd.h"
#include <array>
#include <cstring>
#include <filesystem>
#include <poll.h>
#include <spdlog/spdlog.h>
#include <stdexcept>
#include <stop_token>
#include <string>
#include <sys/poll.h>
#include <sys/select.h>
#include <thread>

Watcher::Watcher(std::vector<WatchedItem> watched_items)
    : inotifyDescriptor_(inotify_init()) {
  if (inotifyDescriptor_ == -1) {
    throw std::runtime_error("Could not initialize `inotify` subsystem");
  }

  for (auto &&entry : std::move(watched_items)) {
    std::filesystem::path entry_path = entry.GetPath();

    spdlog::info("Creating new watcher for '{}'", entry_path.string());
    WatcherDescriptor watcher_descriptor = inotify_add_watch(
        inotifyDescriptor_, entry_path.c_str(), kWatchedEvents);

    if (watcher_descriptor == -1) {
      spdlog::error("Failed to create watcher for '{}'", entry_path.string());
      continue;
    }

    watchedItems_.emplace(watcher_descriptor, std::move(entry));
  }
};

Watcher::~Watcher() {
  for (auto &pair : watchedItems_) {
    inotify_rm_watch(inotifyDescriptor_, pair.first);
  }

  close(inotifyDescriptor_);
};

auto Watcher::Start(const std::stop_source &stop_src) -> void {
  eventThread_ = std::jthread(
      [this](const std::stop_token &st) -> void { ReceiveEvents(st); });

  eventThreadStopCallback_.emplace(
      stop_src.get_token(), [this]() -> void { eventThread_.request_stop(); });

  eventThread_.join();
}

auto Watcher::ReceiveEvents(const std::stop_token &stop_tkn) const -> void {
  constexpr size_t kBufSize = 1024;
  std::array<char, kBufSize> buffer{};

  pollfd pfd = {.fd = inotifyDescriptor_, .events = POLLIN, .revents = 0};

  while (!stop_tkn.stop_requested()) {
    int poll_result = poll(&pfd, 1, kPollTimeoutMs);

    if (poll_result <= 0) {
      continue;
    }

    if ((pfd.revents & POLLIN) != 0) {
      size_t len = read(inotifyDescriptor_, buffer.data(), kBufSize);

      if (len == 0) {
        spdlog::warn("error while reading inotify descriptor");
        continue;
      }

      auto *ev = reinterpret_cast<inotify_event *>(buffer.data());
      HandleEvent(ev);
    }
  }
};

auto Watcher::HandleEvent(inotify_event *ev) const -> void {
  if (ev == nullptr) {
    spdlog::warn("Cannot handle event because given pointer is nullptr");
    return;
  }

  if (!watchedItems_.contains(ev->wd)) {
    spdlog::warn("unknown watcher descriptor '{0}'", ev->wd);
    return;
  }

  std::string path = watchedItems_.at(ev->wd).GetPath() / std::string(ev->name);

  switch (ev->mask) {
  // file created
  case IN_CREATE:
    spdlog::info("File created (wd: {0}, name: '{1}')", ev->wd, path);
    break;

  // file modified
  case IN_MODIFY:
    spdlog::info("File changed (wd: {0}, name: '{1}')", ev->wd, path);
    break;

  default:
    spdlog::warn("Unsupported event (wd: {0}, name: '{1}')", ev->wd, path);
    break;
  }
};
