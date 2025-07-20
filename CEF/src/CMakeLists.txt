# Project
project("MyCEFApp" VERSION 1.0 LANGUAGES C CXX)

# Executable
add_executable(${PROJECT_NAME} 
  "simple_app.cc" 
  "simple_handler.cc"
)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_23)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# Exteranl Libraries
target_link_libraries(${PROJECT_NAME} PRIVATE CEF::CEF)

# Properties
CEF_SET_EXECUTABLE_TARGET_PROPERTIES(${PROJECT_NAME})

# Warning
target_compile_options(${PROJECT_NAME} PRIVATE $<$<OR:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>:/W4>)
target_compile_options(${PROJECT_NAME} PRIVATE $<$<OR:$<C_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:GNU>>:-Wall -Wextra>)
target_compile_options(${PROJECT_NAME} PRIVATE $<$<OR:$<C_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:AppleClang>>:-Wall -Wextra>)

# Platform
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
  target_sources(${PROJECT_NAME} PRIVATE "win/cefsimple.cc" "win/simple_handler.cc" )
  set_target_properties(${PROJECT_NAME} PROPERTIES 
    WIN32_EXECUTABLE ON
  )
  CEF_MAKE_RUNTIME_TARGET(CEFRuntime "win/CEFRuntime.cpp" "win/compatibility.manifest")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
  target_sources(${PROJECT_NAME} PRIVATE "linux/cefsimple.cc" "linux/simple_handler.cc")
  set_target_properties(${PROJECT_NAME} PROPERTIES 
    BUILD_WITH_INSTALL_RPATH TRUE
    INSTALL_RPATH "$ORIGIN/CEFRuntime"
  )
  CEF_MAKE_RUNTIME_TARGET(CEFRuntime "linux/CEFRuntime.cpp")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
  target_sources(${PROJECT_NAME} PRIVATE "mac/cefsimple.mm" "mac/simple_handler.mm")
  set_target_properties(${PROJECT_NAME} PROPERTIES 
    MACOSX_BUNDLE ON
    MACOSX_BUNDLE_INFO_PLIST "mac/Info.plist.in"
  )
  CEF_MAKE_RUNTIME_TARGET(${PROJECT_NAME} "mac/CEFRuntime.cpp")
  # ARC
  option(OPTION_USE_ARC "Build with ARC (automatic Reference Counting) on macOS." ON)
  if(OPTION_USE_ARC)
    list(APPEND CEF_COMPILER_FLAGS -fobjc-arc)
    set_target_properties(${target} PROPERTIES CLANG_ENABLE_OBJC_ARC "YES")
  endif()
endif()

