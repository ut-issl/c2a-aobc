/**
 * @file
 * @brief 温度異常検知アプリ
 */

#ifndef TEMPERATURE_ANOMALY_H_
#define TEMPERATURE_ANOMALY_H_

#include <src_core/System/ApplicationManager/app_info.h>
#include <src_core/TlmCmd/common_cmd_packet.h>

/**
 * @enum
 * @brief 温度異常のローカルID
 * @note  uint8_t を想定, EL, EH での使用を想定
 */
typedef enum
{
  APP_TEMPERATURE_ANOMALY_IDX_PIC,          //!< PIC温度計
  APP_TEMPERATURE_ANOMALY_IDX_RW_DCDC,      //!< RW DCDC温度計
  APP_TEMPERATURE_ANOMALY_IDX_ROUGH_GYRO,   //!< 粗ジャイロセンサ内蔵温度計
  APP_TEMPERATURE_ANOMALY_IDX_FINE_GYRO_X,  //!< 精ジャイロセンサ内蔵温度計X軸
  APP_TEMPERATURE_ANOMALY_IDX_FINE_GYRO_Y,  //!< 精ジャイロセンサ内蔵温度計Y軸
  APP_TEMPERATURE_ANOMALY_IDX_FINE_GYRO_Z,  //!< 精ジャイロセンサ内蔵温度計Z軸
  APP_TEMPERATURE_ANOMALY_IDX_STT_MCU,      //!< STT内蔵温度計MCU
  APP_TEMPERATURE_ANOMALY_IDX_STT_FPGA,     //!< STT内蔵温度計FPGA
  APP_TEMPERATURE_ANOMALY_IDX_RW_X,         //!< RW X軸内蔵温度計
  APP_TEMPERATURE_ANOMALY_IDX_RW_Y,         //!< RW Y軸内蔵温度計
  APP_TEMPERATURE_ANOMALY_IDX_RW_Z,         //!< RW Z軸内蔵温度計
  APP_TEMPERATURE_ANOMALY_IDX_GPSR,         //!< GPS-R内蔵温度計
  APP_TEMPERATURE_ANOMALY_IDX_MAX,
} APP_TEMPERATURE_ANOMALY_IDX;

/**
 * @struct APP_TEMPERATURE_ANOMALY_TemperatureThreshold
 * @brief 温度異常検知用閾値
 */
typedef struct
{
  float upper_degC;   //!< 上限温度
  float lower_degC;   //!< 下限温度
} APP_TEMPERATURE_ANOMALY_TemperatureThreshold;

/**
 * @struct TemperatureAnomaly
 * @brief 温度異常検知用テレメ
 */
typedef struct
{
  APP_TEMPERATURE_ANOMALY_TemperatureThreshold threshold_list[APP_TEMPERATURE_ANOMALY_IDX_MAX]; //!< 閾値リスト
} TemperatureAnomaly;

extern const TemperatureAnomaly* const temperature_anomaly;

AppInfo APP_TEMPERATURE_ANOMALY_create_app(void);

CCP_EXEC_STS Cmd_APP_TEMPERATURE_ANOMALY_SET_THRESHOLD(const CommonCmdPacket* packet);

#endif
