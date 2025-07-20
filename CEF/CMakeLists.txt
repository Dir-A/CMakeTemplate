# MyCEFApp_Main
cmake_minimum_required(VERSION 3.25)

# Project Name
project("MyCEFApp_Main" VERSION 1.0 LANGUAGES C CXX)

# Project Settings
include("cmake/ZQFSettings.cmake")
include("cmake/ZQFCEFSettings.cmake")

add_subdirectory("src")
