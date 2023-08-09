/**
* @file   three_axis_control_rw.h
* @brief  RWを使った三軸姿勢制御アプリケーション
* @note   三軸姿勢制御のためのPID制御を行い、目標回転トルクを計算する。
*/
#ifndef THREE_AXIS_CONTROL_RW_H_
#define THREE_AXIS_CONTROL_RW_H_

#include <stdint.h>
#include <src_core/System/ApplicationManager/app_info.h>
#include <src_core/TlmCmd/common_cmd_packet.h>
#include <src_user/Library/physical_constants.h>
#include <src_user/Library/pid_control.h>
#include <src_user/Applications/UserDefined/AOCS/aocs_error.h>

/**
 * @struct ThreeAxisControlRw
 * @brief  RW三軸姿勢制御に必要な情報を管理するための構造体
 */
typedef struct
{
  PidControl pid_omega[PHYSICAL_CONST_THREE_DIM];         //!< 姿勢角速度に関するPID制御インスタンス
  PidControl pid_att[PHYSICAL_CONST_THREE_DIM];           //!< 姿勢角度に関するPID制御インスタンス
  PidGains gains_omega_cmd_tmp[PHYSICAL_CONST_THREE_DIM]; //!< コマンド一時保存用角速度ゲイン
  PidGains gains_att_cmd_tmp[PHYSICAL_CONST_THREE_DIM];   //!<  コマンド一時保存用姿勢ゲイン
  float target_torque_body_Nm[PHYSICAL_CONST_THREE_DIM];  //!< 機体固定座標系目標トルク [Nm]
} ThreeAxisControlRw;

extern const ThreeAxisControlRw* const three_axis_control_rw;

AppInfo APP_TAC_RW_create_app(void);

CCP_CmdRet Cmd_APP_TAC_RW_SET_GAIN(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_APP_TAC_RW_SAVE_GAIN(const CommonCmdPacket* packet);

#endif
