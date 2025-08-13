# Project
project(${MY_CEF_APP_NAME} VERSION 1.0 LANGUAGES C CXX)

# Executable
add_executable(${PROJECT_NAME} 
  "src/AppMain.cpp" 
  "src/MyApp.cpp" 
  "src/MyClient.cpp" 
  "src/Delegate/MyWndDelegate.cpp"
  "src/Delegate/MyBrowserViewDelegate.cpp" )
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_23)
target_compile_options(${PROJECT_NAME} PRIVATE $<$<OR:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>:/utf-8>)
target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src")

# Warning
target_compile_options(${PROJECT_NAME} PRIVATE $<$<OR:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>:/W4>)
target_compile_options(${PROJECT_NAME} PRIVATE $<$<OR:$<C_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:GNU>>:-Wall -Wextra>)
target_compile_options(${PROJECT_NAME} PRIVATE $<$<OR:$<C_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:AppleClang>>:-Wall -Wextra>)

# Exteranl Libraries
target_link_libraries(${PROJECT_NAME} PRIVATE CEF::CEF)

# Properties
CEF_SET_EXECUTABLE_TARGET_PROPERTIES(${PROJECT_NAME})

# Platform
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
  target_sources(${PROJECT_NAME} PRIVATE "src/Platform/Windows/main.cpp")
  set_target_properties(${PROJECT_NAME} PROPERTIES WIN32_EXECUTABLE ON)
  target_compile_definitions(${PROJECT_NAME} PRIVATE "CEF_RUNTIME_DIR=\"${MY_CEF_RUNTIME_NAME}\"")
  target_compile_definitions(${PROJECT_NAME} PRIVATE "CEF_RUNTIME_NAME=\"${MY_CEF_RUNTIME_NAME}.exe\"")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
  target_sources(${PROJECT_NAME} PRIVATE "src/Platform/Linux/main.cpp")
  set_target_properties(${PROJECT_NAME} PROPERTIES BUILD_WITH_INSTALL_RPATH TRUE INSTALL_RPATH "$ORIGIN/${MY_CEF_RUNTIME_NAME}")
  target_compile_definitions(${PROJECT_NAME} PRIVATE "CEF_RUNTIME_DIR=\"${MY_CEF_RUNTIME_NAME}\"")
  target_compile_definitions(${PROJECT_NAME} PRIVATE "CEF_RUNTIME_NAME=\"${MY_CEF_RUNTIME_NAME}\"")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
  target_sources(${PROJECT_NAME} PRIVATE "src/Platform/macOS/main.mm")
  set_target_properties(${PROJECT_NAME} PROPERTIES MACOSX_BUNDLE ON MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/res/Info.plist.in")
  # ARC
  option(OPTION_USE_ARC "Build with ARC (automatic Reference Counting) on macOS." ON)
  if(OPTION_USE_ARC)
    list(APPEND CEF_COMPILER_FLAGS -fobjc-arc)
    set_target_properties(${target} PROPERTIES CLANG_ENABLE_OBJC_ARC "YES")
  endif()
  # Copy Runtime Files
  add_custom_command(
    TARGET ${PROJECT_NAME}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CEF_BINARY_DIR}/Chromium Embedded Framework.framework"
            "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PROJECT_NAME}.app/Contents/Frameworks/Chromium Embedded Framework.framework"
    VERBATIM
  )
endif()

