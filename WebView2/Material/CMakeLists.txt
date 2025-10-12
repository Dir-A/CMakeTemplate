cmake_minimum_required(VERSION 3.25)

# VCPKG
if(ZQF_VCPKG)
  include("cmake/ZQF_VCPKG.cmake")
  zqf_vcpkg_startup()
endif()

# Project Name
project("WebView2_Material" VERSION 1.0 LANGUAGES C CXX)

# Modules
find_package(zqf_cmake_modules CONFIG REQUIRED)
include("ZQF_Settings")

# Set Executable
add_executable(${PROJECT_NAME} "src/main.cpp")
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_23)
target_compile_options(${PROJECT_NAME} PRIVATE $<$<OR:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>:/utf-8>)

# Warning
target_compile_options(${PROJECT_NAME} PRIVATE $<$<OR:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>:/W4>)
target_compile_options(${PROJECT_NAME} PRIVATE $<$<OR:$<C_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:GNU>>:-Wall -Wextra>)

# External Packages
find_package(unofficial-webview2 CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE unofficial::webview2::webview2)
