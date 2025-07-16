# MyCEF
cmake_minimum_required(VERSION 3.25)

# Project Name
project(MyCEF)

# Project Settings
include(cmake/ZQFSettings.cmake)
include(cmake/ZQFCEFSettings.cmake)

add_subdirectory("src/cef_subprocess")
add_subdirectory("src/cef_test")
