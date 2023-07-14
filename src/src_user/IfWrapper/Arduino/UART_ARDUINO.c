
#include "./CHIPKIT/UART_CHIPKIT.h"
#include <src_user/Settings/port_config.h>

int UART_init(void* my_uart_v)
{
  return UART_CHIPKIT_init(my_uart_v);
}

int UART_rx(void* my_uart_v, void* data_v, int buffer_size)
{
  return UART_CHIPKIT_rx(my_uart_v, data_v, buffer_size);
}

int UART_tx(void* my_uart_v, void* data_v, int data_size)
{
  return UART_CHIPKIT_tx(my_uart_v, data_v, data_size);
}

int UART_reopen(void* my_uart_v, int reason)
{
  return UART_CHIPKIT_reopen(my_uart_v, reason);
}
