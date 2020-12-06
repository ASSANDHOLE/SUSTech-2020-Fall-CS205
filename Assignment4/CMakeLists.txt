cmake_minimum_required(VERSION 2.1)
project(Assignment4)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_definitions(-std=c++11)

add_executable(Assignment4 main.cpp src/Matrix.cpp src/Matrix.hpp)
