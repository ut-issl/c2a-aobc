/**
* @file   oem7600_filter.h
* @brief  OEM7600出力フィルタ
*/
#ifndef OEM7600_FILTER_H_
#define OEM7600_FILTER_H_

#include <stdint.h>
#include <src_core/System/ApplicationManager/app_info.h>
#include <src_core/TlmCmd/common_cmd_packet.h>
#include <src_user/Library/physical_constants.h>
#include <src_user/Library/SignalProcess/spike_filter.h>
#include <src_user/Library/TimeSystem/gps_time.h>

/**
 * @struct Oem7600Filter
 * @brief OEM7600出力フィルタに関する外部公開情報用構造体
 */
typedef struct
{
  SpikeFilter_Config position_spike_filter_config[PHYSICAL_CONST_THREE_DIM];  //!< スパイク除去フィルタ設定パラメータ for 衛星位置
  SpikeFilter_Config velocity_spike_filter_config[PHYSICAL_CONST_THREE_DIM];  //!< スパイク除去フィルタ設定パラメータ for 衛星速度
  SpikeFilter_Config total_gps_time_spike_filter_config;                  //!< スパイク除去フィルタ設定パラメータ for 受信機時刻通算秒


  double sat_pos_filtered_ecef_m[PHYSICAL_CONST_THREE_DIM];          //!< フィルタリング後のECEF座標系での衛星位置ベクトル [m]
  double sat_vel_filtered_ecef_m_s[PHYSICAL_CONST_THREE_DIM];        //!< フィルタリング後のECEF座標系での衛星速度ベクトル [m/s]
  double sat_pos_filtered_eci_m[PHYSICAL_CONST_THREE_DIM];           //!< フィルタリング後のECI座標系での衛星位置ベクトル [m]
  double sat_vel_filtered_eci_m_s[PHYSICAL_CONST_THREE_DIM];         //!< フィルタリング後のECI座標系での衛星速度ベクトル [m/s]
  GpsTime gps_time_filtered;                                         //!< フィルタリング後の受信機時刻
  ObcTime obct_gps_time_observed_filtered;                           //!< フィルタリング後の受信機時刻取得時OBCT

  C2A_MATH_ERROR position_filter_error[PHYSICAL_CONST_THREE_DIM];    //!< スパイク除去フィルタ入力判定ログ for 衛星位置
  C2A_MATH_ERROR velocity_filter_error[PHYSICAL_CONST_THREE_DIM];    //!< スパイク除去フィルタ入力判定ログ for 衛星速度
  C2A_MATH_ERROR total_gps_time_filter_error;                    //!< スパイク除去フィルタ入力判定ログ for 受信機時刻通算秒

  uint16_t total_gps_time_epoch_week;                            //!< 受信機時刻通算秒の元紀とするGPS週番号
} Oem7600Filter;

extern const Oem7600Filter* const oem7600_filter;

AppInfo APP_OEM7600_FILTER_create_app(void);

CCP_CmdRet Cmd_APP_OEM7600_FILTER_SET_SPIKE_FILTER_PARAM(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_APP_OEM7600_FILTER_RESET_SPIKE_FILTER(const CommonCmdPacket* packet);

#endif
