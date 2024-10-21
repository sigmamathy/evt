cmake_minimum_required(VERSION 3.29)
project(evt)

set(CMAKE_CXX_STANDARD 20)

add_executable(evt src/main.cpp
        src/timedate.hpp
        src/timedate.cpp)

install(TARGETS evt DESTINATION /usr/local/bin)