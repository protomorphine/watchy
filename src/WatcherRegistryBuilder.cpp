#include "WatcherRegistryBuilder.h"
#include <spdlog/spdlog.h>

void WatcherRegistryBuilder::Add(const WatcherEntry &entry) {
  spdlog::info("Adding a new watcher entry (path: {}, command: {}) to registry",
               entry.FilePath.string(), entry.Command);
  m_entries.push_back(entry);
}

const WatcherRegistry WatcherRegistryBuilder::Build() {
  return WatcherRegistry{m_entries};
}
