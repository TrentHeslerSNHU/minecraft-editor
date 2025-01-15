cmake_minimum_required(VERSION 3.9)
project(minecraft-editor)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(include local/include)
file(GLOB_RECURSE SOURCES src/*.cpp)
add_executable(minecraft-editor ${SOURCES})

find_package(Threads REQUIRED)
find_library (LEVELDB
    NAMES leveldb libleveldb # what to look for
    HINTS ${CMAKE_SOURCE_DIR}/local/lib # where to look
    NO_DEFAULT_PATH) # do not search system default paths

find_library (SNAPPY
    NAMES snappy libsnappy) # what to look for

target_link_libraries(main PRIVATE Threads::Threads ${LEVELDB} ${SNAPPY})

