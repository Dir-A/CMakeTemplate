# Project
project("cef_subprocess")

# Executable
add_executable(${PROJECT_NAME} "main.cpp")
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_23)
target_link_libraries(${PROJECT_NAME} PRIVATE CEF::LIB)
CEF_SET_EXECUTABLE_TARGET_PROPERTIES(${PROJECT_NAME})

# Properties
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PROJECT_NAME}")

# Warning
target_compile_options(${PROJECT_NAME} PRIVATE $<$<OR:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>:/W4>)
target_compile_options(${PROJECT_NAME} PRIVATE $<$<OR:$<C_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:GNU>>:-Wall -Wextra>)

# Platform
if("Windows" STREQUAL CMAKE_SYSTEM_NAME)
  target_sources(${PROJECT_NAME} PRIVATE "win/compatibility.manifest")
  set_target_properties(${PROJECT_NAME} PROPERTIES WIN32_EXECUTABLE ON)
elseif("Linux" STREQUAL CMAKE_SYSTEM_NAME)
  # Set rpath so that libraries can be placed next to the executable.
  set_target_properties(${PROJECT_NAME} PROPERTIES INSTALL_RPATH "$ORIGIN")
  set_target_properties(${PROJECT_NAME} PROPERTIES BUILD_WITH_INSTALL_RPATH TRUE)
  set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PROJECT_NAME}")

  if (EXISTS "${CEF_BINARY_DIR}/libminigbm.so")
    CEF_COPY_FILES(${PROJECT_NAME} "libminigbm.so" "${CEF_BINARY_DIR}" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PROJECT_NAME}")
  endif()
endif()

# Runtime Files
CEF_COPY_FILES(${PROJECT_NAME} "${CEF_BINARY_FILES}" "${CEF_BINARY_DIR}" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PROJECT_NAME}")
CEF_COPY_FILES(${PROJECT_NAME} "${CEF_RESOURCE_FILES}" "${CEF_RESOURCE_DIR}" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PROJECT_NAME}")