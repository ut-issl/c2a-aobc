
// Arduino(C++)のSerialポートををCで使えるようにするためのラッパ関数

#include "./UART_CHIPKIT.h"
#include <src_user/Settings/port_config.h>

#include <Arduino.h>

// Private functions
/**
 * @brief UARTポートを選択する
 * @param[in] ch UARTポート番号 PICの制約から0から5まで
 * @return chに該当するHardwareSerialのポインタが返ってくる。ch>6ではNULLが返ってくるので適切な対処が必要
 */
static HardwareSerial* UART_CHIPKIT_select_serial_port_(const unsigned char ch);

// Global functions
int UART_CHIPKIT_init(void* my_uart_v)
{
  UART_Config* my_uart = (UART_Config*)my_uart_v;

  if ((my_uart->ch < 0) || (my_uart->ch >= PORT_CH_MAX_UART_PORT))
  {
    return (int)UART_ERR_CODE::UART_CH_ERR;
  }

  uint32_t max_baudrate = 460800; // TODO_L: この値以上は実機で試験していないだけで本当は大丈夫な可能性もあるので要確認
  if (my_uart->baudrate <= 0 || my_uart->baudrate > max_baudrate)
  {
    return (int)UART_ERR_CODE::UART_BAUDRATE_ERR;
  }

  // TODO_L: Add stop bit setting ?

  HardwareSerial* my_serial_port;
  my_serial_port = UART_CHIPKIT_select_serial_port_(my_uart->ch);
  if (my_serial_port == NULL) return (int)UART_ERR_CODE::UART_CH_ERR;

  my_serial_port->begin(my_uart->baudrate);

  // SetTimeout function
  if (my_uart->ch == PORT_CH_UART_SAGITTA) // TODO_L: my_uart_vの中でタイムアウト時間を持つべき
  {
    my_serial_port->setTimeout(2); // ms
  }

  return (int)UART_ERR_CODE::UART_OK;
}

int UART_CHIPKIT_rx(void* my_uart_v, void* data_v, int buffer_size)
{
  UART_Config* my_uart = (UART_Config*)my_uart_v;
  unsigned char* data = (unsigned char*)data_v;
  unsigned int ui_size = 0;
  int i_ret = 0;
  HardwareSerial* my_serial_port;

  if (buffer_size < 0) return (int)UART_ERR_CODE::UART_DATA_NEGA_ERR;

  my_serial_port = UART_CHIPKIT_select_serial_port_(my_uart->ch);
  if (my_serial_port == NULL) return (int)UART_ERR_CODE::UART_CH_ERR;

  const unsigned int length = my_serial_port->available();

  if (my_uart->ch == PORT_CH_UART_SAGITTA)
  {
    ui_size = (unsigned int)buffer_size;
  }
  else if (length > (unsigned int)buffer_size)
  {
    ui_size = (unsigned int)buffer_size;
  }
  else
  {
    ui_size = length;
  }

  if (length > 0)
  {
    i_ret = my_serial_port->readBytes(data, ui_size);
  }
  else
  {
    return 0;
  }

  return i_ret;
}

int UART_CHIPKIT_tx(void* my_uart_v, void* data_v, int data_size)
{
  UART_Config* my_uart = (UART_Config*)my_uart_v;
  unsigned char* data = (unsigned char*)data_v;

  HardwareSerial* my_serial_port;
  my_serial_port = UART_CHIPKIT_select_serial_port_(my_uart->ch);
  if (my_serial_port == NULL) return (int)UART_ERR_CODE::UART_CH_ERR;

  my_serial_port->write(data, data_size);

  return (int)UART_ERR_CODE::UART_OK;
}

int UART_CHIPKIT_reopen(void* my_uart_v, int reason)
{
  UART_Config* my_uart = (UART_Config*)my_uart_v;

  HardwareSerial* my_serial_port;
  my_serial_port = UART_CHIPKIT_select_serial_port_(my_uart->ch);
  if (my_serial_port == NULL) return (int)UART_ERR_CODE::UART_CH_ERR;

  my_serial_port->end();
  my_serial_port->begin(my_uart->baudrate);

  if (my_uart->ch == PORT_CH_UART_SAGITTA)
  {
    my_serial_port->setTimeout(2); // ms
  }

  return (int)UART_ERR_CODE::UART_OK;
}

// Private functions
static HardwareSerial* UART_CHIPKIT_select_serial_port_(const unsigned char ch)
{
  HardwareSerial* ret_serial = NULL;

  switch (ch)
  {
  case 0:
    ret_serial = &Serial;
    break;
  case 1:
    ret_serial = &Serial1;
    break;
  case 2:
    ret_serial = &Serial2;
    break;
  case 3:
    ret_serial = &Serial3;
    break;
  case 4:
    ret_serial = &Serial4;
    break;
  case 5:
    ret_serial = &Serial5;
    break;
  default:
    break;
  }

  return ret_serial;
}
