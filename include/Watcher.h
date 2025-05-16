#pragma once

#include "WatcherItem.h"
#include "WatcherRegistry.h"

#include <array>
#include <cstdint>
#include <future>
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

    // Event types to handle.
    static constexpr uint32_t kWatchedEvents = IN_MODIFY | IN_CREATE;

public:
    /**
     * @brief Creates new instance of Watcher using given registry.
     * @param registry Registry with watcher entries.
     */
    explicit Watcher(WatcherRegistry &registry);

    /**
     * @brief Free all used resources.
     */
    ~Watcher();

    /**
     * Starts all given watchers.
     */
    auto Start() -> void;

private:
    /**
     * @brief Adds new inotify watcher.
     * @param entry Information for watcher.
     */
    auto CreateInotifyWatcher(const WatchedItem &entry) -> void;

    /**
     * @brief Handle 'inotify' system event.
     * @param event Event.
     */
    auto HandleEvent(inotify_event *event) const -> void;

    /**
     * @brief Starts a loop to read and handle 'inotify' events.
     */
    auto ReceiveEvents() const -> void;

    WatcherRegistry &registry_;
    WatcherDescriptor inotify_descriptor_;
    std::unordered_map<WatcherDescriptor, const WatchedItem &> watched_items_;
};
