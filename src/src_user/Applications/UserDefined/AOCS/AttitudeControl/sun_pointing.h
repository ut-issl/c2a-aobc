/**
* @file   sun_pointing.h
* @brief  太陽指向則制御アプリケーション
*/
#ifndef SUN_POINTING_H_
#define SUN_POINTING_H_

#include <stdint.h>
#include <src_core/System/ApplicationManager/app_info.h>
#include <src_core/TlmCmd/common_cmd_packet.h>
#include "../../../../Library/physical_constants.h"
#include "../../../../Library/ControlUtility/cross_product_control.h"
#include "../../../../Library/pid_control.h"
#include "../../../../Library/SignalProcess/z_filter.h"
#include "../../AOCS/aocs_error.h"

/**
 * @enum   SUN_POINTING_AXIS_ID
 * @brief  太陽指向させる軸の配列上のID
 * @note   uint8_tを想定
 * @note   配列アクセス先IDなので負値はひとまず使わない
 */
typedef enum
{
  SUN_POINTING_AXIS_ID_X = 0,
  SUN_POINTING_AXIS_ID_Y = 1,
  SUN_POINTING_AXIS_ID_Z = 2,
} SUN_POINTING_AXIS_ID;

/**
 * @enum   SUN_POINTING_CONTROL_STATE
 * @brief  太陽指向の達成状態
 * @note   uint8_tを想定
 * @note   TODO_L: 発散の定義は要検討．必要かどうかも要検討
 */
typedef enum
{
  SUN_POINTING_CONTROL_STATE_TRANSIENT = 0,  //!< 過渡状態
  SUN_POINTING_CONTROL_STATE_SPIN_UP   = 1,  //!< スピンアップ開始
  SUN_POINTING_CONTROL_STATE_CONVERGE  = 2,  //!< 収束近傍 (TBD)
  SUN_POINTING_CONTROL_STATE_DIVERSE   = 3,  //!< 発散 (TBD)
} SUN_POINTING_CONTROL_STATE;

/**
 * @struct SunPointing
 * @brief  太陽指向則制御に必要な情報を管理するための構造体
 */
typedef struct
{
  PidControl pid_omega[PHYSICAL_CONST_THREE_DIM];         //!< 姿勢角速度に関するPID制御インスタンス
  PidControl pid_att[PHYSICAL_CONST_THREE_DIM];           //!< 姿勢角度に関するPID制御インスタンス
  PidGains gains_omega_cmd_tmp[PHYSICAL_CONST_THREE_DIM]; //!< コマンド一時保存用角速度ゲイン
  PidGains gains_att_cmd_tmp[PHYSICAL_CONST_THREE_DIM];   //!<  コマンド一時保存用姿勢ゲイン
  CrossProductControl cross_product_cntrl;                //!< CrossProduct制御インスタンス
  float target_torque_body_Nm[PHYSICAL_CONST_THREE_DIM];  //!< 機体固定座標系目標トルク [Nm]

  float max_direct_feedback_angle_rad;                   //!< この値以上の角度偏差はこの値にクリップする [rad]
  float max_direct_feedback_rate_rad_s;                  //!< この値以上の速度偏差はこの値にクリップする [rad/s]
  float max_integral_angle_rad;                          //!< この値以上の積分偏差はこの値にクリップする [rad]
  float max_angle_to_run_integral_control_rad;           //!< 各軸の積分器について個別に実行可否を決める姿勢偏差角最大値 [rad]
  float integral_control_permission_angle_rad;           //!< 積分器の使用を許可する3軸RSS姿勢偏差角最大値 [rad]
  float spin_rate_around_sun_rad_s;                      //!< 太陽指向軸周りのスピンレート目標値 [rad/s]
  float acceptable_angle_error_to_spin_up_rad;           //!< 太陽指向軸周りのスピンアップ開始を許可する太陽指向角誤差許容値 [rad]
  // 姿勢マヌーバ過渡状態時
  float mtq_allowable_error_ratio_transient;             //!< CrossProduct誤差許容率 (0:全く許容しない ~ 1:全て許容)
  float correction_gain_transient;                       //!< CrossProduct誤差許容抑制ゲイン
  // 姿勢マヌーバ収束近傍時
  float mtq_allowable_error_ratio_stable;                //!< CrossProduct誤差許容率
  float correction_gain_stable;                          //!< CrossProduct誤差許容抑制ゲイン

  // LPF関連設定値
  float lpf_sample_freq_Hz;                              //!< LPFサンプリング周波数 [Hz]
  float lpf_trq_cutoff_freq_Hz;                          //!< スピン軸直交軸周り出力トルクLPFのカットオフ周波数 [Hz]
  float lpf_trq_damping_factor;                          //!< スピン軸直交軸周り出力トルクLPFの減衰係数 [-]
  float lpf_trq_cutoff_freq_spin_axis_Hz;                //!< スピン軸周り出力トルクLPFのカットオフ周波数 [Hz]
  float lpf_spin_rate_cutoff_freq_Hz;                    //!< 太陽指向軸周りのスピンアップ時間調整用LPFのカットオフ周波数 [Hz]

  ZFilter lpf_cntrl_torque[PHYSICAL_CONST_THREE_DIM];    //!< 出力飽和による制御特性劣化を防ぐために余剰トルク成分を落とすためのLPF
  ZFilter lpf_spin_rate;                                 //!< 姿勢状態に応じてゆっくり目標スピンレートを変更するためのLPF
  SUN_POINTING_AXIS_ID sun_pointing_axis_id;             //!< 太陽指向させる軸の配列上のID
  SUN_POINTING_CONTROL_STATE control_state;              //!< 太陽指向の達成状態
} SunPointing;

extern const SunPointing* const sun_pointing;

/**
 * @brief  太陽指向制御定期実行アプリ生成関数
 * @param  void
 * @return AppInfo
 */
AppInfo APP_SUN_POINTING_create_app(void);

CCP_CmdRet Cmd_APP_SUN_POINTING_SET_GAIN(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_APP_SUN_POINTING_SAVE_GAIN(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_APP_SUN_POINTING_SET_TARGET(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_APP_SUN_POINTING_SET_FEEDBACK_LIMIT_PARAMS(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_APP_SUN_POINTING_SET_LPF_PARAMS(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_APP_SUN_POINTING_SET_ANTI_INTOXICATION_PARAMS(const CommonCmdPacket* packet);

#endif
