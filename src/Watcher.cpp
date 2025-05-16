#include "Watcher.h"
#include "WatcherItem.h"
#include "WatcherRegistry.h"

#include "sys/inotify.h"
#include "unistd.h"
#include <array>
#include <format>
#include <spdlog/spdlog.h>
#include <stdexcept>
#include <string>
#include <thread>

Watcher::Watcher(WatcherRegistry &registry) : registry_(registry) {
    inotify_descriptor_ = inotify_init();

    if (inotify_descriptor_ == -1) {
        throw std::runtime_error("Could not initialize `inotify` subsystem");
    }
};

Watcher::~Watcher() {
    for (auto &pair : watched_items_) {
        inotify_rm_watch(inotify_descriptor_, pair.first);
    }

    close(inotify_descriptor_);
};

auto Watcher::Start() -> void {
    for (const WatchedItem &entry : registry_.GetEntries()) {
        CreateInotifyWatcher(entry);
    }

    std::thread events_thread{[&] { ReceiveEvents(); }};
    events_thread.join();
}

auto Watcher::CreateInotifyWatcher(const WatchedItem &entry) -> void {
    spdlog::info("Creating new watcher for '{}'", entry.GetPath().string());
    WatcherDescriptor watcher_descriptor =
        inotify_add_watch(inotify_descriptor_, entry.GetPath().c_str(), kWatchedEvents);

    if (watcher_descriptor == -1) {
        spdlog::error("Failed to create watcher for '{}'", entry.GetPath().string());
        return;
    }

    watched_items_.emplace(watcher_descriptor, entry);
}

auto Watcher::ReceiveEvents() const -> void {
    constexpr size_t kBufSize = 1024;
    std::array<char, kBufSize> buffer{};

    while (true) {
        size_t len = read(inotify_descriptor_, buffer.data(), kBufSize);

        if (len == -1) {
            spdlog::warn("Unable to read data from 'inotify' descriptor");
            continue;
        }

        auto *event = reinterpret_cast<inotify_event *>(buffer.data());
        HandleEvent(event);
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

    auto path = std::format("{0}/{1}", watched_items_.at(event->wd).GetPath().string(), std::string(event->name));

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
