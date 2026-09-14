# Project
project(ZQFCEFRuntime VERSION 1.0 LANGUAGES C CXX)

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
  zqf_cef_prebuilt_config(${PROJECT_NAME})
  zqf_cef_prebuilt_copyfiles(${PROJECT_NAME})
  target_link_libraries(${PROJECT_NAME} PRIVATE ZQF::CEFPrebuilt::CLibrary)

  # Properties
  if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    set_target_properties(${PROJECT_NAME} PROPERTIES WIN32_EXECUTABLE TRUE)
  elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set_target_properties(${PROJECT_NAME} PROPERTIES BUILD_WITH_INSTALL_RPATH TRUE INSTALL_RPATH "$ORIGIN/${PROJECT_NAME}")
  endif()

  get_target_property(target_out_dir ${PROJECT_NAME} RUNTIME_OUTPUT_DIRECTORY)
  set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${target_out_dir}/${PROJECT_NAME}/")

elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
  foreach(_suffix_list ${CEF_HELPER_APP_SUFFIXES})
    string(REPLACE ":" ";" _suffix_list ${_suffix_list})
    list(GET _suffix_list 0 _name_suffix)
    list(GET _suffix_list 1 _target_suffix)

    set(_helper_target "${PROJECT_NAME}_Helper${_target_suffix}")
    set(_helper_output_name "${PROJECT_NAME} Helper${_name_suffix}")

    if(_target_suffix STREQUAL "")
      set(_helper_bundle_identifier "org.zqf.cefruntime.helper")
    else()
      string(TOLOWER "${_target_suffix}" _bundle_id_suffix)
      string(REPLACE "_" "" _bundle_id_suffix "${_bundle_id_suffix}")
      set(_helper_bundle_identifier "org.zqf.cefruntime.helper.${_bundle_id_suffix}")
    endif()

    add_executable(${_helper_target} "src/main.cpp")

    # Compiler
    target_compile_features(${_helper_target} PRIVATE cxx_std_23)
    target_compile_options(${_helper_target} PRIVATE $<$<OR:$<C_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:GNU>>:-Wall -Wextra>)
    target_compile_options(${_helper_target} PRIVATE $<$<OR:$<C_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:AppleClang>>:-Wall -Wextra>)

    # External libraries
    zqf_cef_prebuilt_config(${_helper_target})
    target_link_libraries(${_helper_target} PRIVATE ZQF::CEFPrebuilt::CEF)

    # Properties
    get_target_property(target_out_dir ${_helper_target} RUNTIME_OUTPUT_DIRECTORY)
    set_target_properties(
      ${_helper_target}
      PROPERTIES
      MACOSX_BUNDLE ON
      MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/res/Info.plist.in"
      MACOSX_BUNDLE_BUNDLE_NAME "${_helper_output_name}"
      MACOSX_BUNDLE_GUI_IDENTIFIER "${_helper_bundle_identifier}"
      MACOSX_BUNDLE_BUNDLE_VERSION "${PROJECT_VERSION}"
      MACOSX_BUNDLE_SHORT_VERSION_STRING "${PROJECT_VERSION}"
      MACOSX_BUNDLE_LONG_VERSION_STRING "${PROJECT_VERSION}"
      OUTPUT_NAME ${_helper_output_name}
      RUNTIME_OUTPUT_DIRECTORY "${target_out_dir}/${PROJECT_NAME}/"
    )
  endforeach()
endif()
