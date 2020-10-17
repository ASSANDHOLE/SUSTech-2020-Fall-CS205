cmake_minimum_required(VERSION 3.17)
project(Assignment3)

set(CMAKE_CXX_STANDARD 17)
set(INC_DIR ./include)
set(LINK_DIR ./lib)

include_directories(${INC_DIR})
link_directories(${LINK_DIR})

add_executable(Assignment3 main.cpp)
target_link_libraries(Assignment3 libopenblas.dll)
