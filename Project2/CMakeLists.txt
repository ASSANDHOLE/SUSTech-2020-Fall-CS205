cmake_minimum_required(VERSION 3.1)
project(Project2)

set(CMAKE_CXX_STANDARD 14)

set(LOCAL_INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR}/include)
#set(OpenCV_DIR "path/to/opencv")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mavx2 -O3")

aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/src SRC_FLIES)

find_package(OpenCV REQUIRED)
IF(OPENCV_FOUND)
    message("Found OpenCV!")
    include_directories(
            ${LOCAL_INCLUDE_DIR}
            ${OpenCV_INCLUDE_DIRS}
    )
ELSE()
    message(FATAL_ERROR "Missed OpenCV! CMake will exit")
ENDIF()

add_executable(${PROJECT_NAME} main.cpp timer_local.h ${SRC_FLIES})

target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})