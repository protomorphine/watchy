#include "WatcherRegistryBuilder.h"

#include <spdlog/spdlog.h>

auto WatcherRegistryBuilder::Add(const WatchedItem &entry) -> void {
    spdlog::info(
        "Adding a new watcher entry (path: {}, command: {}) to registry", entry.GetPath().string(), entry.GetCommand()
    );
    entries_.push_back(entry);
}

auto WatcherRegistryBuilder::Build() -> WatcherRegistry { return WatcherRegistry{entries_}; }
