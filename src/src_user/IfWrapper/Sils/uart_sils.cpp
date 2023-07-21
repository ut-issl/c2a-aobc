#pragma section REPRO

#include "uart_sils.h"

#include <src_core/IfWrapper/uart.h>


#ifdef USE_UART_COM
#include "com_port.h"
#include "../../Settings/port_config.h"

static ComPort uart_sils_com_port(COM_PORT_SILS);
#endif


int UART_init(void* my_uart_v)
{
  return 0;
}

int UART_rx(void* my_uart_v, void* data_v, int buffer_size)
{
  UART_Config* my_uart = (UART_Config*)my_uart_v;

#ifdef USE_UART_COM
  if (my_uart->ch == PORT_CH_UART_MOBC)
  {
    return uart_sils_com_port.receive((unsigned char*)data_v, 0, buffer_size);
  }
  else
  {
    return OBC_C2A_ReceivedByObc(my_uart->ch, (unsigned char*)data_v, 0, buffer_size);
  }
#else
  return OBC_C2A_ReceivedByObc(my_uart->ch, (unsigned char*)data_v, 0, buffer_size);
#endif
}

int UART_tx(void* my_uart_v, void* data_v, int data_size)
{
  UART_Config* my_uart = (UART_Config*)my_uart_v;

#ifdef USE_UART_COM
  if (my_uart->ch == PORT_CH_UART_MOBC)
  {
    uart_sils_com_port.send((unsigned char*)data_v, 0, data_size);
  }
  else
  {
    int result = OBC_C2A_SendFromObc(my_uart->ch, (unsigned char*)data_v, 0, data_size);

    if (result < 0) return -1;
  }
#else
  int result = OBC_C2A_SendFromObc(my_uart->ch, (unsigned char*)data_v, 0, data_size);

  if (result < 0) return -1;
#endif

  return 0;
}

int UART_reopen(void* my_uart_v, int reason)
{
  return 0;
}

#pragma section
