# MySayHello
cmake_minimum_required(VERSION 3.25)

# Project Name
project(MySayHello)

# Project Config
include(CMakeMycfg.cmake)

# Set Executable
add_executable("${PROJECT_NAME}" "src/main.cpp")
set_target_properties("${PROJECT_NAME}" PROPERTIES WIN32_EXECUTABLE ON)
target_compile_features("${PROJECT_NAME}" PRIVATE cxx_std_23)
target_compile_options("${PROJECT_NAME}" PRIVATE "$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

# Warning
if(MSVC)
    target_compile_options("${PROJECT_NAME}" PRIVATE /W4)
else()
    target_compile_options("${PROJECT_NAME}" PRIVATE -Wall -Wextra)
endif()
