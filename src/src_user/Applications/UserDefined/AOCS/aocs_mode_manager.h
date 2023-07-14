/**
 * @file
 * @brief  自動モード遷移を実行するアプリ
 */

#ifndef AOCS_MODE_MANAGER_H_
#define AOCS_MODE_MANAGER_H_

#include <stdint.h>
#include <src_core/System/ApplicationManager/app_info.h>
#include <src_core/TlmCmd/common_cmd_packet.h>

/**
 * @struct AocsModeManager
 * @brief  自動モード遷移に関する外部公開情報用構造体
 */
typedef struct
{
  // 全体共通
  uint8_t is_enabled_auto_mode_transition; //!< 自動モード遷移機能のEnableフラグ
  ObcTime previous_obc_time;               //!< 前回MMアプリが呼び出された時のOBC時刻
  uint8_t bdot_retry_count;                //!< Bdotに戻った回数の総数
  uint8_t is_control_error_small;          //!< 制御誤差が十分小さいことを示すフラグ

  // 収束判定
  float ang_vel_conv_limit_rad_s;   //!< 角速度収束判定しきい値 [rad/s]
  float ang_vel_conv_time_limit_s;  //!< 角速度収束判定時間閾値 [s]

  // Bdot異常判定
  float ang_vel_norm_increase_limit_rad_s;  //!< 角速度ノルム増加上限角速度 [rad/s]
  float ang_vel_anomaly_detection_period_s; //!< 角速度ノルム異常アノマリを判定する周期 [s]

  // 発散判定
  float sun_angle_div_limit_rad;     //!< 太陽捕捉発散判定しきい値 [rad]
  float sun_angle_div_time_limit_s;  //!< 太陽捕捉発散判定時間閾値 [s]
  float three_axis_div_limit_rad;    //!< 三軸姿勢発散判定しきい値 [rad]
  float three_axis_div_time_limit_s; //!< 三軸姿勢発散判定時間閾値 [s]

  // センサ異常判定
  float sun_invisible_time_limit_s;  //!< 太陽非可視時間閾値 [s]
  float stt_invisible_time_limit_s;  //!< STT非可視時間閾値 [s]

} AocsModeManager;

extern const AocsModeManager* const aocs_mode_manager;

AppInfo APP_AOCS_MM_initial_create_app(void);
AppInfo APP_AOCS_MM_bdot_create_app(void);
AppInfo APP_AOCS_MM_sun_pointing_create_app(void);
AppInfo APP_AOCS_MM_rough_three_axis_create_app(void);
AppInfo APP_AOCS_MM_fine_three_axis_create_app(void);

CCP_CmdRet Cmd_APP_AOCS_MM_SET_AUTO_MODE_TRANSITION(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_APP_AOCS_MM_SET_ANG_VEL_THRESHOLD(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_APP_AOCS_MM_SET_SUN_ANGLE_THRESHOLD(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_APP_AOCS_MM_SET_THREE_AXIS_THRESHOLD(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_APP_AOCS_MM_SET_INVISIBLE_THRESHOLD(const CommonCmdPacket* packet);

#endif
