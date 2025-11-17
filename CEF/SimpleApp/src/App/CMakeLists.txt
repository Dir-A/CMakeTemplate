# Project
project(CEFSimpleApp VERSION 1.0 LANGUAGES C CXX)

# Executable
add_executable(${PROJECT_NAME}
  "src/main.cpp"
  "src/Preboot.cpp"
  $<$<PLATFORM_ID:Darwin>:src/Preboot.mm>)
target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src")

# Compiler
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_23)
target_link_options(${PROJECT_NAME} PRIVATE $<$<C_COMPILER_ID:MSVC>:/Entry:mainCRTStartup>)
target_compile_options(${PROJECT_NAME} PRIVATE $<$<OR:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>:/utf-8>)

# Warning
target_compile_options(${PROJECT_NAME} PRIVATE $<$<OR:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>:/W4>)
target_compile_options(${PROJECT_NAME} PRIVATE $<$<OR:$<C_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:GNU>>:-Wall -Wextra>)
target_compile_options(${PROJECT_NAME} PRIVATE $<$<OR:$<C_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:Clang>>:-Wall -Wextra>)

# Exteranl Libraries
find_package(zqf_cef_prebuilt CONFIG REQUIRED)
zqf_cef_config(${PROJECT_NAME})
target_link_libraries(${PROJECT_NAME} PRIVATE ZQF::CEF::CEF)

# Properties
set_target_properties(${PROJECT_NAME} PROPERTIES WIN32_EXECUTABLE TRUE)
set_target_properties(${PROJECT_NAME} PROPERTIES BUILD_WITH_INSTALL_RPATH TRUE INSTALL_RPATH "$ORIGIN/CEFRuntime")
set_target_properties(${PROJECT_NAME} PROPERTIES MACOSX_BUNDLE TRUE MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/res/Info.plist.in)

# Runtime Directory
if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
  if(CMAKE_RUNTIME_OUTPUT_DIRECTORY)
    set(CEF_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/CEFSimpleApp.app/Contents/MacOS/CEFRuntime/" PARENT_SCOPE)
  else()
    set(CEF_RUNTIME_OUTPUT_DIRECTORY "CEFSimpleApp.app/Contents/MacOS/CEFRuntime/" PARENT_SCOPE)
  endif()
else()
  if(CMAKE_RUNTIME_OUTPUT_DIRECTORY)
    set(CEF_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/CEFRuntime/" PARENT_SCOPE)
  else()
    set(CEF_RUNTIME_OUTPUT_DIRECTORY "CEFRuntime/" PARENT_SCOPE)
  endif()
endif()

