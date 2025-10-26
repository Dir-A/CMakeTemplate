cmake_minimum_required(VERSION 3.25)

# VCPKG
include("cmake/ZQF_VCPKG.cmake")
zqf_vcpkg_startup()

# Project Name
project("CEFSimpleApp_Main" VERSION 1.0 LANGUAGES C CXX)

# Modules
find_package(zqf_cmake_modules CONFIG REQUIRED)
include("ZQF_Settings")

# Source Code
add_subdirectory("src")
