cmake_minimum_required(VERSION 3.1)
project(Settings)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_library(settings SHARED settings.cpp settings.h)