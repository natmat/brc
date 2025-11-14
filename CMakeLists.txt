cmake_minimum_required(VERSION 3.10)

project(brc LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ------------------------------------------------------
# Default build type = Debug (unless user overrides it)
# ------------------------------------------------------
if(NOT CMAKE_BUILD_TYPE)
    message(STATUS "No build type selected, defaulting to Debug.")
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the build type." FORCE)
endif()

# ------------------------------------------------------
# Compiler flags for different build types
# ------------------------------------------------------
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

# ------------------------------------------------------
# Your executable
# ------------------------------------------------------
add_executable(brc
    brc.cpp
)
target_include_directories(brc PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

