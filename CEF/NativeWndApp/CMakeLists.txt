# CEFNativeWndApp_Main
cmake_minimum_required(VERSION 3.25)

# Project Name
project("CEFNativeWndApp_Main" VERSION 1.0 LANGUAGES C CXX)

# Project Settings
include("cmake/ZQFSettings.cmake")
include("cmake/ZQFCEFSettings.cmake")

# Source Code
add_subdirectory("src")
