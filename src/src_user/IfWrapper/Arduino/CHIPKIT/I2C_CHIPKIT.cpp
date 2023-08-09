/**
 * @file
 * @brief Arduino chipKITライブラリ内のC++で書かれたDTWI(I2C)をCで使えるようにするためのラッパ
 */

#include "./I2C_CHIPKIT.h"
#include <src_user/Settings/port_config.h>
extern "C"
{
#include <src_core/Library/print.h>
}

#include <DTWI.h>

// DTWIのインスタンス化
static DTWI0 dtwi0_;
static DTWI1 dtwi1_;
static DTWI2 dtwi2_;
static DTWI3 dtwi3_;
static DTWI4 dtwi4_;

// Private functions
/**
 * @brief I2Cポートを選択する
 * @param[in] ch I2Cポート番号 PICの制約から0から4まで
 * @return chに該当するDTWIのポインタが返ってくる。ch>5ではNULLが返ってくるので適切な対処が必要
 */
static DTWI* I2C_CHIPKIT_select_port_(const uint8_t ch);

/**
 * @brief I2CでMasterからDeviceへのデータ書き込みを始める
 * @param[in] i2c_port I2Cのポート
 * @param[in] dev_addr 書き込み先デバイスアドレス
 * @param[in] timeout_threshold タイムアウト閾値
 * @return I2C_ERR_CODEに対応するエラー
 */
static I2C_ERR_CODE I2C_CHIPKIT_begin_write_mode_(DTWI* i2c_port,
                                                  const uint8_t dev_addr,
                                                  const uint32_t timeout_threshold);

/**
 * @brief I2CでMasterからDeviceへのデータ読み出しを始める
 * @param[in] i2c_port I2Cのポート
 * @param[in] dev_addr 読み出し元デバイスアドレス
 * @param[in] buffer_size 読み出すデータのサイズ
 * @param[in] timeout_threshold タイムアウト閾値
 * @return I2C_ERR_CODEに対応するエラー
 */
static I2C_ERR_CODE I2C_CHIPKIT_begin_read_mode_(DTWI* i2c_port,
                                                 const uint8_t dev_addr,
                                                const uint32_t buffer_size,
                                                const uint32_t timeout_threshold);

/**
 * @brief I2C通信を停止する
 * @param[in] i2c_port I2Cのポート
 * @param[in] timeout_threshold タイムアウト閾値
 * @return I2C_ERR_CODEに対応するエラー
 */
static I2C_ERR_CODE I2C_CHIPKIT_stop_communication_(DTWI* i2c_port, const uint32_t timeout_threshold);

/**
 * @brief タイムアウトをチェックする
 * @param[in] timeout_counter ここで与えられたカウンタをこの関数内部でインクリメントし，時間超過を確認している
 * @param[in] timeout_threshold タイムアウト閾値
 * @return タイムアウトしたらfalseを返す
 */
static bool I2C_CHIPKIT_check_timeout_(uint32_t& timeout_counter, const uint32_t timeout_threshold);

// Global functions
int I2C_CHIPKIT_init(void* my_i2c_v)
{
  I2C_Config* my_i2c = (I2C_Config*)my_i2c_v;
  int i_ret = (int)I2C_ERR_CODE::I2C_OK;
  DTWI* my_i2c_port;
  DTWI::I2C_FREQ i2c_freq;

  if ((my_i2c->ch < 0) || (my_i2c->ch >= PORT_CH_MAX_I2C_PORT))
  {
    return (int)I2C_ERR_CODE::I2C_CH_ERR;
  }

  my_i2c_port = I2C_CHIPKIT_select_port_(my_i2c->ch);
  if (my_i2c_port == NULL) return (int)I2C_ERR_CODE::I2C_CH_ERR;

  // DTWIでは下の周波数[kHz]だけに対応

  switch (my_i2c->frequency_khz)
  {
  case 0:
    i2c_freq = DTWI::FQ0Hz;
    break;
  case 10:
    i2c_freq = DTWI::FQ10KHz;
    break;
  case 50:
    i2c_freq = DTWI::FQ50KHz;
    break;
  case 100:
    i2c_freq = DTWI::FQ100KHz;
    break;
  case 400:
    i2c_freq = DTWI::FQ400KHz;
    break;
  case 1000:
    i2c_freq = DTWI::FQ1MHz;
    break;
  default:
    i2c_freq = DTWI::FQ0Hz;
    i_ret = (int)I2C_ERR_CODE::I2C_FREQUENCY_ERR;
    break;
  }

  if (!my_i2c_port->beginMaster(i2c_freq)) return (int)I2C_ERR_CODE::I2C_ALREADY_ERR;

  return i_ret;
}

