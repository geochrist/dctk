cmake_minimum_required (VERSION 3.9)
MESSAGE(STATUS "CMAKE_ROOT: " ${CMAKE_ROOT})

# Project name
project(delay_calc_toolkit)

include(CTest)

# Turn on the verbose
set(CMAKE_VERBOSE_MAKEFILE ON)

# CXX target properties
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(USE_WERROR ON)
#set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -O2")
message(STATUS "CMAKE_BUILD_TYPE: " ${CMAKE_BUILD_TYPE})
message(STATUS "CMAKE_CXX_COMPILER: " ${CMAKE_CXX_COMPILER})
message(STATUS "CMAKE_CXX_FLAGS: " ${CMAKE_CXX_FLAGS})
message(STATUS "CMAKE_EXE_LINKER_FLAGS: " ${CMAKE_EXE_LINKER_FLAGS})


# add the binary tree to the search path for include files
include_directories(${PROJECT_SOURCE_DIR})
include_directories(../yaml-cpp/include)
include_directories(src-liberty_parse-2.6)
include_directories(src)

# -----------------------------------------------------------------------------
# Example program 
# -----------------------------------------------------------------------------
message(STATUS "Building examples ...")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build)

message(STATUS "EXAMPLE_CXX_FLAGS: " ${EXAMPLE_CXX_FLAGS})
message(STATUS "EXAMPLE_EXE_LINKER_FLAGS: " ${EXAMPLE_EXE_LINKER_FLAGS})

# YAML reader
link_directories(../yaml-cpp/build)

# Liberty parser
link_directories(src-liberty_parse-2.6)

# Data model
add_library(dctk src/Circuit.cpp src/liberty_reader.cpp src/CellLib.cpp src/Cell.cpp src/CellPin.cpp src/CellArc.cpp src/circuit_reader.cpp)

# contestant's delay calculation code/library
include_directories(src-dut)
link_directories(src-dut)
add_library(dut src-dut/delay_calculator.cpp)

# benchmarking executable
add_executable(delay_calc_tool src/delay_calc_tool.cpp)
target_link_libraries(delay_calc_tool dctk yaml-cpp libparse dut)

# Random Net Generator
add_executable(gen_random_nets src/gen_random_nets.cpp)
target_link_libraries(gen_random_nets dctk libparse yaml-cpp)





