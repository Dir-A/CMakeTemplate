# MxLib
cmake_minimum_required(VERSION 3.25)

# Project
project("My_MxHello" VERSION 1.0 LANGUAGES C CXX)

# Source
list(APPEND ${PROJECT_NAME}_SRC_FILE 
    "src/My/MxHello/SayHello.cpp")

# Library
add_library(${PROJECT_NAME} ${${PROJECT_NAME}_SRC_FILE})
add_library(My::MxHello ALIAS ${PROJECT_NAME})
set_target_properties(${PROJECT_NAME} PROPERTIES EXPORT_NAME MxHello)
target_include_directories(${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/src> $<INSTALL_INTERFACE:include>)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_23)
target_compile_options(${PROJECT_NAME} PRIVATE $<$<OR:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>:/utf-8>)

# Warning
target_compile_options(${PROJECT_NAME} PRIVATE $<$<OR:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>:/W4>)
target_compile_options(${PROJECT_NAME} PRIVATE $<$<OR:$<C_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:GNU>>:-Wall -Wextra>)

# Install
include(GNUInstallDirs)
install(
  TARGETS 
    ${PROJECT_NAME}
  EXPORT 
    ${PROJECT_NAME}-targets
  LIBRARY
    DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE
    DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME
    DESTINATION ${CMAKE_INSTALL_BINDIR}
)
install(
    DIRECTORY 
      ${CMAKE_CURRENT_LIST_DIR}/src/
    DESTINATION 
      ${CMAKE_INSTALL_INCLUDEDIR}
    COMPONENT 
      headers
    FILES_MATCHING 
      PATTERN "*.h"
      PATTERN "*.hpp"
)
install(
  EXPORT 
    ${PROJECT_NAME}-targets
  FILE 
    ${PROJECT_NAME}-config.cmake
  DESTINATION 
    lib/cmake/${PROJECT_NAME}
  NAMESPACE 
    My::
)