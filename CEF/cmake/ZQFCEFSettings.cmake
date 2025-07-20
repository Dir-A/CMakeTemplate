# CEF Settings
set(CEF_DOWNLOAD_PATH "${CMAKE_BINARY_DIR}/cef_prebuild.tar.gz")
set(CEF_EXTRACT_DIR "${CMAKE_BINARY_DIR}/cef_prebuild")
set(CEF_ROOT "${CEF_EXTRACT_DIR}/bin")

# Config Donwload Info
if(CMAKE_SYSTEM_PROCESSOR MATCHES "AMD64|x86_64")
  if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
      set(CEF_DOWNLOAD_TYPE "windows64")
      set(CEF_DOWNLOAD_SHA1 "c0954e75eb0b3091535d2314896f40357c9d99d8")
    elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
      set(CEF_DOWNLOAD_TYPE "linux64")
      set(CEF_DOWNLOAD_SHA1 "43f3ca820b4aac464b0c5125e3df05edf36532c3")
    elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
      set(CEF_DOWNLOAD_TYPE "macosx64")
      set(CEF_DOWNLOAD_SHA1 "0cee64095113bdaf4ce26056ba22ec6d0218e32b") 
    endif()
  endif()
elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "ARM64|aarch64")
else()
  message(FATAL_ERROR "CEF: Config Download Failed: unknown system type")
endif()

# Donwload
if(EXISTS "${CEF_DOWNLOAD_PATH}")
  file(SHA1 ${CEF_DOWNLOAD_PATH} CEF_PREBUILT_SHA1)
endif()
if(NOT CEF_PREBUILT_SHA1 STREQUAL CEF_DOWNLOAD_SHA1)
  set(CEF_DOWNLOAD_URL "https://cef-builds.spotifycdn.com/cef_binary_138.0.23%2Bg26cc530%2Bchromium-138.0.7204.101_${CEF_DOWNLOAD_TYPE}.tar.bz2")

  message(STATUS "CEF: Start Download Pre-built: ${CEF_DOWNLOAD_URL}")

  file(DOWNLOAD ${CEF_DOWNLOAD_URL} ${CEF_DOWNLOAD_PATH}
    EXPECTED_HASH SHA1=${CEF_DOWNLOAD_SHA1}
    SHOW_PROGRESS
    STATUS CEF_DOWNLOAD_RESULT
  )

  # Check Download
  if(CEF_DOWNLOAD_RESULT)
    list(GET CEF_DOWNLOAD_RESULT 0 status_code)
    list(GET CEF_DOWNLOAD_RESULT 1 status_message)
    if(NOT status_code EQUAL 0)
      message(FATAL_ERROR "CEF: Download Pre-built Failed: [${status_code}] ${status_message}")
    endif()
  else()
    message(FATAL_ERROR "CEF: Download Pre-built Failed: unknown error")
  endif()
endif()

# Extract
if(NOT EXISTS "${CEF_ROOT}")
  file(ARCHIVE_EXTRACT
    INPUT "${CEF_DOWNLOAD_PATH}"
    DESTINATION "${CEF_EXTRACT_DIR}"
    VERBOSE
  )

  # Find Sub Dir
  file(GLOB FOUND_CEF_DIR_LIST LIST_DIRECTORIES true "${CEF_EXTRACT_DIR}/cef_binary_*")
  if(NOT FOUND_CEF_DIR_LIST)
    message(FATAL_ERROR "CEF: Could not find 'cef_binary_*' directory in temp folder.")
  endif()
  list(GET FOUND_CEF_DIR_LIST 0 FOUND_CEF_DIR_FIRST)

  # Rename to prevent long paths
  file(RENAME "${FOUND_CEF_DIR_FIRST}" "${CEF_ROOT}")
endif()

# Custom CEF Settings
option(USE_ATL OFF)
option(USE_SANDBOX OFF)

# Config CEF C Library
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CEF_ROOT}/cmake")
find_package(CEF REQUIRED)

