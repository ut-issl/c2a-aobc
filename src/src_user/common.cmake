if(BUILD_C2A_AS_CXX)
  set_source_files_properties(${C2A_SRCS} PROPERTIES LANGUAGE CXX)
else()
  set_target_properties(${PROJECT_NAME} PROPERTIES C_STANDARD 99)
  set_target_properties(${PROJECT_NAME} PROPERTIES C_EXTENSIONS FALSE) # no extensions(GNU)
endif()

if(BUILD_C2A_AS_SILS_FW)
  target_compile_definitions(${PROJECT_NAME} PUBLIC SILS_FW)
endif()

# Compile option
if(MSVC)
  target_compile_options(${PROJECT_NAME} PUBLIC "/W4")
  target_compile_options(${PROJECT_NAME} PUBLIC "/MT")
  target_compile_options(${PROJECT_NAME} PUBLIC "/TP") #Compile C codes as C++
  target_compile_options(${PROJECT_NAME} PUBLIC "/source-charset:utf-8")
  # warning
  target_compile_options(${PROJECT_NAME} PRIVATE /wd4083) # disable #pragma SECTION REPRO warning
  target_compile_options(${PROJECT_NAME} PRIVATE /wd4081) # disable #pragma SECTION (EOF)
else()
  # warning
  target_compile_options(${PROJECT_NAME} PUBLIC "-Wall" "-Wno-unknown-pragmas")
  # debug
  target_compile_options(${PROJECT_NAME} PUBLIC "-g")
  if(NOT USE_32BIT_COMPILER)
    target_compile_options(${PROJECT_NAME} PUBLIC "-m32")
    target_link_options(${PROJECT_NAME} PRIVATE "-m32")
  endif()
endif()
