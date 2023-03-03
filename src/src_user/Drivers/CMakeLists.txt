cmake_minimum_required(VERSION 3.13)

project(C2A_USER_DRIVERS)

set(C2A_SRCS
  Aocs/mpu9250.c
  Aocs/mtq_seiren.c
  Aocs/nanossoc_d60.c
  Aocs/oem7600.c
  Aocs/rm3100.c
  Aocs/rw0003.c
  Aocs/sagitta.c
  Aocs/stim210.c
  Cdh/fm25v10.c
  Etc/mobc.c
  Power/ina260.c
)

add_library(${PROJECT_NAME} OBJECT ${C2A_SRCS})

include(${C2A_USER_DIR}/common.cmake)
