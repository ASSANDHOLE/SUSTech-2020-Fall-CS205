cmake_minimum_required(VERSION 3.17)
project(Project1)

set(CMAKE_CXX_STANDARD 14)

add_executable(Project1 main.cpp matrix.cpp matrix.h mul_mat.h)
target_link_libraries(Project1)