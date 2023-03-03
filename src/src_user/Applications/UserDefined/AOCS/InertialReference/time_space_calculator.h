/**
* @file   time_space_calculator.h
* @brief  各種慣性系リファレンス計算のうち，時刻と座標の変換を行う
*/
#ifndef TIME_SPACE_CALCULATOR_H_
#define TIME_SPACE_CALCULATOR_H_

#include <src_core/System/ApplicationManager/app_info.h>
#include <src_core/TlmCmd/common_cmd_packet.h>

/**
 * @struct TimeSpaceCalculator
 * @brief  計算時刻オフセットの設定値など
 */
typedef struct
{
  float offset_sec; //!< 計算結果を使用する時刻と実行時刻の間のオフセット[sec]
} TimeSpaceCalculator;

extern const TimeSpaceCalculator* const time_space_calculator;

AppInfo APP_TIME_SPACE_CALC_create_app(void);

CCP_EXEC_STS Cmd_APP_TIME_SPACE_CALC_SET_OFFSET_TIME(const CommonCmdPacket* packet);

#endif
