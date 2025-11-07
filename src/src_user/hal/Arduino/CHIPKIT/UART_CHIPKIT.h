
#ifndef UART_CHIPKIT_H_
#define UART_CHIPKIT_H_

#include <src_core/IfWrapper/uart.h>

#ifdef __cplusplus
extern "C" {
#endif

  // 基本的に下記関数の引数，戻り値はuart.hに準ずる
  int UART_CHIPKIT_init(void* my_uart_v);
  int UART_CHIPKIT_rx(void* my_uart_v, void* data_v, int buffer_size);
  int UART_CHIPKIT_tx(void* my_uart_v, void* data_v, int data_size);
  int UART_CHIPKIT_reopen(void* my_uart_v, int reason);

#ifdef __cplusplus
};
#endif

#endif
