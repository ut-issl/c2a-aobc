#pragma section REPRO
/**
* @file   thermo_sensor.c
* @brief  PICに接続された温度計を読み取るアプリ
*/
#include "thermo_sensor.h"

#include <stdint.h>
#include <src_user/IfWrapper/ADC.h>
#include <src_user/Settings/port_config.h>
#include <src_user/Library/physical_constants.h>


static ThermoSensor        thermo_sensor_;
const  ThermoSensor* const thermo_sensor = &thermo_sensor_;

static uint8_t APP_TSNS_adc_port_list_[APP_TSNS_IDX_MAX];

static void  APP_TSNS_init_(void);
static void  APP_TSNS_exec_(void);
static float APP_TSNS_convert_to_degC_(const int adc_raw);

AppInfo APP_TSNS_create_app(void)
{
  return AI_create_app_info("ThermoSensor", APP_TSNS_init_, APP_TSNS_exec_);
}

static void APP_TSNS_init_(void)
{
  thermo_sensor_.reference_V = 0.0f;
  for (int i = 0; i < APP_TSNS_IDX_MAX; i++)
  {
    thermo_sensor_.temperature_degC[i] = 0.0f;
  }
  APP_TSNS_adc_port_list_[APP_TSNS_IDX_PIC]     = PORT_CH_ADC_TEMPERATURE_PIC;
  APP_TSNS_adc_port_list_[APP_TSNS_IDX_RW_DCDC] = PORT_CH_ADC_TEMPERATURE_DCDC;

  ADC_initialize(0);

  return;
}

static void APP_TSNS_exec_(void)
{
  thermo_sensor_.reference_V = (float)ADC_get_value(PORT_CH_ADC_REFERENCE); // TODO_L 電圧への変換式を調べる

  for (int i = 0; i < APP_TSNS_IDX_MAX; i++)
  {
    int temp_raw = ADC_get_value(APP_TSNS_adc_port_list_[i]);
    thermo_sensor_.temperature_degC[i] = APP_TSNS_convert_to_degC_(temp_raw);
  }

  return;
}

static float APP_TSNS_convert_to_degC_(const int adc_raw)
{
  const float kRawToKelvin = 0.5f;
  return PHYSICAL_CONST_kelvin_to_degC((float)adc_raw * kRawToKelvin);
}

#pragma section
