cmake_minimum_required(VERSION 3.13)

project(C2A_USER_LIB)

set(C2A_SRCS
  pid_control.c
  ControlUtility/cross_product_control.c
  ControlUtility/gyroscopic_effect.c
  print.c
  math_constants.c
  physical_constants.c
  matrix33.c
  matrix.c
  vector3.c
  xxhash.c
  vt100.c
  time_space.c
  geomagnetism.c
  c2a_math.c
  quaternion.c
  crc8.c
  TimeSystem/gps_time.c
  Orbit/kepler_orbit.c
  Orbit/sgp4.c
  SignalProcess/z_filter.c
  SignalProcess/spike_filter.c
  SignalProcess/polynomial_approximation.c
)

if(CMAKE_SYSTEM_PROCESSOR STREQUAL "pic32mx")
  set(C2A_PIC_SRCS
    print_arduino.cpp
  )
endif()

add_library(${PROJECT_NAME} OBJECT ${C2A_SRCS} ${C2A_PIC_SRCS})

include(${C2A_USER_DIR}/common.cmake)
