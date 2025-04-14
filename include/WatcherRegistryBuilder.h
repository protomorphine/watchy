#pragma once

#include "WatcherEntry.h"
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
  void Add(const WatcherEntry &entry);

  /**
   * @brief Creates a registry.
   * @return New instance of WatcherRegistry.
   */
  const WatcherRegistry Build();

private:
  std::vector<WatcherEntry> m_entries;
};
