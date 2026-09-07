# Project
project(CEFSimpleApp VERSION 1.0 LANGUAGES C CXX)

# Executable
add_executable(${PROJECT_NAME}
  "src/main.cpp"
  "src/CEFPreset.cpp"
  $<$<PLATFORM_ID:Darwin>:src/CEFPreset.mm>)
target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src")

# Compiler
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_23)
target_link_options(${PROJECT_NAME} PRIVATE
  $<$<OR:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>:/Entry:mainCRTStartup> # msvc
  $<$<OR:$<C_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:Clang>>: # clang
  $<$<OR:$<C_COMPILER_FRONTEND_VARIANT:MSVC>,$<CXX_COMPILER_FRONTEND_VARIANT:MSVC>>:/Entry:mainCRTStartup> # msvc frontend
  >)
target_compile_options(${PROJECT_NAME} PRIVATE
  $<$<OR:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>:/utf-8 /W4> # msvc
  $<$<OR:$<C_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:GNU>>:-Wall -Wextra> # gcc
  $<$<OR:$<C_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:Clang>>: # clang
  $<$<OR:$<C_COMPILER_FRONTEND_VARIANT:GNU>,$<CXX_COMPILER_FRONTEND_VARIANT:GNU>>:-Wall -Wextra> # gnu frontend
  $<$<OR:$<C_COMPILER_FRONTEND_VARIANT:MSVC>,$<CXX_COMPILER_FRONTEND_VARIANT:MSVC>>:/utf-8 /W4> # msvc frontend
  >
)

# Exteranl Libraries
find_package(zqf_cef_prebuilt CONFIG REQUIRED)
zqf_cef_prebuilt_config(${PROJECT_NAME})
target_link_libraries(${PROJECT_NAME} PRIVATE ZQF::CEFPrebuilt::CEF)

# Properties
set_target_properties(${PROJECT_NAME} PROPERTIES WIN32_EXECUTABLE TRUE)
set_target_properties(${PROJECT_NAME} PROPERTIES BUILD_WITH_INSTALL_RPATH TRUE INSTALL_RPATH "$ORIGIN/CEFRuntime")
set(EXECUTABLE_NAME ${PROJECT_NAME})
set(PRODUCT_NAME ${PROJECT_NAME})
set_target_properties(${PROJECT_NAME} PROPERTIES MACOSX_BUNDLE TRUE MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/res/Info.plist.in)

# Runtime Directory
if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
  set(CEF_RUNTIME_OUTPUT_DIRECTORY "$<TARGET_BUNDLE_CONTENT_DIR:${PROJECT_NAME}>/Frameworks" PARENT_SCOPE)
else()
  if(CMAKE_RUNTIME_OUTPUT_DIRECTORY)
    set(CEF_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/CEFRuntime/" PARENT_SCOPE)
  else()
    set(CEF_RUNTIME_OUTPUT_DIRECTORY "CEFRuntime/" PARENT_SCOPE)
  endif()
endif()
