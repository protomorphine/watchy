#include "Watcher.h"
#include "WatcherItem.h"

#include "spdlog/spdlog.h"
#include <csignal>
#include <stop_token>
#include <type_traits>

namespace {
std::stop_source ssource;

template <typename... S>
requires(std::is_integral_v<S> && ...)
auto stop_on_signals(S... signals) -> void {
    for (auto const sig : {signals...}) {
        std::signal(sig, [](int s) -> void {
            spdlog::info("Received signal: {}. Exiting...", s);
            ssource.request_stop();
        });
    }
}
} // namespace

auto main() -> int {
    stop_on_signals(SIGINT, SIGQUIT, SIGKILL);

    spdlog::info("Starting watchy app");

    std::vector<WatchedItem> items {
        WatchedItem("/home/dzaytsev/work/watchy", "ls"),
        WatchedItem("/home/dzaytsev/work/tg-notes", "ls")
    };

    Watcher{items}.Start(ssource);
    return 0;
}