# Remove CEF Force Set Flags
list(REMOVE_ITEM CEF_CXX_COMPILER_FLAGS "-std=c++17")
list(REMOVE_ITEM CEF_CXX_COMPILER_FLAGS "/std:c++17")
list(REMOVE_ITEM CEF_COMPILER_FLAGS_RELEASE "/MT")
list(REMOVE_ITEM CEF_COMPILER_FLAGS_DEBUG "/MTd")

# Add CEF CXX Wrapper Library
add_subdirectory(${CEF_LIBCEF_DLL_WRAPPER_PATH} libcef_dll_wrapper)
target_compile_features(libcef_dll_wrapper PRIVATE cxx_std_17)

# Wrapper CEF Target
if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
  add_library(cef_cef INTERFACE)
  add_library(CEF::CEF ALIAS cef_cef)
  target_link_libraries(cef_cef INTERFACE libcef_dll_wrapper ${CEF_STANDARD_LIBS})
else()
  # Add CEF C Libary Target 
  ADD_LOGICAL_TARGET("libcef_lib" "${CEF_LIB_DEBUG}" "${CEF_LIB_RELEASE}")

  # Wrapper CEF C Library
  add_library(cef_lib INTERFACE)
  add_library(CEF::LIB ALIAS cef_lib)
  target_link_libraries(cef_lib INTERFACE libcef_lib ${CEF_STANDARD_LIBS})

  # Wrapper CEF CXX Wrapper Library
  add_library(cef_cxx_wrapper INTERFACE)
  add_library(CEF::CXX::Wrapper ALIAS cef_cxx_wrapper)
  target_link_libraries(cef_cxx_wrapper INTERFACE libcef_dll_wrapper)

  # Combine CEF C Libary And CXX Wrapper
  add_library(cef_full INTERFACE)
  add_library(CEF::FULL ALIAS cef_full)
  target_link_libraries(cef_full INTERFACE CEF::LIB CEF::CXX::Wrapper)

  # CEF
  add_library(cef_cef INTERFACE)
  add_library(CEF::CEF ALIAS cef_full)
endif()

# Append Macro CEF_ Prefix
macro(CEF_SET_EXECUTABLE_TARGET_PROPERTIES target)
  SET_EXECUTABLE_TARGET_PROPERTIES("${target}")
endmacro()

macro(CEF_COPY_FILES target file_list source_dir target_dir)
  COPY_FILES("${target}" "${file_list}" "${source_dir}" "${target_dir}")
endmacro()

