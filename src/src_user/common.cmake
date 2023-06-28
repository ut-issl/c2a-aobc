if(BUILD_C2A_AS_CXX)
  set_source_files_properties(${C2A_SRCS} PROPERTIES LANGUAGE CXX)
endif()

target_compile_definitions(${PROJECT_NAME} PUBLIC SILS_FW)

# Compile option
if(MSVC)
  target_compile_options(${PROJECT_NAME} PUBLIC "/W4")
  target_compile_options(${PROJECT_NAME} PUBLIC "/MT")
  target_compile_options(${PROJECT_NAME} PUBLIC "/TP") #Compile C codes as C++
  target_compile_options(${PROJECT_NAME} PUBLIC "/source-charset:utf-8")
  # warning
  target_compile_options(${PROJECT_NAME} PRIVATE /wd4083) # disable #pragma SECTION REPRO warning
  target_compile_options(${PROJECT_NAME} PRIVATE /wd4081) # disable #pragma section (EOF)
else()
  target_compile_options(${PROJECT_NAME} PUBLIC "${CMAKE_CXX_FLAGS}-Wall")
  set(CMAKE_CXX_FLAGS "-m32 -rdynamic -Wall -g")  # SJIS, 32bit
  set(CMAKE_C_FLAGS "-m32 -rdynamic -Wall -g") # SJIS, 32bit
endif()
