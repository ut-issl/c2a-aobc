#pragma section REPRO
#include "if_list.h"

#include <stddef.h>

int (*IF_init[])(void* my_if) = { UART_init, I2C_init, SPI_init };

int (*IF_RX[])(void* my_if, void* data_v, int count) = { UART_rx, I2C_rx, SPI_rx };

int (*IF_TX[])(void* my_if, void* data_v, int count) = { UART_tx, I2C_tx, SPI_tx };

int (*IF_reopen[])(void* my_if, int reason) = { UART_reopen, I2C_reopen, SPI_reopen };

#pragma section
