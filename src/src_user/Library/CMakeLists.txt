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
  Orbit/kepler_orbit.c
  Orbit/sgp4.c
  SignalProcess/z_filter.c
  SignalProcess/spike_filter.c
  SignalProcess/polynomial_approximation.c
)

add_library(${PROJECT_NAME} OBJECT ${C2A_SRCS})

include(${C2A_USER_DIR}/common.cmake)
