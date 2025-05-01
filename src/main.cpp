#include "Watcher.h"
#include "WatcherRegistryBuilder.h"
#include "spdlog/spdlog.h"

auto main() -> int {
    spdlog::info("Starting watchy app");

    WatcherRegistryBuilder builder;
    builder.Add(WatcherEntry("/home/drzaytsev/work/watchy", "ls"));

    auto registry = builder.Build();

    Watcher watcher{registry};
    watcher.Start();

    return 0;
}
