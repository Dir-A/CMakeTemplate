# CEF Settings
set(CEF_DOWNLOAD_PATH "${CMAKE_BINARY_DIR}/cef_bin.tar.gz")
set(CEF_EXTRACT_DIR "${CMAKE_BINARY_DIR}/cef_ext")
set(CEF_ROOT "${CMAKE_BINARY_DIR}/cef_bin")

# Config Donwload Info
if(CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64|AMD64")
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
elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "arm64|aarch64|ARM64")
  if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    set(CEF_DOWNLOAD_TYPE "macosarm64")
    set(CEF_DOWNLOAD_SHA1 "9adc8a60fdf04867ff9d5c6dbacca2be8feff3a0") 
  endif()
else()
  message(FATAL_ERROR "CEF: Config Download Failed: unknown system type")
endif()

set(CEF_DOWNLOAD_URL "https://cef-builds.spotifycdn.com/cef_binary_138.0.23%2Bg26cc530%2Bchromium-138.0.7204.101_${CEF_DOWNLOAD_TYPE}.tar.bz2")

# Donwload
if(EXISTS "${CEF_DOWNLOAD_PATH}")
  file(SHA1 ${CEF_DOWNLOAD_PATH} CEF_PREBUILT_SHA1)
endif()
if(NOT CEF_PREBUILT_SHA1 STREQUAL CEF_DOWNLOAD_SHA1)
  message(STATUS "CEF: Start Download Pre-built: ${CEF_DOWNLOAD_URL}")
  file(DOWNLOAD ${CEF_DOWNLOAD_URL} ${CEF_DOWNLOAD_PATH} EXPECTED_HASH SHA1=${CEF_DOWNLOAD_SHA1} SHOW_PROGRESS STATUS CEF_DOWNLOAD_RESULT)

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
  file(ARCHIVE_EXTRACT INPUT "${CEF_DOWNLOAD_PATH}" DESTINATION "${CEF_EXTRACT_DIR}" VERBOSE)

  # Find Sub Dir
  file(GLOB FOUND_CEF_DIR_LIST LIST_DIRECTORIES true "${CEF_EXTRACT_DIR}/cef_binary_*")
  if(NOT FOUND_CEF_DIR_LIST)
    message(FATAL_ERROR "CEF: Could not find 'cef_binary_*' directory in temp folder.")
  endif()
  list(GET FOUND_CEF_DIR_LIST 0 FOUND_CEF_DIR_FIRST)

  # Rename to prevent long paths
  file(RENAME "${FOUND_CEF_DIR_FIRST}" "${CEF_ROOT}")
  file(REMOVE_RECURSE "${CEF_EXTRACT_DIR}" )
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
  add_library(CEF::CEF ALIAS cef_full)
endif()

# Append Macro CEF_ Prefix
macro(CEF_SET_EXECUTABLE_TARGET_PROPERTIES target)
  SET_EXECUTABLE_TARGET_PROPERTIES("${target}")
endmacro()

macro(CEF_COPY_FILES target file_list source_dir target_dir)
  COPY_FILES("${target}" "${file_list}" "${source_dir}" "${target_dir}")
endmacro()
