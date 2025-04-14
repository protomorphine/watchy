cmake_minimum_required(VERSION 4.0)
project(watchy)

find_package(spdlog REQUIRED)

set(SOURCES
    src/main.cpp
    src/WatcherRegistry.cpp
    src/WatcherRegistryBuilder.cpp)

add_executable(watchy ${SOURCES})

# Set the directories that should be included in the build command for this target
# when running g++ these will be included as -I/directory/path/
target_include_directories(watchy
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(watchy PRIVATE spdlog::spdlog $<$<BOOL:${MINGW}>:ws2_32>)
