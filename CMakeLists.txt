# CMakeLists.txt for Disaster Relief Optimization System
# Minimum CMake version
cmake_minimum_required(VERSION 3.10)

# Project name and version
project(DisasterReliefOptimization VERSION 1.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Compiler flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
set(CMAKE_CXX_FLAGS_RELEASE "-O2")
set(CMAKE_CXX_FLAGS_DEBUG "-g -DDEBUG")

# Source files
set(SOURCES
    main.cpp
    graph.cpp
    algorithms.cpp
    solver.cpp
    benchmark.cpp
)

# Header files
set(HEADERS
    graph.h
    algorithms.h
    solver.h
    benchmark.h
)

# Executable
add_executable(disaster_relief ${SOURCES} ${HEADERS})

# Include directories (if needed)
target_include_directories(disaster_relief PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# Installation
install(TARGETS disaster_relief DESTINATION bin)

# Testing (optional)
enable_testing()

# Custom targets
add_custom_target(run-sample
    COMMAND disaster_relief sample
    DEPENDS disaster_relief
    COMMENT "Running sample problem"
)

add_custom_target(run-benchmark
    COMMAND disaster_relief benchmark
    DEPENDS disaster_relief
    COMMENT "Running benchmarks"
)

add_custom_target(run-all
    COMMAND disaster_relief all
    DEPENDS disaster_relief
    COMMENT "Running all tests"
)

# Print build information
message(STATUS "")
message(STATUS "Disaster Relief Optimization System")
message(STATUS "====================================")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ compiler: ${CMAKE_CXX_COMPILER}")
message(STATUS "C++ standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Install prefix: ${CMAKE_INSTALL_PREFIX}")
message(STATUS "")

