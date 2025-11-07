#ifndef GPIO_CHIPKIT_H_
#define GPIO_CHIPKIT_H_

#include <src_user/IfWrapper/GPIO.h>

#ifdef __cplusplus
extern "C" {
#endif

  // 引数・返値の定義はGPIO.hに従う
  int GPIO_CHIPKIT_set_direction(const uint8_t port, const GPIO_DIRECTION direction);
  int GPIO_CHIPKIT_set_output(const uint8_t port, const GPIO_HL direction);
  int GPIO_CHIPKIT_read_input(const uint8_t port);

#ifdef __cplusplus
};
#endif

#endif
