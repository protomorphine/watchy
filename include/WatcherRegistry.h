#pragma once

#include <vector>

#include "WatcherEntry.h"

/**
 * @class WatcherRegistry
 * @brief represent a registry to store watcher entries.
 */
class WatcherRegistry {
public:
    /**
     * @brief Creates new instance of WatcherRegistry.
     * @param entries Entries.
     */
    explicit WatcherRegistry(std::vector<WatcherEntry>& entries);

    /**
     * @brief Returns available entries.
     */
    auto GetEntries() -> const std::vector<WatcherEntry>&;

private:
    std::vector<WatcherEntry>& entries_;
};
