/**
* @file   three_axis_control_mtq.h
* @brief  MTQを使った三軸姿勢制御アプリケーション
* @note   三軸姿勢制御のためのPID制御を行い、目標磁気モーメントを計算する。
*/
#ifndef THREE_AXIS_CONTROL_MTQ_H_
#define THREE_AXIS_CONTROL_MTQ_H_

#include <src_core/System/ApplicationManager/app_info.h>
#include <src_core/TlmCmd/common_cmd_packet.h>
#include "../../../../Library/physical_constants.h"
#include "../../../../Library/ControlUtility/cross_product_control.h";
#include "../../../../Library/pid_control.h"
#include "../../../../Library/stdint.h"
#include "../../../../Library/SignalProcess/z_filter.h"
#include "../../AOCS/aocs_error.h"

/**
 * @struct ThreeAxisControlMtq
 * @brief  MTQを使った三軸姿勢制御に必要な情報を管理するための構造体
 */
typedef struct
{
  PidControl pid_omega[PHYSICAL_CONST_THREE_DIM];         //!< 姿勢角速度に関するPID制御インスタンス
  PidControl pid_att[PHYSICAL_CONST_THREE_DIM];           //!< 姿勢角度に関するPID制御インスタンス
  PidGains gains_omega_cmd_tmp[PHYSICAL_CONST_THREE_DIM]; //!< コマンド一時保存用角速度ゲイン
  PidGains gains_att_cmd_tmp[PHYSICAL_CONST_THREE_DIM];   //!<  コマンド一時保存用姿勢ゲイン
  CrossProductControl cross_product_cntrl;                //!< CrossProduct制御インスタンス
  float target_torque_body_Nm[PHYSICAL_CONST_THREE_DIM];  //!< 機体固定座標系目標トルク [Nm]

  float mtq_lpf_sample_freq_Hz;                          //!< 出力トルクLPFのサンプリング周波数 [Hz]
  float mtq_lpf_cutoff_freq_Hz;                          //!< 出力トルクLPFのカットオフ周波数 [Hz]
  float mtq_lpf_damping_factor;                          //!< 出力トルクLPFの減衰係数 [-]

  float max_direct_feedback_angle_mtq_rad;               //!< この値以上の角度偏差はこの値にクリップする [rad]
  float max_direct_feedback_rate_mtq_rad_s;              //!< この値以上の速度偏差はこの値にクリップする [rad/s]
  float max_integral_angle_mtq_rad;                      //!< この値以上の積分偏差はこの値にクリップする [rad]
  float max_angle_to_run_integral_control_rad;           //!< 各軸の積分器について個別に実行可否を決める姿勢偏差角最大値 [rad]
  // 姿勢マヌーバ収束状態時のテレメ
  float allowable_error_ratio_stable;                    //!< CrossProduct誤差許容率 (0:全く許容しない ~ 1:全て許容)
  float correction_gain_stable;                          //!< CrossProduct誤差許容抑制ゲイン
  float allowable_error_ratio_transient;                 //!< CrossProduct誤差許容率 (0:全く許容しない ~ 1:全て許容)
  float correction_gain_transient;                       //!< CrossProduct誤差許容抑制ゲイン
  float acceptable_angle_error_as_stable_rad;            //!< 姿勢マヌーバ収束状態と見做す3軸誤差角RSS許容値 [rad]
  ZFilter lpf_mtq_cntrl[PHYSICAL_CONST_THREE_DIM];       //!< MTQ出力飽和による制御特性劣化を防ぐために余剰トルク成分を落とすためのLPF
} ThreeAxisControlMtq;

extern const ThreeAxisControlMtq* const three_axis_control_mtq;

AppInfo APP_TAC_MTQ_create_app(void);

CCP_CmdRet Cmd_APP_TAC_MTQ_SET_GAIN(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_APP_TAC_MTQ_SAVE_GAIN(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_APP_TAC_MTQ_SET_FEEDBACK_LIMIT_PARAMS(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_APP_TAC_MTQ_SET_LPF_PARAMS(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_APP_TAC_MTQ_SET_ANTI_INTOXICATION_PARAMS(const CommonCmdPacket* packet);

#endif
