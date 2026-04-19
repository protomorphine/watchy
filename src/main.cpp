#include "Watcher.h"
#include "WatchedItem.h"

#include "spdlog/spdlog.h"
#include "yaml-cpp/yaml.h"
#include <csignal>
#include <stop_token>
#include <string>
#include <vector>
#include <yaml-cpp/node/node.h>
#include <yaml-cpp/parser.h>

namespace {
std::stop_source app_stop_source;

template <std::integral... S>
auto RegisterSignalHandlers(S... signals) -> void {
  struct sigaction sa{};

  sa.sa_handler = [](int s) -> auto {
    spdlog::info("Received signal: {}. Exiting...", s);
    app_stop_source.request_stop();
  };

  sigemptyset(&sa.sa_mask);

  (sigaction(signals, &sa, nullptr), ...);
}

auto GetItems(std::string config_path) -> std::vector<WatchedItem> {
  YAML::Node config = YAML::LoadFile(config_path);
  YAML::Node entries = config["entries"];

  std::vector<WatchedItem> items;

  if (!entries.IsSequence()) {
    return items;
  }

  for (const auto &entry : config["entries"]) {
    items.push_back(entry.as<WatchedItem>());
  }

  return items;
}
} // namespace

auto main() -> int {
  RegisterSignalHandlers(SIGINT, SIGQUIT, SIGKILL);

  spdlog::info("Starting watchy app");

  Watcher(GetItems("config.yml")).Start(app_stop_source);
  return 0;
}
