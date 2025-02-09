# HelloQT
cmake_minimum_required(VERSION 3.25)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Project Name
project(HelloQT)

# Project Config
include(CMakeMycfg.cmake)

# Package
find_package(Qt6 REQUIRED COMPONENTS Widgets)

# Set Executable
add_executable("${PROJECT_NAME}" "src/main.cpp")
target_link_libraries("${PROJECT_NAME}" Qt6::Widgets)
target_compile_features("${PROJECT_NAME}" PRIVATE cxx_std_23)
target_compile_options("${PROJECT_NAME}" PRIVATE "$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
# set_target_properties("${PROJECT_NAME}" PROPERTIES WIN32_EXECUTABLE ON)

# Warning
if(MSVC)
    target_compile_options("${PROJECT_NAME}" PRIVATE /W4)
else()
    target_compile_options("${PROJECT_NAME}" PRIVATE -Wall -Wextra)
endif()
