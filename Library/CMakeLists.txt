# MxHello Main
cmake_minimum_required(VERSION 3.25)

# Project
project(My_MxHello_Main)

# Config
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    include(cmake/ZQFSettings.cmake)
endif()

# Source
add_subdirectory(src)

# Tests
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    add_subdirectory(test)
endif()
