/**
* @file   thermo_sensor.h
* @brief  PICに接続された温度計を読み取るアプリ
*/
#ifndef THERMO_SENSOR_H_
#define THERMO_SENSOR_H_

#include <src_core/System/ApplicationManager/app_info.h>

/**
 * @enum   APP_TSNS_IDX
 * @brief  AOBC基板搭載温度計のインデックス定義
 * @note   uint8_tを想定
 */
typedef enum
{
  APP_TSNS_IDX_PIC = 0,   //!< PIC付近の温度計
  APP_TSNS_IDX_RW_DCDC,   //!< RW DCDCコンバータ付近の温度計
  APP_TSNS_IDX_MAX,
} APP_TSNS_IDX;

/**
 * @struct ThermoSensor
 * @brief PICに直接接続された温度計の外部公開情報用構造体
 */
typedef struct
{
  float temperature_degC[APP_TSNS_IDX_MAX]; //!< 計測温度 [degC]
  float reference_V;                        //!< AD変換器参照電圧 [V]
} ThermoSensor;

extern const ThermoSensor* const thermo_sensor;

AppInfo APP_TSNS_create_app(void);

#endif
