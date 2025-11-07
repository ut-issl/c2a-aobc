#include "uart_example.h"

#include <src_core/IfWrapper/uart.h>
#include <src_core/Library/print.h>
#include <src_user/Settings/port_config.h>

static UART_Config uart_debug_;

static void DI_uart_example_init_(void);
static void DI_uart_example_exec_(void);

AppInfo DI_uart_example(void)
{
  return AI_create_app_info("DI_uart_example", DI_uart_example_init_, DI_uart_example_exec_);
}

static void DI_uart_example_init_(void)
{
  uart_debug_.ch = PORT_CH_UART_DEBUG;
  uart_debug_.baudrate = 115200;

  // 本来はここで初期化するが，DebugはPrintf用に最初に初期化されているので行わない
  // if(RS422_init(&uart_debug_) != 0)
  // {
  //   Printf("Error: RS422_init in debug_out.\n");
  // }
  return;
}

static void DI_uart_example_exec_(void)
{
  unsigned char test_cmd[50];
  unsigned int rec_cmd_len = 0;

  rec_cmd_len = UART_rx(&uart_debug_, test_cmd, 50);
  if (rec_cmd_len > 0)
  {
    unsigned char header[] = "AOBC:";
    unsigned char footer[] = "\n";
    UART_tx(&uart_debug_, header, 5);
    UART_tx(&uart_debug_, test_cmd, rec_cmd_len);
    UART_tx(&uart_debug_, footer, 1);
  }

  return;
}
