#ifndef ADC_CHIPKIT_H_
#define ADC_CHIPKIT_H_

#include "../../ADC.h"

#ifdef __cplusplus
extern "C" {
#endif

  // 引数・返値の定義はADC.hに従う
  int ADC_CHIPKIT_initialize(unsigned int init_param);
  int ADC_CHIPKIT_get_value(uint8_t ch);

#ifdef __cplusplus
};
#endif

#endif
