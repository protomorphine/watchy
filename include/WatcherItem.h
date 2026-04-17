#pragma once

#include <filesystem>
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>

/**
 * @class WatcherEntry
 * @brief Represent a watcher entry.
 */
class WatchedItem {
public:
    /**
     * @brief Creates new instance of WatcherEntry.
     *
     * @param path Path to watch.
     * @param command Command to execute on watcher's trigger.
     */
    WatchedItem(std::filesystem::path &&path, std::string &&command) : file_path_(path), command_(command) {
        if (!std::filesystem::is_directory(path)) {
            throw std::runtime_error{"Given path must be a valid path to directory"};
        }
    };

    WatchedItem(WatchedItem &&other) noexcept
        : file_path_(std::move(other.file_path_)), command_(std::move(other.command_)) {}

    WatchedItem(const WatchedItem &other) = default;

    /**
     * @brief Returns command to execute on watcher's trigger.
     *
     * @return String representation of command.
     */
    [[nodiscard]] auto GetCommand() const -> std::string_view { return command_; }

    /**
     * @brief Returns path to watching directory.
     *
     * @return Path to watching directory.
     */
    [[nodiscard]] auto GetPath() const -> std::filesystem::path { return file_path_; }

private:
    std::string command_;
    std::filesystem::path file_path_;
};
