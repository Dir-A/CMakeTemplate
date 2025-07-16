project("cef_test")

add_executable(${PROJECT_NAME} 
  "simple_app.cc" 
  "simple_handler.cc")

target_link_libraries(${PROJECT_NAME} PRIVATE CEF::FULL)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_23)

if(OS_WINDOWS)
  target_sources(${PROJECT_NAME} PRIVATE  "cefsimple_win.cc" "simple_handler_win.cc" )
  # set_target_properties(${PROJECT_NAME} PROPERTIES WIN32_EXECUTABLE ON)
  CEF_SET_EXECUTABLE_TARGET_PROPERTIES(${PROJECT_NAME})
endif()
