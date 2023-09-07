## CMake file for S2E integration

cmake_minimum_required(VERSION 3.13)
project(C2A)

option(BUILD_C2A_AS_CXX "Build C2A as C++" ON)

option(BUILD_C2A_AS_SILS_FW "Build C2A as SILS firmware" ON)

# UART COM for connection to PC UART
# これをONにした状態でTMTC_IFが動いていない場合，そちらのバッファが詰まってSILSの動作が止まることがあるので注意すること！
option(USE_UART_COM "Use UART COM" OFF)

set(C2A_CORE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src_aobc/src/src_core)
set(C2A_USER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src_aobc/src/src_user)
set(C2A_SATELLITE_PARAMETERS_DIR ../../../../src_aobc_example/Settings/SatelliteParameters/) # relative path from Settings
add_definitions(-DUSER_DEFINED_I2C_ADDRESS)
add_definitions(-DUSER_DEFINED_SENSOR_NUMBER)

if(NOT BUILD_C2A_AS_CXX)
  set(BUILD_C2A_AS_C99 ON)
endif()
set(USE_ALL_C2A_CORE_APPS OFF)
set(USE_ALL_C2A_CORE_TEST_APPS OFF)
set(USE_ALL_C2A_CORE_LIB ON)

set(C2A_SRCS
  ${C2A_CORE_DIR}/Applications/divided_cmd_utility.c
  ${C2A_CORE_DIR}/Applications/event_utility.c
  ${C2A_CORE_DIR}/Applications/gs_command_dispatcher.c
  ${C2A_CORE_DIR}/Applications/memory_dump.c
  ${C2A_CORE_DIR}/Applications/nop.c
  ${C2A_CORE_DIR}/Applications/realtime_command_dispatcher.c
  ${C2A_CORE_DIR}/Applications/timeline_command_dispatcher.c
  # ${C2A_CORE_DIR}/Applications/utility_command.c
  # ${C2A_CORE_DIR}/Applications/utility_counter.c

  # SRC_USER codes
  # ${C2A_USER_DIR}/C2A_main.c
)

include_directories(src_aobc/src)
include_directories(src_aobc_example)

add_subdirectory(${C2A_CORE_DIR})

add_subdirectory(${C2A_USER_DIR}/Applications)
add_subdirectory(${C2A_USER_DIR}/Drivers)
add_subdirectory(${C2A_USER_DIR}/IfWrapper)
add_subdirectory(${C2A_USER_DIR}/Library)
add_subdirectory(${C2A_USER_DIR}/Settings)
add_subdirectory(${C2A_USER_DIR}/TlmCmd)

add_library(${PROJECT_NAME} STATIC ${C2A_SRCS})

set(C2A_USER_MODULES
  C2A_USER_APPS
  C2A_USER_DRIVERS
  C2A_USER_CMD_TLM
  C2A_USER_IF_WRAPPER
  C2A_USER_LIB
  C2A_USER_SETTINGS
)

if(MSVC)
  target_link_options(${PROJECT_NAME} PRIVATE "/WHOLEARCHIVE")
  target_link_libraries(${PROJECT_NAME} PRIVATE
    C2A_CORE
    ${C2A_USER_MODULES}
  )
else()
  target_link_libraries(${PROJECT_NAME} PRIVATE
    -Wl,--whole-archive
    C2A_CORE
    -Wl,--no-whole-archive
    ${C2A_USER_MODULES}
    -lm
  )
endif()

include(${C2A_USER_DIR}/common.cmake)
