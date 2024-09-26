cmake_minimum_required(VERSION 3.28)
project(SchedulerLib)

set(CMAKE_CXX_STANDARD 17)

# Source files
file(GLOB SOURCES src/*.cpp src/data/*.cpp src/data/course_types/*.cpp src/schedule/*.cpp)

add_library(SchedulerLib STATIC ${SOURCES})

# Include directories
target_include_directories(SchedulerLib PUBLIC
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/include/data
        ${CMAKE_SOURCE_DIR}/include/data/course_types
        ${CMAKE_SOURCE_DIR}/include/schedule)
