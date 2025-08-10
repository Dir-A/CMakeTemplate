if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
  set(CEF_SUBPROCESS_RUNTIME_NAME "CEFApp") # same as the app name
else()
  set(CEF_SUBPROCESS_RUNTIME_NAME "CEFRuntime")
endif()

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
  # Project
  project(${CEF_SUBPROCESS_RUNTIME_NAME} VERSION 1.0 LANGUAGES C CXX)
  
  # Executable
  add_executable(${PROJECT_NAME} "src/main.cpp" "res/compatibility.manifest")
  target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_23)
  target_compile_options(${PROJECT_NAME} PRIVATE $<$<OR:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>:/utf-8>)
  target_compile_options(${PROJECT_NAME} PRIVATE $<$<OR:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>:/W4>)
  target_compile_options(${PROJECT_NAME} PRIVATE $<$<OR:$<C_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:GNU>>:-Wall -Wextra>)
  target_link_libraries(${PROJECT_NAME} PRIVATE CEF::LIB)
  CEF_SET_EXECUTABLE_TARGET_PROPERTIES(${PROJECT_NAME})

  # Properties
  set(CEF_TARGET_OUT_DIR "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PROJECT_NAME}")
  set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CEF_TARGET_OUT_DIR}")
  set_target_properties(${PROJECT_NAME} PROPERTIES WIN32_EXECUTABLE ON)

  # Copy Runtime Files
  CEF_COPY_FILES(${PROJECT_NAME} "${CEF_BINARY_FILES}" "${CEF_BINARY_DIR}" "${CEF_TARGET_OUT_DIR}")
  CEF_COPY_FILES(${PROJECT_NAME} "${CEF_RESOURCE_FILES}" "${CEF_RESOURCE_DIR}" "${CEF_TARGET_OUT_DIR}")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
  # Project
  project(${CEF_SUBPROCESS_RUNTIME_NAME} VERSION 1.0 LANGUAGES C CXX)

  # Executable
  add_executable(${PROJECT_NAME} "src/main.cpp")
  target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_23)
  target_compile_options(${PROJECT_NAME} PRIVATE $<$<OR:$<C_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:GNU>>:-Wall -Wextra>)
  target_link_libraries(${PROJECT_NAME} PRIVATE CEF::LIB)
  CEF_SET_EXECUTABLE_TARGET_PROPERTIES(${PROJECT_NAME})

  # Properties
  set(CEF_TARGET_OUT_DIR "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PROJECT_NAME}")
  set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CEF_TARGET_OUT_DIR}")

  # Set rpath so that libraries can be placed next to the executable.
  set_target_properties(${PROJECT_NAME} PROPERTIES INSTALL_RPATH "$ORIGIN")
  set_target_properties(${PROJECT_NAME} PROPERTIES BUILD_WITH_INSTALL_RPATH TRUE)
  set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CEF_TARGET_OUT_DIR}")

  # Copy Runtime Files
  CEF_COPY_FILES(${PROJECT_NAME} "${CEF_BINARY_FILES}" "${CEF_BINARY_DIR}" "${CEF_TARGET_OUT_DIR}")
  CEF_COPY_FILES(${PROJECT_NAME} "${CEF_RESOURCE_FILES}" "${CEF_RESOURCE_DIR}" "${CEF_TARGET_OUT_DIR}")
  if (EXISTS "${CEF_BINARY_DIR}/libminigbm.so")
    CEF_COPY_FILES(${PROJECT_NAME} "libminigbm.so" "${CEF_BINARY_DIR}" "${CEF_TARGET_OUT_DIR}")
  endif()
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
  set(CEF_TARGET_NAME ${CEF_SUBPROCESS_RUNTIME_NAME})
  # Variables referenced from the main Info.plist file.
  set(EXECUTABLE_NAME "${CEF_TARGET_NAME}")
  set(PRODUCT_NAME "${CEF_TARGET_NAME}")

  # Create the multiple Helper app bundle targets.
  set(CEF_HELPER_TARGET "${CEF_TARGET_NAME}_Helper")
  set(CEF_HELPER_OUTPUT_NAME "${CEF_TARGET_NAME} Helper")

  # Make Helpers
  foreach(_suffix_list ${CEF_HELPER_APP_SUFFIXES})
    # Convert to a list and extract the suffix values.
    string(REPLACE ":" ";" _suffix_list ${_suffix_list})
    list(GET _suffix_list 0 _name_suffix)
    list(GET _suffix_list 1 _target_suffix)
    list(GET _suffix_list 2 _plist_suffix)

    # Define Helper target and output names.
    set(_helper_target "${CEF_HELPER_TARGET}${_target_suffix}")
    set(_helper_output_name "${CEF_HELPER_OUTPUT_NAME}${_name_suffix}")

    # Plist Gen
    file(READ "${CMAKE_CURRENT_SOURCE_DIR}/res/helper-Info.plist.in" _plist_contents)
    string(REPLACE "\${EXECUTABLE_NAME}" "${_helper_output_name}" _plist_contents ${_plist_contents})
    string(REPLACE "\${PRODUCT_NAME}" "${_helper_output_name}" _plist_contents ${_plist_contents})
    string(REPLACE "\${BUNDLE_ID_SUFFIX}" "${_plist_suffix}" _plist_contents ${_plist_contents})
    set(_helper_info_plist "${CMAKE_CURRENT_BINARY_DIR}/plist_gen/helper-Info${_target_suffix}.plist")
    file(WRITE ${_helper_info_plist} ${_plist_contents})

    # Create Helper executable target.
    add_executable(${_helper_target} "src/main.cpp")
    target_compile_features(${_helper_target} PRIVATE cxx_std_23)
    target_compile_options(${_helper_target} PRIVATE $<$<OR:$<C_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:GNU>>:-Wall -Wextra>)
    target_compile_options(${_helper_target} PRIVATE $<$<OR:$<C_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:AppleClang>>:-Wall -Wextra>)
    target_link_libraries(${_helper_target} PRIVATE CEF::CEF)
    set_target_properties(${_helper_target} PROPERTIES
      MACOSX_BUNDLE ON
      MACOSX_BUNDLE_INFO_PLIST ${_helper_info_plist}
      OUTPUT_NAME ${_helper_output_name}
      RUNTIME_OUTPUT_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CEF_TARGET_NAME}.app/Contents/Frameworks/"
    )
    SET_EXECUTABLE_TARGET_PROPERTIES(${_helper_target})
  endforeach()
endif()

