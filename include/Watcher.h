#pragma once

#include <sys/inotify.h>
#include <cstdint>
#include <future>
#include <thread>
#include <unordered_map>
#include <vector>
#include "WatcherEntry.h"
#include "WatcherRegistry.h"

// Event types to handle.
static constexpr uint32_t kWatchedEvents = IN_MODIFY | IN_CREATE;

/**
 * @class Watcher
 * @brief Provides a watcher subsystem.
 */
class Watcher {
public:
    /**
     * @brief Creates new instance of Watcher using given registry.
     * @param registry Registry with watcher entries.
     */
    explicit Watcher(WatcherRegistry& registry);

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
    auto AddSystemWatcher(const WatcherEntry& entry) -> void;

    /**
     * @brief Handle 'inotify' system event.
     * @param event Event.
     */
    static auto HandleEvent(inotify_event* event) -> void;

    /**
     * @brief Starts a loop to read and handle 'inotify' events.
     */
    auto ReceiveEvents() const -> void;

    int inotify_descriptor_;
    WatcherRegistry& registry_;
    std::vector<int> active_watcher_descriptors_;
};
