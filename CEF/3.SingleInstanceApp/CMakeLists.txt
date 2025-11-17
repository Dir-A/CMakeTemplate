cmake_minimum_required(VERSION 3.25)

# VCPKG
if(ZQF_VCPKG)
  include("cmake/vcpkg/fetch.cmake")
endif()

# Project Name
project("CEFSingleInstance_Main" VERSION 1.0 LANGUAGES C CXX)

# Modules
find_package(zqf_cmake_modules CONFIG REQUIRED)

# Config
if(ZQF_DEVELOP)
  include("zqf_settings")
  include("zqf_profiles")
endif()

# Source Code
add_subdirectory("src")
