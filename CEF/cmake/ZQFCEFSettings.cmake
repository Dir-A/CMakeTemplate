set(CEF_DOWNLOAD_URL "https://cef-builds.spotifycdn.com/cef_binary_138.0.23%2Bg26cc530%2Bchromium-138.0.7204.101_windows64.tar.bz2")
set(CEF_DOWNLOAD_SHA1 "c0954e75eb0b3091535d2314896f40357c9d99d8")
set(CEF_DOWNLOAD_PATH "${CMAKE_BINARY_DIR}/cef_prebuild.tar.gz")
set(CEF_EXTRACT_DIR "${CMAKE_BINARY_DIR}/cef_prebuild")
set(CEF_ROOT "${CEF_EXTRACT_DIR}/bin")

# Download && Extract
if(NOT EXISTS "${CEF_EXTRACT_DIR}")
    # Donwload
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
            message(FATAL_ERROR "Download CEF binary failed: [${status_code}] ${status_message}")
        endif()
    else()
        message(FATAL_ERROR "Download CEF binary failed: unknown error")
    endif()

    # Extract
    file(ARCHIVE_EXTRACT
        INPUT "${CEF_DOWNLOAD_PATH}"
        DESTINATION "${CEF_EXTRACT_DIR}"
        VERBOSE
    )

    # Find Sub Dir
    file(GLOB FOUND_CEF_DIR_LIST
        LIST_DIRECTORIES true
        "${CEF_EXTRACT_DIR}/cef_binary_*"
    )
    if(NOT FOUND_CEF_DIR_LIST)
        message(FATAL_ERROR "Could not find 'cef_binary_*' directory in temp folder.")
    endif()
    list(GET FOUND_CEF_DIR_LIST 0 FOUND_CEF_DIR_FIRST)

    # Rename to prevent long paths
    file(RENAME "${FOUND_CEF_DIR_FIRST}" "${CEF_ROOT}")
endif()

# Make CEF's MSVC_RUNTIME Follow The CMAKE Settings
if((${CMAKE_SYSTEM_NAME} STREQUAL "Windows") AND MSVC)
    set(MY_RUNTIME_LIBRARY "/MD")
    if(CMAKE_MSVC_RUNTIME_LIBRARY)
        if((${CMAKE_MSVC_RUNTIME_LIBRARY} STREQUAL "MultiThreaded") OR (${CMAKE_MSVC_RUNTIME_LIBRARY} STREQUAL "MultiThreadedDebug"))
            set(MY_RUNTIME_LIBRARY "/MT")
        endif()
    endif()
    set(CEF_RUNTIME_LIBRARY_FLAG ${MY_RUNTIME_LIBRARY} CACHE STRING "MSVC Runtime Libaray" FORCE)
endif()

# Custom CEF Settings
option(USE_ATL OFF)
option(USE_SANDBOX OFF)

# Config CEF C Library
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CEF_ROOT}/cmake")
find_package(CEF REQUIRED)

# Remove CEF Force Set CXX Standard
list(REMOVE_ITEM CEF_CXX_COMPILER_FLAGS "-std=c++17")
list(REMOVE_ITEM CEF_CXX_COMPILER_FLAGS "/std:c++17")

# # Add Standard
# if (NOT CMAKE_CXX_STANDARD)
#     # Add Default Standard For All Target if Target Not Set Standard
#     set(CMAKE_CXX_STANDARD 17)
#     set(CMAKE_CXX_STANDARD_REQUIRED ON)
# else()
#     if(CMAKE_CXX_STANDARD LESS 17)
#         message(FATAL_ERROR "CMAKE_CXX_STANDARD must be at least 17 for CEF.")
#     endif()
# endif()

# Add CEF C Libary Target
if(OS_MAC)
  set(CEF_HELPER_TARGET "cefsimple_Helper")
  set(CEF_HELPER_OUTPUT_NAME "cefsimple Helper")
else()
  ADD_LOGICAL_TARGET("libcef_lib" "${CEF_LIB_DEBUG}" "${CEF_LIB_RELEASE}")
endif()

# Add CEF CXX Wrapper Library
add_subdirectory(${CEF_LIBCEF_DLL_WRAPPER_PATH})
target_compile_features(libcef_dll_wrapper PRIVATE cxx_std_17)

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

# Append Macro CEF_ Prefix
macro(CEF_SET_EXECUTABLE_TARGET_PROPERTIES target)
  SET_EXECUTABLE_TARGET_PROPERTIES("${target}")
endmacro()

macro(CEF_COPY_FILES target file_list source_dir target_dir)
  COPY_FILES("${target}" "${file_list}" "${source_dir}" "${target_dir}")
endmacro()
