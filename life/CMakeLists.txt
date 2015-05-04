cmake_minimum_required(VERSION 2.8.4)
project(life)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES life.cpp)
add_executable(life ${SOURCE_FILES})
