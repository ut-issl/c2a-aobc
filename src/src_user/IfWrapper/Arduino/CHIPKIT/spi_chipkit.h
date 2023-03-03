#ifndef SPI_CHIPKIT_H_
#define SPI_CHIPKIT_H_

#include <src_core/IfWrapper/spi.h>

#ifdef __cplusplus
extern "C" {
#endif

  // 引数・返値の定義はspi.hに従う
  int SPI_CHIPKIT_init(void* my_spi_v);
  int SPI_CHIPKIT_rx(void* my_spi_v, void* data_v, int buffer_size);
  int SPI_CHIPKIT_tx(void* my_spi_v, void* data_v, int data_size);
  int SPI_CHIPKIT_reopen(void* my_spi_v);

#ifdef __cplusplus
};
#endif

#endif
