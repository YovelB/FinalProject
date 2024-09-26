cmake_minimum_required(VERSION 3.28)
project(FinalProject)

set(CMAKE_CXX_STANDARD 17)

# source files
file (GLOB SOURCES "src/*.cpp" "src/users/*.cpp")

# create the executable
add_executable(FinalProject ${SOURCES})

# include library headers
target_include_directories(FinalProject PUBLIC libs/scheduler/include)

# link against the SchedulerLib static library
target_link_libraries(FinalProject PRIVATE ${CMAKE_SOURCE_DIR}/libs/SchedulerLib/libSchedulerLib.a)
