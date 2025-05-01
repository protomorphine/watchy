#include "WatcherRegistry.h"
#include <vector>
#include "WatcherEntry.h"

WatcherRegistry::WatcherRegistry(std::vector<WatcherEntry>& entries) : entries_(entries) {};

auto WatcherRegistry::GetEntries() -> const std::vector<WatcherEntry>& { return entries_; };
