cmake_minimum_required(VERSION 3.9)
project(minecraft-editor)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(LEVELDB_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(LEVELDB_BUILD_BENCHMARKS OFF CACHE BOOL "" FORCE)
add_subdirectory(leveldb EXCLUDE_FROM_ALL)
add_subdirectory(snappy EXCLUDE_FROM_ALL)

file(GLOB_RECURSE SOURCES src/*.cpp)
add_executable(minecraft-editor ${SOURCES})

find_package(Threads REQUIRED)

target_link_libraries(minecraft-editor PRIVATE Threads::Threads leveldb snappy)

