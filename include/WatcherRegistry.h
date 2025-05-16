#pragma once

#include "WatcherItem.h"

#include <vector>

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
    explicit WatcherRegistry(std::vector<WatchedItem> &entries);

    /**
     * @brief Returns available entries.
     */
    [[nodiscard]] auto GetEntries() const -> const std::vector<WatchedItem> &;

private:
    std::vector<WatchedItem> &entries_;
};
