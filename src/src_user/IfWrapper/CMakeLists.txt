cmake_minimum_required(VERSION 3.13)

project(C2A_USER_IF_WRAPPER)

set(C2A_SRCS
  if_list.c
  Sils/uart_sils.cpp
  Sils/i2c_sils.c
  Sils/spi_sils.c
  Sils/adc_sils.c
  Sils/wdt_sils.c
  Sils/gpio_sils.c
)

if(USE_UART_COM)
  add_definitions(-DUSE_UART_COM)
  list(APPEND C2A_SRCS
    Sils/com_port.cpp
  )
  message("Use UART COM")
endif()

add_library(${PROJECT_NAME} OBJECT ${C2A_SRCS})

include(${C2A_USER_DIR}/common.cmake)