int I2C_CHIPKIT_rx(void* my_i2c_v, void* data_v, int buffer_size)
{
  I2C_Config* my_i2c = (I2C_Config*)my_i2c_v;
  unsigned char* data = (unsigned char*)data_v;
  int i_ret = (int)I2C_ERR_CODE::I2C_OK;
  DTWI* my_i2c_port;
  DTWI::I2C_STATUS status;
  uint32_t timeout_counter = 0;

  if (buffer_size < 0)
  {
    return (int)I2C_ERR_CODE::I2C_DATA_NEGA_ERR;
  }
  else if ((uint32_t)buffer_size < my_i2c->rx_length)
  {
    return (int)I2C_ERR_CODE::I2C_RX_BUFFER_SMALL;
  }
  else
  {
    // 下記を実行
  }
  // SelectPort
  my_i2c_port = I2C_CHIPKIT_select_port_(my_i2c->ch);
  if (my_i2c_port == NULL) return (int)I2C_ERR_CODE::I2C_CH_ERR;

  // Start read mode
  i_ret = (int)I2C_CHIPKIT_begin_read_mode_(my_i2c_port, my_i2c->device_address, my_i2c->rx_length, my_i2c->timeout_threshold);
  if (i_ret != (int)I2C_ERR_CODE::I2C_OK) return i_ret;

  // Wait untile data received
  do {
    status = my_i2c_port->getStatus();
    if (!I2C_CHIPKIT_check_timeout_(timeout_counter, my_i2c->timeout_threshold))
    {
      I2C_CHIPKIT_stop_communication_(my_i2c_port, my_i2c->timeout_threshold);
      return (int)I2C_ERR_CODE::I2C_TIMEOUT;
    }
  } while (status.fMyBus && !status.fNacking && status.fRead);

  // Stop I2C communication
  if (my_i2c->stop_flag != 0)
  {
    i_ret = (int)I2C_CHIPKIT_stop_communication_(my_i2c_port, my_i2c->timeout_threshold);
  }

  // Read received data from register
  i_ret = my_i2c_port->read(data, my_i2c->rx_length);

  return i_ret;
}

int I2C_CHIPKIT_tx(void* my_i2c_v, void* data_v, int data_size)
{
  I2C_Config* my_i2c = (I2C_Config*)my_i2c_v;
  unsigned char* data = (unsigned char*)data_v;
  uint32_t ui_size = 0;
  int i_ret = (int)I2C_ERR_CODE::I2C_OK;
  DTWI* my_i2c_port;

  if (data_size < 0) return (int)I2C_ERR_CODE::I2C_DATA_NEGA_ERR;
  ui_size = (uint32_t)data_size;

  // Select port
  my_i2c_port = I2C_CHIPKIT_select_port_(my_i2c->ch);
  if (my_i2c_port == NULL) return (int)I2C_ERR_CODE::I2C_CH_ERR;

  // Start write mode
  i_ret = (int)I2C_CHIPKIT_begin_write_mode_(my_i2c_port, my_i2c->device_address, my_i2c->timeout_threshold);
  if (i_ret != (int)I2C_ERR_CODE::I2C_OK) return i_ret;

  // Write data
  my_i2c_port->write(data, ui_size);  // TODO_L: check return from write function?

  uint32_t timeout_counter = 0;
  DTWI::I2C_STATUS status;
  do
  {
    status = my_i2c_port->getStatus();
    if (!I2C_CHIPKIT_check_timeout_(timeout_counter, my_i2c->timeout_threshold))
    {
      I2C_CHIPKIT_stop_communication_(my_i2c_port, my_i2c->timeout_threshold);
      return (int)I2C_ERR_CODE::I2C_TIMEOUT;
    }
  } while (!status.fWrite);

  // Stop I2C communication
  if (my_i2c->stop_flag != 0)
  {
#ifndef SILS_FW
    // 通信安定化のためRWのみstopbit前にdelay挿入 (効果を実験で確認済み)
    if (my_i2c->ch == PORT_CH_I2C_RWS)
    {
      delayMicroseconds(500);
    }
#endif
    i_ret = (int)I2C_CHIPKIT_stop_communication_(my_i2c_port, my_i2c->timeout_threshold);
  }

  return i_ret;
}

