#include "WatcherRegistry.h"
#include "WatcherItem.h"

#include <format>
#include <stdexcept>
#include <vector>

WatcherRegistry::WatcherRegistry(std::vector<WatchedItem> &entries) : entries_(entries) {
    auto it = std::ranges::adjacent_find(entries_, [](const WatchedItem &a, const WatchedItem &b) {
        return a.GetPath() == b.GetPath();
    });

    if (it != entries_.end()) {
        throw std::runtime_error{std::format("element with path '{0}' occurs more than once", it->GetPath().string())};
    }
};

auto WatcherRegistry::GetEntries() const -> const std::vector<WatchedItem> & { return entries_; };
