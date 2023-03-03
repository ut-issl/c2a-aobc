cmake_minimum_required(VERSION 3.13)

project(C2A_USER_IF_WRAPPER)

set(C2A_SRCS
  if_list.c
  # SILS/CCSDS_SILS.c
  # SILS/DC_SILS.c
  # SILS/FLASH_SILS.c
  SILS/UART_SILS.c
  SILS/I2C_SILS.c
  SILS/spi_sils.c
  SILS/ADC_SILS.c
  SILS/WDT_SILS.c
  SILS/GPIO_SILS.c
)

if(USE_SCI_COM)
  add_definitions(-DUSE_SCI_COM)
  list(APPEND C2A_SRCS
    SILS/CCSDS_SILS_SCI_IF.c
  )
  message("USE SCI_COM")
endif()

if(USE_SCI_COM_UART)
  add_definitions(-DUSE_SCI_COM_UART)
  list(APPEND C2A_SRCS
    SILS/uart_sils_sci_if.cpp
  )
  message("USE SCI_COM")
endif()

add_library(${PROJECT_NAME} OBJECT ${C2A_SRCS})

include(${C2A_USER_DIR}/common.cmake)
