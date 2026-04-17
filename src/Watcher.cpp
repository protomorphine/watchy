#include "Watcher.h"
#include "WatcherItem.h"

#include "sys/inotify.h"
#include "unistd.h"
#include <array>
#include <cerrno>
#include <cstring>
#include <poll.h>
#include <spdlog/spdlog.h>
#include <stdexcept>
#include <stop_token>
#include <string>
#include <sys/poll.h>
#include <sys/select.h>
#include <thread>

Watcher::Watcher(std::vector<WatchedItem> witems) {
    inotify_descriptor_ = inotify_init();

    if (inotify_descriptor_ == -1) {
        throw std::runtime_error("Could not initialize `inotify` subsystem");
    }

    for (auto&& entry : std::move(witems)) {
        CreateInotifyWatcher(std::move(entry));
    }
};

Watcher::~Watcher() {
    for (auto &pair : watched_items_) {
        inotify_rm_watch(inotify_descriptor_, pair.first);
    }

    close(inotify_descriptor_);
};

auto Watcher::CreateInotifyWatcher(WatchedItem entry) -> void {
    spdlog::info("Creating new watcher for '{}'", entry.GetPath().string());
    WatcherDescriptor watcher_descriptor =
        inotify_add_watch(inotify_descriptor_, entry.GetPath().c_str(), kWatchedEvents);

    if (watcher_descriptor == -1) {
        spdlog::error("Failed to create watcher for '{}'", entry.GetPath().string());
        return;
    }

    watched_items_.emplace(watcher_descriptor, std::move(entry));
}

auto Watcher::Start(const std::stop_source &ssource) -> void {
    event_thread_ = std::jthread([this](const std::stop_token &st) -> void { ReceiveEvents(st); });

    event_thread_stop_callback_.emplace(ssource.get_token(), [this]() -> void { event_thread_.request_stop(); });

    event_thread_.join();
}

auto Watcher::ReceiveEvents(const std::stop_token &stop) const -> void {
    constexpr size_t kBufSize = 1024;
    std::array<char, kBufSize> buffer{};

    pollfd pfd = {.fd = inotify_descriptor_, .events = POLLIN, .revents = 0};

    while (!stop.stop_requested()) {
        int presult = poll(&pfd, 1, 100);

        if (presult == 0) {
            continue;
        }

        if (presult < 0) {
            spdlog::warn("poll error: {}", strerror(errno));
            continue;
        }

        if (pfd.revents & POLLIN) {
            size_t len = read(inotify_descriptor_, buffer.data(), kBufSize);

            if (len == 0) {
                spdlog::warn("error while reading inotify descriptor");
                continue;
            }

            auto *event = reinterpret_cast<inotify_event *>(buffer.data());
            HandleEvent(event);
        }
    }
};

auto Watcher::HandleEvent(inotify_event *event) const -> void {
    if (event == nullptr) {
        spdlog::warn("Cannot handle event because given pointer is nullptr");
        return;
    }

    if (!watched_items_.contains(event->wd)) {
        spdlog::warn("unknown watcher descriptor '{0}'", event->wd);
        return;
    }

    std::string path = watched_items_.at(event->wd).GetPath() / std::string(event->name);

    switch (event->mask) {
    // file created
    case IN_CREATE:
        spdlog::info("File created (wd: {0}, name: '{1}')", event->wd, path);
        break;

    // file modified
    case IN_MODIFY:
        spdlog::info("File changed (wd: {0}, name: '{1}')", event->wd, path);
        break;

    default:
        spdlog::warn("Unsupported event (wd: {0}, name: '{1}')", event->wd, path);
        break;
    }
};
