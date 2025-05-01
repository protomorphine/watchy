#include "WatcherRegistryBuilder.h"
#include <spdlog/spdlog.h>

auto WatcherRegistryBuilder::Add(const WatcherEntry& entry) -> void {
    spdlog::info(
        "Adding a new watcher entry (path: {}, command: {}) to registry", entry.FilePath.string(), entry.Command
    );
    entries_.push_back(entry);
}

auto WatcherRegistryBuilder::Build() -> WatcherRegistry { return WatcherRegistry{entries_}; }
