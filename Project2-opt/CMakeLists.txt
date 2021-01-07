cmake_minimum_required(VERSION 3.1)
project(Project2)

set(CMAKE_CXX_STANDARD 14)

set(LOCAL_INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR}/include)
#set(OpenCV_DIR "path/to/opencv") #add if not set in path variables

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")

aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/src SRC_FLIES)

IF(SPEED_RUN)
	add_definitions(-DSPEED_RUN)
ENDIF()

find_package(OpenMP)
IF(OPENMP_FOUND)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${OpenMP_SHARED_LINKER_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
    add_definitions(-DENABLE_OPENMP)
ELSE()
    message("Missed OpenMP!")
ENDIF()

find_package(OpenCV REQUIRED)
IF(OPENCV_FOUND)
    include_directories(
            ${LOCAL_INCLUDE_DIR}
            ${OpenCV_INCLUDE_DIRS}
    )
ELSE()
    message(FATAL_ERROR "Missed OpenCV! CMake will exit")
ENDIF()

add_executable(${PROJECT_NAME} main.cpp speed_tracker.h ${SRC_FLIES})

target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})