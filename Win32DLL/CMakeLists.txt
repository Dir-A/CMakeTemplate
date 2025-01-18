# MySayHelloDLL
cmake_minimum_required(VERSION 3.25)

# Project Name
project(MySayHelloDLL)

# Project Config
include(CMakeMycfg.cmake)

# Set Executable
add_library("${PROJECT_NAME}" MODULE "src/dllmain.cpp")
target_compile_features("${PROJECT_NAME}" PRIVATE cxx_std_23)
target_compile_options("${PROJECT_NAME}" PRIVATE "$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

# Warning
if(MSVC)
    target_compile_options("${PROJECT_NAME}" PRIVATE /W4)
else()
    target_compile_options("${PROJECT_NAME}" PRIVATE -Wall -Wextra)
endif()
