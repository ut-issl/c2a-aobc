/**
 * @file
 * @brief SILS用I2C Interface Wrapper
 * @note
 */

#include <src_core/IfWrapper/i2c.h>
#include <src_user/Settings/port_config.h>

int OBC_C2A_I2cWriteCommand (int port_id, const unsigned char i2c_addr, const unsigned char* data, const unsigned char len);
int OBC_C2A_I2cWriteRegister(int port_id, const unsigned char i2c_addr, const unsigned char* data, const unsigned char len);
int OBC_C2A_I2cReadRegister (int port_id, const unsigned char i2c_addr, unsigned char* data, const unsigned char len);

int I2C_init(void* my_i2c_v)
{
  I2C_Config* my_i2c = (I2C_Config*)my_i2c_v;
  int i_ret = 0;

  // TODO_L: 実装する

  return i_ret;
}

int I2C_rx(void* my_i2c_v, void* data_v, int buffer_size)
{
  I2C_Config* my_i2c = (I2C_Config*)my_i2c_v;
  int i_ret = 0;
  unsigned char* data = (unsigned char*)data_v;

  OBC_C2A_I2cReadRegister (my_i2c->ch, my_i2c->device_address, data, my_i2c->rx_length);
  i_ret = my_i2c->rx_length;

  return i_ret;
}

int I2C_tx(void* my_i2c_v, void* data_v, int data_size)
{
  I2C_Config* my_i2c = (I2C_Config*)my_i2c_v;
  int i_ret = 0;
  unsigned char* data = (unsigned char*)data_v;

  OBC_C2A_I2cWriteCommand(my_i2c->ch, my_i2c->device_address, data, data_size);

  // RW0003専用特殊処理 FIXME: 別の場所に置き換える->S2E側の大きな改修が必要なので少し後回し
  if (my_i2c->ch == 1)
  {
    if (my_i2c->device_address == I2C_DEVICE_ADDR_RW_X ||
        my_i2c->device_address == I2C_DEVICE_ADDR_RW_Y ||
        my_i2c->device_address == I2C_DEVICE_ADDR_RW_Z)
    {
      if (my_i2c->stop_flag == 0)
      {
        uint8_t ReadRegAddress = data[2];
        OBC_C2A_I2cWriteCommand(my_i2c->ch, my_i2c->device_address, &ReadRegAddress, 1);
      }
    }
  }

  return i_ret;
}

int I2C_reopen(void* my_i2c_v, int reason)
{
  I2C_Config* my_i2c = (I2C_Config*)my_i2c_v;
  int i_ret = 0;

  // TODO_L: 実装する

  return i_ret;
}

void I2C_set_stop_flag(void* my_i2c_v, const unsigned char stop_flag)
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
