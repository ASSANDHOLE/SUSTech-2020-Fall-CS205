cmake_minimum_required(VERSION 3.10)
project(Project2)

set(TARGET_NAME face_detection)

set(CMAKE_CXX_STANDARD 17)

aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/src SRC_FLIES)

add_library(${TARGET_NAME}_lib SHARED ${SRC_FLIES})

add_executable(${TARGET_NAME}_demo demo.cpp speed_tracker.h)

target_include_directories(${TARGET_NAME}_lib PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)

find_package(OpenMP)
IF(OpenMP_CXX_FOUND)
    # Enable or disable OpenMP SIMD and parallel for
    add_definitions(-DOPENMP_SIMD)
    add_definitions(-DOPENMP_PARALLEL_FOR)
    target_link_libraries(${TARGET_NAME}_lib PRIVATE OpenMP::OpenMP_CXX)
ELSE()
    message("Missed OpenMP!")
ENDIF()

# set(OpenCV_DIR "path/to/opencv") # add if not set in path variables
find_package(OpenCV REQUIRED COMPONENTS core highgui imgproc)
IF(OPENCV_FOUND)
    target_link_libraries(${TARGET_NAME}_lib PRIVATE opencv_core opencv_highgui opencv_imgproc)
ELSE()
    message(FATAL_ERROR "Missed OpenCV! CMake will exit")
ENDIF()

target_link_libraries(${TARGET_NAME}_demo PRIVATE ${TARGET_NAME}_lib)

# Add an alias lib ${TARGET_NAME}::lib
add_library(${TARGET_NAME}::lib ALIAS ${TARGET_NAME}_lib)
