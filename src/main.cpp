#include <iostream>

#include "WatcherRegistry.h"
#include "WatcherRegistryBuilder.h"
#include "spdlog/spdlog.h"

int main() {
  spdlog::info("starting watchy app");

  WatcherRegistryBuilder builder{};
  builder.Add(WatcherEntry("/home/drzaytsev/work/watchy/", "ls"));

  auto registry = builder.Build();

  auto registeredEntries = registry.GetEntries();

  for (int i = 0; i < registeredEntries.size(); ++i) {
    WatcherEntry &entry = registeredEntries[i];
    std::cout << entry.FilePath << " -> " << entry.Command << std::endl;
  }

  return 0;
}
