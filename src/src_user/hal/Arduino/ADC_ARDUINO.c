#include "./CHIPKIT/ADC_CHIPKIT.h"

int ADC_initialize(unsigned int init_param)
{
  return ADC_CHIPKIT_initialize(init_param);
}

int ADC_get_value(uint8_t ch)
{
  return ADC_CHIPKIT_get_value(ch);
}
