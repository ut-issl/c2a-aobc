#pragma section REPRO
/**
 * @file
 * @brief   uart_sils_sci_if
 * @details SILSでDriverのテストをするように作った
 */

#include "uart_sils_if.h"


int uart_sils_if_init(void)
{
  return 0;
}

int uart_sils_if_tx(unsigned char* data_v, int count)
{
  uart_sils_com_port.send(data_v, 0, count);
  return 0;
}

int uart_sils_if_rx(unsigned char* data_v, int count)
{
  return uart_sils_com_port.receive(data_v, 0, count);
}


#pragma section
