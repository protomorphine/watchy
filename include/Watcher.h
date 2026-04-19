#pragma once

#include "WatchedItem.h"

#include <array>
#include <cstdint>
#include <functional>
#include <future>
#include <optional>
#include <stop_token>
#include <sys/inotify.h>
#include <thread>
#include <unordered_map>
#include <variant>
#include <vector>

/**
 * @class Watcher
 * @brief Provides a watcher subsystem.
 */
class Watcher {
  using WatcherDescriptor = int;
  using StopCallback = std::stop_callback<std::function<void()>>;

  // Event types to handle.
  static constexpr uint32_t kWatchedEvents = IN_MODIFY | IN_CREATE;
  static constexpr int kPollTimeoutMs = 100;

public:
  explicit Watcher(std::vector<WatchedItem> watched_items);

  ~Watcher();

  auto Start(const std::stop_source &stop_src) -> void;

private:
  auto HandleEvent(inotify_event *ev) const -> void;

  auto ReceiveEvents(const std::stop_token &stop_tkn) const -> void;

  WatcherDescriptor inotifyDescriptor_;

  std::jthread eventThread_;
  std::optional<StopCallback> eventThreadStopCallback_;

  std::unordered_map<WatcherDescriptor, WatchedItem> watchedItems_;
};
