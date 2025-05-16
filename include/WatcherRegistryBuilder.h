#pragma once

#include "WatcherItem.h"
#include "WatcherRegistry.h"

/**
 * @class WatcherRegistryBuilder
 * @brief Builder for WatcherRegistry.
 */
class WatcherRegistryBuilder {
public:
    /**
     * @brief Adds a given entry to registry.
     * @param entry Wathcher entry.
     */
    auto Add(const WatchedItem &entry) -> void;

    /**
     * @brief Creates a registry.
     * @return New instance of WatcherRegistry.
     */
    [[nodiscard]] auto Build() -> WatcherRegistry;

private:
    std::vector<WatchedItem> entries_;
};
