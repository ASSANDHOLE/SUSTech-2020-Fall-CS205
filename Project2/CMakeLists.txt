cmake_minimum_required(VERSION 3.17)
project(Project2)

set(CMAKE_CXX_STANDARD 14)

set(LOCAL_INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR}/include)

#Change the path to your--
set(OpenCV_DIR "path/to/opencv")

aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/src SRC_FLIES)

find_package(OpenCV REQUIRED)

include_directories(
        ${LOCAL_INCLUDE_DIR}
        ${OpenCV_INCLUDE_DIRS}
)

add_executable(${PROJECT_NAME} main.cpp ${SRC_FLIES})

target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})