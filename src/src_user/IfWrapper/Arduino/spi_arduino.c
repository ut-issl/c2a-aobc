/**
 * @file
 * @brief Arduino用SPI Interface Wrapper
 * @note spi_chipkitをそのまま呼び出しているだけで，関数の引数定義などはSPI.hに従う
 */

#include "./CHIPKIT/spi_chipkit.h"
#include <src_core/Library/print.h>

int SPI_init(void* my_spi_v)
{
  SPI_Config* my_spi = (SPI_Config*)my_spi_v;
  int ret = 0;

  ret = SPI_CHIPKIT_init(my_spi);
  return ret;
}

int SPI_rx(void* my_spi_v, void* data_v, int buffer_size)
{
  SPI_Config* my_spi = (SPI_Config*)my_spi_v;
  int ret = 0;

  ret = SPI_CHIPKIT_rx(my_spi, data_v, buffer_size);
  return ret;
}

int SPI_tx(void* my_spi_v, void* data_v, int data_size)
{
  SPI_Config* my_spi = (SPI_Config*)my_spi_v;
  int ret = 0;

  ret = SPI_CHIPKIT_tx(my_spi, data_v, data_size);
  return ret;
}

int SPI_reopen(void* my_spi_v, int reason)
{
  SPI_Config* my_spi = (SPI_Config*)my_spi_v;
  int ret = 0;

  ret = SPI_CHIPKIT_reopen(my_spi);

  if (reason != 0)
  {
  }

  return ret;
}

void SPI_set_rx_length(void* my_spi_v, const uint16_t rx_length)
{
  SPI_Config* my_spi = (SPI_Config*)my_spi_v;

  my_spi->rx_length = rx_length;

  return;
}

void SPI_set_cs_state_after_tx(void* my_spi_v, const SPI_CS_STATE_AFTER_TX cs_state_after_tx)
{
  SPI_Config* my_spi = (SPI_Config*)my_spi_v;

  my_spi->cs_state_after_tx = cs_state_after_tx;

  return;
}