int I2C_CHIPKIT_reopen(void* my_i2c_v)
{
  I2C_Config* my_i2c = (I2C_Config*)my_i2c_v;
  int i_ret = (int)I2C_ERR_CODE::I2C_OK;
  DTWI* my_i2c_port;

  // SelectPort
  my_i2c_port = I2C_CHIPKIT_select_port_(my_i2c->ch);
  if (my_i2c_port == NULL) return (int)I2C_ERR_CODE::I2C_CH_ERR;

  // Reopen
  my_i2c_port->endMaster();
  i_ret = (int)I2C_CHIPKIT_init(my_i2c);

  return i_ret;
}

// Private functions
static DTWI* I2C_CHIPKIT_select_port_(const uint8_t ch)
{
  DTWI* ret_i2c = NULL;

  switch (ch)
  {
  case 0:
    ret_i2c = &dtwi0_;
    break;
  case 1:
    ret_i2c = &dtwi1_;
    break;
  case 2:
    ret_i2c = &dtwi2_;
    break;
  case 3:
    ret_i2c = &dtwi3_;
    break;
  case 4:
    ret_i2c = &dtwi4_;
    break;
  default:
    break;
  }

  return ret_i2c;
}

// Private functions
static I2C_ERR_CODE I2C_CHIPKIT_begin_write_mode_(DTWI* i2c_port, const uint8_t dev_addr, const uint32_t timeout_threshold)
{
  uint32_t timeout_counter = 0;
  while (!i2c_port->startMasterWrite(dev_addr))
  {
    if (!I2C_CHIPKIT_check_timeout_(timeout_counter, timeout_threshold))
    {
      I2C_CHIPKIT_stop_communication_(i2c_port, timeout_threshold);
      return I2C_ERR_CODE::I2C_TIMEOUT;
    }
  }
  return I2C_ERR_CODE::I2C_OK;
}

static I2C_ERR_CODE I2C_CHIPKIT_begin_read_mode_(DTWI* i2c_port, const uint8_t dev_addr, const uint32_t buffer_size, const uint32_t timeout_threshold)
{
  uint32_t timeout_counter = 0;
  while (!i2c_port->startMasterRead(dev_addr, buffer_size))
  {
    if (!I2C_CHIPKIT_check_timeout_(timeout_counter, timeout_threshold))
    {
      I2C_CHIPKIT_stop_communication_(i2c_port, timeout_threshold);
      return I2C_ERR_CODE::I2C_TIMEOUT;
    }
  }
  return I2C_ERR_CODE::I2C_OK;
}

static I2C_ERR_CODE I2C_CHIPKIT_stop_communication_(DTWI* i2c_port, const uint32_t timeout_threshold)
{
  uint32_t timeout_counter = 0;
  while (!i2c_port->stopMaster())
  {
    if (!I2C_CHIPKIT_check_timeout_(timeout_counter, timeout_threshold))
    {
      return I2C_ERR_CODE::I2C_TIMEOUT;
    }
  }
  return I2C_ERR_CODE::I2C_OK;
}

static bool I2C_CHIPKIT_check_timeout_(uint32_t& timeout_counter, const uint32_t timeout_threshold)
{
  timeout_counter++;
  if (timeout_counter > timeout_threshold)
  {
    // Printf("I2C Timeout \n");
    return false;
  }

  return true;
}
