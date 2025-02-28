# MySayHelloDLL
cmake_minimum_required(VERSION 3.25)

# Project
project(MySayHelloDLL)

# Config
include(cmake/PersonalSettings.cmake)

# Executable
add_library(${PROJECT_NAME} MODULE "src/dllmain.cpp")
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_23)
target_compile_options(${PROJECT_NAME} PRIVATE $<$<OR:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>:/utf-8>)

# Warning
target_compile_options(${PROJECT_NAME} PRIVATE $<$<OR:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>:/W4>)
target_compile_options(${PROJECT_NAME} PRIVATE $<$<OR:$<C_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:GNU>>:-Wall -Wextra>)