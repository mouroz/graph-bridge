cmake_minimum_required(VERSION 3.10)
project(MyProject)

# Set C++ Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Set output directory for the executable
if(WIN32)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})
elseif(UNIX)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})
endif()


# Add include directories (so we don't need relative paths in #include)
include_directories(${CMAKE_SOURCE_DIR}/code/utils ${CMAKE_SOURCE_DIR}/code/graph)

# Set output directory for executables (outside of build/)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/code)

# Collect all source files
set(SOURCES
    code/main.cpp
    code/tarjan.cpp
    code/naive.cpp
    code/eulerian.cpp
    code/graph/graph.cpp
    code/graph/graph_reader.cpp
    code/utils/randomizer.cpp
)

# Create the executable
add_executable(MyProject ${SOURCES})

# Set build type to Release by default if not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Optimization flags
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -march=native -flto -ffast-math -funroll-loops")
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g")

# Set compiler flags for Linux and Windows
if(UNIX)
    target_compile_options(MyProject PRIVATE -Wall -Wextra -Wpedantic)
elseif(WIN32)
    target_compile_options(MyProject PRIVATE -Wall -Wextra)
endif()
