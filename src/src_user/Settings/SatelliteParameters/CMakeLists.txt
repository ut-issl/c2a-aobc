cmake_minimum_required(VERSION 3.13)

project(C2A_USER_SETTINGS_SATELLITE_PARAMETERS)

set(C2A_SRCS
  Sample/orbit_parameters.c
  Sample/structure_parameters.c
)

add_library(${PROJECT_NAME} OBJECT ${C2A_SRCS})

include(${C2A_USER_DIR}/common.cmake)
