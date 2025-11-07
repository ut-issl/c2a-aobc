/**
* @file   sun_sensor_selector.h
* @brief  複数あるサンセンサ出力を組み合わせて一つの太陽方向ベクトルを計算するアプリ
*/
#ifndef SUN_SENSOR_SELECTOR_H_
#define SUN_SENSOR_SELECTOR_H_

#include <stdint.h>
#include <src_core/System/ApplicationManager/app_info.h>
#include <src_core/TlmCmd/common_cmd_packet.h>

typedef struct
{
  float sun_intensity_upper_threshold_percent; //!< 各サンセンサが利用可能となる太陽強度の最大値 [%]
  float sun_intensity_lower_threshold_percent; //!< 各サンセンサが利用可能となる太陽強度の最小値 [%]
} SunSensorSelector;

/**
 * @enum   APP_SS_SELECTOR_AVAILABLE
 * @brief  個々のサンセンサが利用可能かどうかの状態を表す
 * @detail 利用可能であるというのは，十分な受光量があり，かつチェックサムが正しいということを指す
 * @note   uint8_tを想定
 */
typedef enum
{
  APP_SS_SELECTOR_AVAILABLE_OK = 0,
  APP_SS_SELECTOR_AVAILABLE_NG,
} APP_SS_SELECTOR_AVAILABLE;

extern const SunSensorSelector* const sun_sensor_selector;

AppInfo APP_SS_SELECTOR_create_app(void);

/*
* @fn     Cmd_SUN_SENSOR_SELECTOR_SET_INTENSITY_THRESHOLD
* @brief  サンセンサが利用可能となる閾値の設定
* @param  CommonCmdPacket
* @return CCP_EXEC_STSに準拠
*/
CCP_CmdRet Cmd_SUN_SENSOR_SELECTOR_SET_INTENSITY_THRESHOLD(const CommonCmdPacket* packet);

#endif
