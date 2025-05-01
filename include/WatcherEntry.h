#pragma once

#include <filesystem>
#include <string>
#include <utility>

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
    WatcherEntry(std::filesystem::path path, std::string command)
        : FilePath(std::move(path)), Command(std::move(command)) {};

    /**
     * Path to directory.
     */
    const std::filesystem::path FilePath;

    /**
     * Command to execute on files modify.
     */
    const std::string Command;
};
