cmake_minimum_required(VERSION 3.10)
project(hodis)

set(CXX_FLAGS -g -Wall)
string(REPLACE ";" " " CMAKE_CXX_FLAGS "${CXX_FLAGS}")
set(CMAKE_CXX_FLAGS_DEBUG "-O0")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNEBUG")
set(CMAKE_CXX_STANDARD 11)

find_package(Gtest)
find_package(Glog)
find_package(Gflags)

include_directories(${Boost_INCLUDE_DIRS})
include_directories(${PROJECT_SOURCE_DIR})

include_directories(src)

file(GLOB SOURCES
        src/*.hpp
        src/*.cpp
        src/*.h
        )
add_library(common_lib ${SOURCES})

add_subdirectory(test)
