#pragma once

#include <filesystem>
#include <string>

/**
 * @class WatcherEntry
 * @brief Represent a watcher entry.
 */
class WatcherEntry {
public:
  /**
   * @brief Creates new instance of WatcherEntry.
   *
   * @param path Path to watch.
   * @param command Command to execute on watcher's trigger.
   */
  WatcherEntry(const std::filesystem::path &path, const std::string &command)
      : FilePath(path), Command(command) { };

  const std::filesystem::path   FilePath;
  const std::string             Command;
};
