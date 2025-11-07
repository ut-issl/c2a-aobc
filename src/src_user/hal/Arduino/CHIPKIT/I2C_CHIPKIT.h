#ifndef I2C_CHIPKIT_H_
#define I2C_CHIPKIT_H_

#include <src_core/IfWrapper/i2c.h>

#ifdef __cplusplus
extern "C" {
#endif

  // 引数・返値の定義はI2C.hに従う
  int I2C_CHIPKIT_init(void* my_i2c_v);
  int I2C_CHIPKIT_rx(void* my_i2c_v, void* data_v, int buffer_size);
  int I2C_CHIPKIT_tx(void* my_i2c_v, void* data_v, int data_size);
  int I2C_CHIPKIT_reopen(void* my_i2c_v);

#ifdef __cplusplus
};

#endif

#endif
