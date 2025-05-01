cmake_minimum_required(VERSION 4.0)
set(CMAKE_CXX_STANDARD 20)

project(watchy)

find_package(spdlog REQUIRED)

set(SOURCES
    src/main.cpp
    src/Watcher.cpp
    src/WatcherRegistry.cpp
    src/WatcherRegistryBuilder.cpp)

add_executable(watchy ${SOURCES})

target_include_directories(watchy
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(watchy PRIVATE spdlog::spdlog $<$<BOOL:${MINGW}>:ws2_32>)
