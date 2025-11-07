/**
 * @file
 * @brief Arduino用I2C Interface Wrapper
 * @note I2C_CHIPKITをそのまま呼び出しているだけで，関数の引数定義などはI2C.hに従う
 */

#include "./CHIPKIT/I2C_CHIPKIT.h"

int I2C_init(void* my_i2c_v)
{
  I2C_Config* my_i2c = (I2C_Config*)my_i2c_v;
  int i_ret = 0;

  i_ret = I2C_CHIPKIT_init(my_i2c);

  return i_ret;
}

int I2C_rx(void* my_i2c_v, void* data_v, int buffer_size)
{
  I2C_Config* my_i2c = (I2C_Config*)my_i2c_v;
  int i_ret = 0;

  i_ret = I2C_CHIPKIT_rx(my_i2c, data_v, buffer_size);
  return i_ret;
}

int I2C_tx(void* my_i2c_v, void* data_v, int data_size)
{
  I2C_Config* my_i2c = (I2C_Config*)my_i2c_v;
  int i_ret = 0;

  i_ret = I2C_CHIPKIT_tx(my_i2c, data_v, data_size);

  return i_ret;
}

int I2C_reopen(void* my_i2c_v, int reason)
{
  I2C_Config* my_i2c = (I2C_Config*)my_i2c_v;
  int i_ret = 0;

  i_ret = I2C_CHIPKIT_reopen(my_i2c);

  if (reason != 0)
  {
  }

  return i_ret;
}

void I2C_set_stop_flag(void* my_i2c_v, const uint8_t stop_flag)
{
  I2C_Config* my_i2c = (I2C_Config*)my_i2c_v;

  if (stop_flag >= 1)
  {
    my_i2c->stop_flag = 1;
  }
  else
  {
    my_i2c->stop_flag = 0;
  }
  return;
}

void I2C_set_rx_length(void* my_i2c_v, const uint32_t rx_length)
{
  I2C_Config* my_i2c = (I2C_Config*)my_i2c_v;

  my_i2c->rx_length = rx_length;
  return;
}
