cmake_minimum_required(VERSION 3.13)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
if(BUILD_C2A_AS_SILS_FW)
  set(CMAKE_CXX_STANDARD 17)  # SILSのIfWrapperはC++17
else()
  set(CMAKE_CXX_STANDARD 11)
endif()

project(C2A_USER_IF_WRAPPER)

set(C2A_SRCS_COMMON
  if_list.c
)

if(BUILD_C2A_AS_SILS_FW)
  set(C2A_SILS_SRCS
    Sils/uart_sils.cpp
    Sils/i2c_sils.c
    Sils/spi_sils.c
    Sils/adc_sils.c
    Sils/wdt_sils.c
    Sils/gpio_sils.c
  )

  if(USE_UART_COM)
    add_definitions(-DUSE_UART_COM)
    list(APPEND C2A_SILS_SRCS
      Sils/com_port.cpp
    )
    message("Use UART COM")
  endif()
elseif(CMAKE_SYSTEM_PROCESSOR STREQUAL "pic32mx")
  # 実機 向け IfWrapper 実装
  set(C2A_PIC_SRCS
    COMMON/WDT_TEMP.c
    Arduino/CHIPKIT/ADC_CHIPKIT.cpp
    Arduino/CHIPKIT/GPIO_CHIPKIT.cpp
    Arduino/CHIPKIT/I2C_CHIPKIT.cpp
    Arduino/CHIPKIT/spi_chipkit.cpp
    Arduino/CHIPKIT/UART_CHIPKIT.cpp
    Arduino/ADC_ARDUINO.c
    Arduino/GPIO_ARDUINO.c
    Arduino/I2C_ARDUINO.c
    Arduino/spi_arduino.c
    Arduino/UART_ARDUINO.c
  )
endif()

set(C2A_SRCS
  ${C2A_SRCS_COMMON}
  ${C2A_SILS_SRCS}
  ${C2A_PIC_SRCS}
)

if(BUILD_C2A_AS_CXX)
  set_source_files_properties(${C2A_SRCS} PROPERTIES LANGUAGE CXX)  # C++
endif()

add_library(${PROJECT_NAME} OBJECT ${C2A_SRCS})

include(${C2A_USER_DIR}/common.cmake)
