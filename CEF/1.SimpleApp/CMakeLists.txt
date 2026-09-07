cmake_minimum_required(VERSION 3.25)

# VCPKG
if(ZQF_VCPKG)
  if(NOT EXISTS "${CMAKE_SOURCE_DIR}/.vcpkg/vcpkg-fetch.cmake")
    file(DOWNLOAD https://raw.githubusercontent.com/Dir-A/Registry/5a4db340dd77316e86180ae22a690b4b99db54f7/cmake/vcpkg_fetch.cmake "${CMAKE_SOURCE_DIR}/.vcpkg/vcpkg-fetch.cmake")
  endif()

  set(ZQF_VCPKG_VERSION "2026-05-27")
  include("${CMAKE_SOURCE_DIR}/.vcpkg/vcpkg-fetch.cmake")
endif()

# Project Name
project("CEFSimpleApp_Main" VERSION 1.0 LANGUAGES C CXX)

# Modules
find_package(zqf_cmake_modules CONFIG REQUIRED)

# Config
if(ZQF_DEVELOP)
  include("zqf_profiles")
  include("zqf_optimizer")
  include("zqf_config")
  zqf_config_output_dir()
  zqf_optimizer_enable_lto()
  zqf_optimizer_enable_gc_sections()
  zqf_profiles_fetch(CLANGD EDITOR_CONFIG)
endif()

# Source Code
add_subdirectory("src")
