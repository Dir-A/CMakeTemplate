# MxLib
cmake_minimum_required(VERSION 3.25)

# Project
project("My_MxHello" VERSION 1.0 LANGUAGES C CXX)

# Source
list(APPEND ${PROJECT_NAME}_SRC_FILE 
    "src/My/MxHello/SayHello.cpp")

# Library
add_library(${PROJECT_NAME} ${${PROJECT_NAME}_SRC_FILE})
target_include_directories(${PROJECT_NAME} PUBLIC "src")
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_23)
target_compile_options(${PROJECT_NAME} PRIVATE $<$<OR:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>:/utf-8>)

# Warning
target_compile_options(${PROJECT_NAME} PRIVATE $<$<OR:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>:/W4>)
target_compile_options(${PROJECT_NAME} PRIVATE $<$<OR:$<C_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:GNU>>:-Wall -Wextra>)

# Alias
add_library(My::MxHello ALIAS ${PROJECT_NAME})

