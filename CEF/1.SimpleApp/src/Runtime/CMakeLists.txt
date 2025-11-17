# Project
project(CEFRuntime VERSION 1.0 LANGUAGES C CXX)

find_package(zqf_cef_prebuilt CONFIG REQUIRED)

if(CMAKE_SYSTEM_NAME MATCHES "Windows|Linux")
  # Executable
  add_executable(${PROJECT_NAME} "src/main.cpp" $<$<PLATFORM_ID:Windows>:res/compatibility.manifest>)

  # Compiler
  target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_23)
  target_compile_options(${PROJECT_NAME} PRIVATE $<$<OR:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>:/utf-8>)

  # Warning
  target_compile_options(${PROJECT_NAME} PRIVATE $<$<OR:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>:/W4>)
  target_compile_options(${PROJECT_NAME} PRIVATE $<$<OR:$<C_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:GNU>>:-Wall -Wextra>)
  target_compile_options(${PROJECT_NAME} PRIVATE $<$<OR:$<C_COMPILER_ID:CLANG>,$<CXX_COMPILER_ID:CLANG>>:-Wall -Wextra>)

  # External Libraries
  zqf_cef_config(${PROJECT_NAME})
  zqf_cef_copyfiles(${PROJECT_NAME})
  target_link_libraries(${PROJECT_NAME} PRIVATE ZQF::CEF::CLibrary)

  # Properties
  set_target_properties(${PROJECT_NAME} PROPERTIES WIN32_EXECUTABLE TRUE)
  set_target_properties(${PROJECT_NAME} PROPERTIES BUILD_WITH_INSTALL_RPATH TRUE INSTALL_RPATH "$ORIGIN")
  set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CEF_RUNTIME_OUTPUT_DIRECTORY}")

elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
  foreach(_suffix_list ${CEF_HELPER_APP_SUFFIXES})
    string(REPLACE ":" ";" _suffix_list ${_suffix_list})
    list(GET _suffix_list 0 _name_suffix)
    list(GET _suffix_list 1 _target_suffix)

    set(_helper_target "${PROJECT_NAME}_Helper${_target_suffix}")
    set(_helper_output_name "${PROJECT_NAME} Helper${_name_suffix}")

    add_executable(${_helper_target} "src/main.cpp")
    target_compile_features(${_helper_target} PRIVATE cxx_std_23)
    target_compile_options(${_helper_target} PRIVATE $<$<OR:$<C_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:GNU>>:-Wall -Wextra>)
    target_compile_options(${_helper_target} PRIVATE $<$<OR:$<C_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:AppleClang>>:-Wall -Wextra>)
    set_target_properties(${_helper_target} PROPERTIES MACOSX_BUNDLE ON OUTPUT_NAME ${_helper_output_name} RUNTIME_OUTPUT_DIRECTORY "${CEF_RUNTIME_OUTPUT_DIRECTORY}")

    zqf_cef_config(${_helper_target})
    target_link_libraries(${_helper_target} PRIVATE ZQF::CEF::CEF)

    if(${_helper_target} STREQUAL ${PROJECT_NAME}_Helper)
      zqf_cef_copyfiles(${_helper_target})
    endif()
  endforeach()
endif()
