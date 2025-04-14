#pragma once

#include <vector>

#include "WatcherEntry.h"

/**
 * @class WatcherRegistry
 * @brief represent a registry to store watcher entries.
 */
class WatcherRegistry {
public:
  explicit WatcherRegistry(std::vector<WatcherEntry> &entries);

  const std::vector<WatcherEntry> &GetEntries() { return m_entries; }

private:
  std::vector<WatcherEntry> &m_entries;
};

