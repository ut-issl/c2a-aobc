/**
 * @file
 * @brief Arduino chipKITライブラリ内のC++で書かれたDSPI(SPI)をCで使えるようにするためのラッパ
 * @note Ref: http://ww1.microchip.com/downloads/en/DeviceDoc/61106G.pdf
 */

#include "./spi_chipkit.h"
#include "../../../Settings/port_config.h"

extern "C"
{
#include <src_core/Library/print.h>
#include "../../GPIO.h"
}

#include <DSPI.h>

// DSPIのインスタンス化
static DSPI0 dspi0_;
static DSPI1 dspi1_;
static DSPI2 dspi2_;
static DSPI3 dspi3_;

const static uint16_t SPI_CHIPKIT_kMaxDataSize_ = 65535;  //!< 仕様書で決まっている最大データサイズ

// Private functions
/**
 * @brief SPIポートを選択する
 * @param[in] comm_ch SPIポート番号 PICの制約から0から4まで
 * @return chに該当するDSPIのポインタが返ってくる。ch>4ではNULLが返ってくるので適切な対処が必要
 */
static DSPI* SPI_CHIPKIT_select_port_(const uint8_t comm_ch);

// Global functions
int SPI_CHIPKIT_init(void* my_spi_v)
{
  SPI_Config* my_spi = (SPI_Config*)my_spi_v;
  int gpio_ret;
  DSPI* my_spi_port;

  if ((my_spi->comm_ch < 0) || (my_spi->comm_ch >= PORT_CH_MAX_SPI_PORT))
  {
    return (int)SPI_ERR_CODE::SPI_ERR_CH;
  }

  // Select SPI Port
  my_spi_port = SPI_CHIPKIT_select_port_(my_spi->comm_ch);
  if (my_spi_port == NULL) return (int)SPI_ERR_CODE::SPI_ERR_CH;

  // Set frequency
  if (my_spi->frequency_khz == 0)
  {
    return (int)SPI_ERR_CODE::SPI_ERR_FREQUENCY;
  }
  else
  {
    uint32_t frequency_hz = (uint32_t)my_spi->frequency_khz * 1000;
    my_spi_port->setSpeed(frequency_hz);
  }

  // Set GPIO Chip Select Port
  gpio_ret = GPIO_set_direction(my_spi->gpio_ch, GPIO_OUTPUT);
  if (gpio_ret != (int)GPIO_OK) return (int)SPI_ERR_CODE::SPI_ERR_GPIO;
  gpio_ret = GPIO_set_output(my_spi->gpio_ch, GPIO_HIGH);
  if (gpio_ret != (int)GPIO_OK) return (int)SPI_ERR_CODE::SPI_ERR_GPIO;

  // Set SPI port
  my_spi_port->begin();

  // Set Mode
  switch (my_spi->mode)
  {
  case SPI_MODE::SPI_MODE_0:
    my_spi_port->setMode(DSPI_MODE0);
    break;
  case SPI_MODE::SPI_MODE_1:
    my_spi_port->setMode(DSPI_MODE1);
    break;
  case SPI_MODE::SPI_MODE_2:
    my_spi_port->setMode(DSPI_MODE2);
    break;
  case SPI_MODE::SPI_MODE_3:
    my_spi_port->setMode(DSPI_MODE3);
    break;
  default:
    // Not reached
    break;
  }

  return (int)SPI_ERR_CODE::SPI_ERR_OK;
}

int SPI_CHIPKIT_rx(void* my_spi_v, void* data_v, int buffer_size)
{
  DSPI* my_spi_port;
  SPI_Config* my_spi = (SPI_Config*)my_spi_v;
  unsigned char* data = (unsigned char*)data_v;

  // RX size check
  if (buffer_size < 0)
  {
    return (int)SPI_ERR_CODE::SPI_ERR_DATA_NEGA;
  }
  else if ((uint32_t)buffer_size < my_spi->rx_length)
  {
    return (int)SPI_ERR_CODE::SPI_ERR_RX_BUFFER_SMALL;
  }
  else
  {
    // 下記を実行
  }

  // Select port
  my_spi_port = SPI_CHIPKIT_select_port_(my_spi->comm_ch);
  if (my_spi_port == NULL) return (int)SPI_ERR_CODE::SPI_ERR_CH;

  // Start Tx
  if (my_spi->cs_state_after_tx == SPI_CS_STATE_AFTER_TX_HIGH)
  {
    my_spi_port->setPinSelect(my_spi->gpio_ch);
    my_spi_port->setSelect(LOW);
  }
  else
  {
    // 何もせずに読み出し動作を継続
  }
  my_spi_port->transfer(my_spi->rx_length, my_spi->tx_data_for_rx, data);

  // End Tx
  my_spi_port->setSelect(HIGH);

  return (int)my_spi->rx_length;
}

int SPI_CHIPKIT_tx(void* my_spi_v, void* data_v, int data_size)
{
  DSPI* my_spi_port;
  SPI_Config* my_spi = (SPI_Config*)my_spi_v;
  unsigned char* data = (unsigned char*)data_v;
  uint16_t ui16_size = 0;

  if (data_size < 0) return (int)SPI_ERR_CODE::SPI_ERR_DATA_NEGA;
  if (data_size > SPI_CHIPKIT_kMaxDataSize_) return (int)SPI_ERR_CODE::SPI_ERR_DATA_LARGE;
  ui16_size = (uint16_t)data_size;

  // Select port
  my_spi_port = SPI_CHIPKIT_select_port_(my_spi->comm_ch);
  if (my_spi_port == NULL) return (int)SPI_ERR_CODE::SPI_ERR_CH;

  // Start Tx
  my_spi_port->setPinSelect(my_spi->gpio_ch);
  my_spi_port->setSelect(LOW);

  my_spi_port->transfer(ui16_size, data);

  // End Tx
  if (my_spi->cs_state_after_tx == SPI_CS_STATE_AFTER_TX_HIGH)
  {
    my_spi_port->setSelect(HIGH);
  }
  else
  {
    // 何も実行しない
  }
  return (int)SPI_ERR_CODE::SPI_ERR_OK;
}

int SPI_CHIPKIT_reopen(void* my_spi_v)
{
  DSPI* my_spi_port;
  SPI_Config* my_spi = (SPI_Config*)my_spi_v;

  // SelectPort
  my_spi_port = SPI_CHIPKIT_select_port_(my_spi->comm_ch);
  if (my_spi_port == NULL) return (int)SPI_ERR_CODE::SPI_ERR_CH;

  // Reopen
  my_spi_port->end();
  SPI_CHIPKIT_init(my_spi_v);

  return (int)SPI_ERR_CODE::SPI_ERR_OK;
}

// Private functions
static DSPI* SPI_CHIPKIT_select_port_(const uint8_t comm_ch)
{
  DSPI* ret_spi = NULL;

  switch (comm_ch)
  {
  case 0:
    ret_spi = &dspi0_;
    break;
  case 1:
    ret_spi = &dspi1_;
    break;
  case 2:
    ret_spi = &dspi2_;
    break;
  case 3:
    ret_spi = &dspi3_;
    break;
  default:
    break;
  }

  return ret_spi;
}
