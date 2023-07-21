#ifndef UART_SILS_H_
#define UART_SILS_H_


int OBC_C2A_SendFromObc(int port_id, unsigned char* buffer, int offset, int count);
int OBC_C2A_ReceivedByObc(int port_id, unsigned char* buffer, int offset, int count);

int UART_init(void* my_uart_v);
int UART_rx(void* my_uart_v, void* data_v, int buffer_size);
int UART_tx(void* my_uart_v, void* data_v, int data_size);
int UART_reopen(void* my_uart_v, int reason);

#endif
