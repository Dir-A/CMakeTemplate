cmake_minimum_required(VERSION 3.25)

# VCPKG
file(DOWNLOAD "https://raw.githubusercontent.com/Dir-A/CMakeModules/32cfbbf79fb9741a19df95ad439662c05ee10974/cmake/ZQF_VCPKG.cmake" "${CMAKE_BINARY_DIR}/ZQF_VCPKG.cmake")
include("${CMAKE_BINARY_DIR}/ZQF_VCPKG.cmake")
zqf_vcpkg_startup()

# Project Name
project("CEFSimpleApp_Main" VERSION 1.0 LANGUAGES C CXX)

# Modules
find_package(zqf_cmake_modules CONFIG REQUIRED)
include("ZQF_Settings")

# Source Code
add_subdirectory("src")
