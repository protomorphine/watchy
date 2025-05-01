#include "Watcher.h"
#include <spdlog/spdlog.h>
#include "WatcherEntry.h"
#include "WatcherRegistry.h"

#include <stdexcept>
#include <thread>

#include "sys/inotify.h"
#include "unistd.h"

Watcher::Watcher(WatcherRegistry& registry) : registry_(registry) {
    inotify_descriptor_ = inotify_init();

    if (inotify_descriptor_ == -1) {
        throw std::runtime_error("Could not initialize `inotify` subsystem");
    }
};

Watcher::~Watcher() {
    for (int watcher_descriptor : active_watcher_descriptors_) {
        inotify_rm_watch(inotify_descriptor_, watcher_descriptor);
    }

    close(inotify_descriptor_);
};

auto Watcher::Start() -> void {
    auto entires = registry_.GetEntries();

    for (WatcherEntry& entry : entires) {
        AddSystemWatcher(entry);
    }

    std::thread events_thread{[&] { ReceiveEvents(); }};
    events_thread.join();
}

auto Watcher::AddSystemWatcher(const WatcherEntry& entry) -> void {
    spdlog::info("Creating new watcher for '{}'", entry.FilePath.string());
    int watcher_descriptor = inotify_add_watch(inotify_descriptor_, entry.FilePath.c_str(), kWatchedEvents);

    if (watcher_descriptor == -1) {
        spdlog::error("Failed to create watcher for '{}'", entry.FilePath.string());
        return;
    }

    active_watcher_descriptors_.push_back(watcher_descriptor);
}

auto Watcher::ReceiveEvents() const -> void {
    constexpr size_t kBufSize = 1024;
    char buffer[kBufSize];

    while (true) {
        size_t len = read(inotify_descriptor_, buffer, kBufSize);

        if (len == -1) {
            spdlog::warn("Unable to read data from 'inotify' descriptor");
            continue;
        }

        auto* event = reinterpret_cast<inotify_event*>(buffer);
        HandleEvent(event);
    }
};

auto Watcher::HandleEvent(inotify_event* event) -> void {
    if (!event) {
        return;
    }

    switch (event->mask) {
        // file created
        case IN_CREATE:
            spdlog::info("File created (wd: {0}, name: '{1}')", event->wd, event->name);
            break;

        // file modified
        case IN_MODIFY:
            spdlog::info("File changed (wd: {0}, name: '{1}')", event->wd, event->name);
            break;

        default:
            spdlog::warn("Unsupported event (wd: {0}, name: '{1}')", event->wd, event->name);
            break;
    }
};
