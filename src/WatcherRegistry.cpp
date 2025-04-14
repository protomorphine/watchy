#include "WatcherRegistry.h"
#include <vector>

WatcherRegistry::WatcherRegistry(std::vector<WatcherEntry> &entries)
    : m_entries(entries) {};
