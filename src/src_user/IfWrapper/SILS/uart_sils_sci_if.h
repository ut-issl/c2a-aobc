/**
 * @file
 * @brief   uart_sils_sci_if
 * @details SILSでDriverのテストをするように作った
            ccsds_sils_sci_if.c/hのほぼコピー
 */
#ifndef UART_SILS_SCI_IF_H_
#define UART_SILS_SCI_IF_H_

#include <Windows.h>

class SciComPortUart
{
public:
  SciComPortUart(void);
  ~SciComPortUart(void);

  int send(unsigned char* buffer, size_t length, size_t offset);
  int receive(unsigned char* buffer, size_t length, size_t offset);

private:
  HANDLE myHComPort_;
  DCB config_;
  bool init_success_;
};

int SILS_SCI_UART_IF_init();
int SILS_SCI_UART_IF_tx(unsigned char* data_v, int count);
int SILS_SCI_UART_IF_rx(unsigned char* data_v, int count);

#endif
