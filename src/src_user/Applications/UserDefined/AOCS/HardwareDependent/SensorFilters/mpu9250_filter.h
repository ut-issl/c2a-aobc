/**
* @file   mpu9250_filter.h
* @brief  MPU9250ジャイロ、磁気センサ出力フィルタ
*/
#ifndef MPU9250_FILTER_H_
#define MPU9250_FILTER_H_

#include <src_core/System/ApplicationManager/app_info.h>
#include <src_core/TlmCmd/common_cmd_packet.h>
#include "../../../../../Library/stdint.h"
#include "../../../../../Library/physical_constants.h"
#include "../../../../../Library/SignalProcess/z_filter.h"
#include "../../../../../Library/SignalProcess/spike_filter.h"

/**
 * @enum   MPU9250_FILTER_SENSOR_ID
 * @brief  CMDによるパラメータ設定対象 (磁気センサ or Gyro) の識別に用いる
 * @note   uint8_tを想定
 */
typedef enum
{
  MPU9250_FILTER_SENSOR_ID_MAG = 0,
  MPU9250_FILTER_SENSOR_ID_GYRO,
  MPU9250_FILTER_SENSOR_ID_MAX
} MPU9250_FILTER_SENSOR_ID;

/**
 * @struct Mpu9250Filter
 * @brief MPU9250フィルタに関する外部公開情報用構造体
 */
typedef struct
{
  // ZFilterの設定パラメータ
  float sampling_freq_Hz;     //!< zfilter (1次LPFとしての使用を想定) 用サンプリング周波数 [Hz] (アプリ実行周期に合わせる)
  float cut_off_freq_lpf_1st_mag_Hz[PHYSICAL_CONST_THREE_DIM];   //!< 磁気センサ用1次LPF用カットオフ周波数 [Hz]
  float cut_off_freq_lpf_1st_gyro_Hz[PHYSICAL_CONST_THREE_DIM];  //!< Gyroセンサ用1次LPF用カットオフ周波数 [Hz]

  SpikeFilter_Config spike_filter_config_mag[PHYSICAL_CONST_THREE_DIM];  //!< 磁気センサ用スパイク除去フィルタ設定パラメータ
  SpikeFilter_Config spike_filter_config_gyro[PHYSICAL_CONST_THREE_DIM]; //!< Gyroセンサ用スパイク除去フィルタ設定パラメータ

  float ang_vel_est_body_rad_s[PHYSICAL_CONST_THREE_DIM];           //!< フィルタリング後の機体固定座標推定角速度 [rad/s]
  float mag_est_body_nT[PHYSICAL_CONST_THREE_DIM];                  //!< フィルタリング後の機体固定座標推定磁場ベクトル [nT]

  C2A_MATH_ERROR filter_mag_error[PHYSICAL_CONST_THREE_DIM];        //!< 磁気センサ用スパイク除去フィルタ入力判定ログ
  C2A_MATH_ERROR filter_gyro_error[PHYSICAL_CONST_THREE_DIM];       //!< Gyroセンサ用スパイク除去フィルタ入力判定ログ
} Mpu9250Filter;

extern const Mpu9250Filter* const mpu9250_filter;

AppInfo APP_MPU9250_FILTER_create_app(void);

CCP_CmdRet Cmd_APP_MPU9250_FILTER_SET_ZFILTER_PARAM(const CommonCmdPacket* packet);

CCP_CmdRet Cmd_APP_MPU9250_FILTER_SET_SPIKE_FILTER_PARAM(const CommonCmdPacket* packet);

CCP_CmdRet Cmd_APP_MPU9250_FILTER_RESET_ZFILTER(const CommonCmdPacket* packet);

CCP_CmdRet Cmd_APP_MPU9250_FILTER_RESET_SPIKE_FILTER(const CommonCmdPacket* packet);

#endif
