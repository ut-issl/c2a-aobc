/**
 * @file
 * @brief Arduino chipKITライブラリ内のC++で書かれたanalogRead(ADC)をCで使えるようにするためのラッパ
 */

#include "./ADC_CHIPKIT.h"
#include <src_user/Settings/port_config.h>
#include <Arduino.h>

int ADC_CHIPKIT_initialize(unsigned int init_param)
{
  analogReference(EXTERNAL);

  return 0;
}

int ADC_CHIPKIT_get_value(uint8_t ch)
{
  if (ch >= PORT_CH_MAX_ADC_PORT) return 0;
  if (ch <= PORT_CH_MIN_ADC_PORT) return 0;

  return analogRead(ch);
}
