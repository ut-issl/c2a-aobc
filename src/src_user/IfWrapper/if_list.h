#ifndef IF_LIST_H_
#define IF_LIST_H_

#include <src_core/IfWrapper/uart.h>
#include <src_core/IfWrapper/i2c.h>
#include <src_core/IfWrapper/spi.h>

typedef enum
{
  UART,
  I2C,
  SPI,
  IF_LIST_MAX
} IF_LIST_ENUM;

extern  int     (*IF_init[]  )(void* my_if);
extern  int     (*IF_RX[]    )(void* my_if, void* data_v, int count);
extern  int     (*IF_TX[]    )(void* my_if, void* data_v, int count);
extern  int     (*IF_reopen[])(void* my_if, int reason);

#endif
