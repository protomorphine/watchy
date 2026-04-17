#pragma once

#include "WatcherItem.h"

#include <array>
#include <cstdint>
#include <functional>
#include <future>
#include <optional>
#include <stop_token>
#include <sys/inotify.h>
#include <thread>
#include <unordered_map>
#include <variant>
#include <vector>

/**
 * @class Watcher
 * @brief Provides a watcher subsystem.
 */
class Watcher {
    using WatcherDescriptor = int;
    using StopCallback = std::stop_callback<std::function<void()>>;

    // Event types to handle.
    static constexpr uint32_t kWatchedEvents = IN_MODIFY | IN_CREATE;

public:
    explicit Watcher(std::vector<WatchedItem> witems);

    ~Watcher();

    auto Start(const std::stop_source &ssource) -> void;

private:
    auto CreateInotifyWatcher(WatchedItem entry) -> void;

    auto HandleEvent(inotify_event *event) const -> void;

    auto ReceiveEvents(const std::stop_token &stop) const -> void;

    WatcherDescriptor inotify_descriptor_;

    std::jthread event_thread_;
    std::optional<StopCallback> event_thread_stop_callback_;

    std::unordered_map<WatcherDescriptor, WatchedItem> watched_items_;
};