macro(CEF_MAKE_RUNTIME_TARGET arg_Runtime_Name)
  # Project
  set(CEF_TARGET_NAME ${arg_Runtime_Name})
  if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    # Executable
    add_executable(${CEF_TARGET_NAME} "${ARGN}")
    target_compile_features(${CEF_TARGET_NAME} PRIVATE cxx_std_17)
    target_link_libraries(${CEF_TARGET_NAME} PRIVATE CEF::LIB)
    CEF_SET_EXECUTABLE_TARGET_PROPERTIES(${CEF_TARGET_NAME})

    # Properties
    set(CEF_TARGET_OUT_DIR "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CEF_TARGET_NAME}")
    set_target_properties(${CEF_TARGET_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CEF_TARGET_OUT_DIR}")
    set_target_properties(${CEF_TARGET_NAME} PROPERTIES WIN32_EXECUTABLE ON)

    # Copy Runtime Files
    CEF_COPY_FILES(${CEF_TARGET_NAME} "${CEF_BINARY_FILES}" "${CEF_BINARY_DIR}" "${CEF_TARGET_OUT_DIR}")
    CEF_COPY_FILES(${CEF_TARGET_NAME} "${CEF_RESOURCE_FILES}" "${CEF_RESOURCE_DIR}" "${CEF_TARGET_OUT_DIR}")

    add_definitions(-DCEF_RUNTIME_DIR="${CEF_TARGET_NAME}")
    add_definitions(-DCEF_RUNTIME_NAME="${CEF_TARGET_NAME}.exe")
  elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    # Executable
    add_executable(${CEF_TARGET_NAME} "${ARGN}")
    target_compile_features(${CEF_TARGET_NAME} PRIVATE cxx_std_17)
    target_link_libraries(${CEF_TARGET_NAME} PRIVATE CEF::LIB)
    CEF_SET_EXECUTABLE_TARGET_PROPERTIES(${CEF_TARGET_NAME})

    # Properties
    set(CEF_TARGET_OUT_DIR "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CEF_TARGET_NAME}")
    set_target_properties(${CEF_TARGET_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CEF_TARGET_OUT_DIR}")

    # Set rpath so that libraries can be placed next to the executable.
    set_target_properties(${CEF_TARGET_NAME} PROPERTIES INSTALL_RPATH "$ORIGIN")
    set_target_properties(${CEF_TARGET_NAME} PROPERTIES BUILD_WITH_INSTALL_RPATH TRUE)
    set_target_properties(${CEF_TARGET_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CEF_TARGET_OUT_DIR}")

    # Copy Runtime Files
    CEF_COPY_FILES(${CEF_TARGET_NAME} "${CEF_BINARY_FILES}" "${CEF_BINARY_DIR}" "${CEF_TARGET_OUT_DIR}")
    CEF_COPY_FILES(${CEF_TARGET_NAME} "${CEF_RESOURCE_FILES}" "${CEF_RESOURCE_DIR}" "${CEF_TARGET_OUT_DIR}")
    if (EXISTS "${CEF_BINARY_DIR}/libminigbm.so")
      CEF_COPY_FILES(${CEF_TARGET_NAME} "libminigbm.so" "${CEF_BINARY_DIR}" "${CEF_TARGET_OUT_DIR}")
    endif()

    add_definitions(-DCEF_RUNTIME_DIR="${CEF_TARGET_NAME}")
    add_definitions(-DCEF_RUNTIME_NAME="${CEF_TARGET_NAME}")
  elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
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
      file(READ "${CMAKE_CURRENT_SOURCE_DIR}/mac/helper-Info.plist.in" _plist_contents)
      string(REPLACE "\${EXECUTABLE_NAME}" "${_helper_output_name}" _plist_contents ${_plist_contents})
      string(REPLACE "\${PRODUCT_NAME}" "${_helper_output_name}" _plist_contents ${_plist_contents})
      string(REPLACE "\${BUNDLE_ID_SUFFIX}" "${_plist_suffix}" _plist_contents ${_plist_contents})
      set(_helper_info_plist "${CMAKE_CURRENT_BINARY_DIR}/plist_gen/helper-Info${_target_suffix}.plist")
      file(WRITE ${_helper_info_plist} ${_plist_contents})

      # Create Helper executable target.
      add_executable(${_helper_target} "${ARGN}")
      target_compile_features(${_helper_target} PRIVATE cxx_std_17)
      target_link_libraries(${_helper_target} libcef_dll_wrapper ${CEF_STANDARD_LIBS})
      set_target_properties(${_helper_target} PROPERTIES
        MACOSX_BUNDLE ON
        MACOSX_BUNDLE_INFO_PLIST ${_helper_info_plist}
        OUTPUT_NAME ${_helper_output_name}
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CEF_TARGET_NAME}.app/Contents/Frameworks/"
      )
      SET_EXECUTABLE_TARGET_PROPERTIES(${_helper_target})
    endforeach()

    # Copy Runtime Files
    add_custom_command(
      TARGET ${arg_Runtime_Name}
      POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_directory
              "${CEF_BINARY_DIR}/Chromium Embedded Framework.framework"
              "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${arg_Runtime_Name}.app/Contents/Frameworks/Chromium Embedded Framework.framework"
      VERBATIM
    )
  endif()
endmacro()