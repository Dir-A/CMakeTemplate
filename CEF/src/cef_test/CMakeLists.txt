# Project
project("cef_test")

# Executable
add_executable(${PROJECT_NAME} 
  "simple_app.cc" 
  "simple_handler.cc"
)
target_link_libraries(${PROJECT_NAME} PRIVATE CEF::FULL)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_23)
CEF_SET_EXECUTABLE_TARGET_PROPERTIES(${PROJECT_NAME})

# Warning
target_compile_options(${PROJECT_NAME} PRIVATE $<$<OR:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>:/W4>)
target_compile_options(${PROJECT_NAME} PRIVATE $<$<OR:$<C_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:GNU>>:-Wall -Wextra>)

# Platform
if("Windows" STREQUAL CMAKE_SYSTEM_NAME)
  target_sources(${PROJECT_NAME} PRIVATE  "cefsimple_win.cc" "simple_handler_win.cc" )
  # set_target_properties(${PROJECT_NAME} PROPERTIES WIN32_EXECUTABLE ON)
elseif("Linux" STREQUAL CMAKE_SYSTEM_NAME)
  target_sources(${PROJECT_NAME} PRIVATE cefsimple_linux.cc simple_handler_linux.cc)
  set_target_properties(${PROJECT_NAME} PROPERTIES INSTALL_RPATH "$ORIGIN/cef_subprocess")
  set_target_properties(${PROJECT_NAME} PROPERTIES BUILD_WITH_INSTALL_RPATH TRUE)
endif()