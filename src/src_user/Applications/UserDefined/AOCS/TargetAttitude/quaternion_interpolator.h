/**
* @file   quaternion_interpolator.h
* @brief  目標Quaternionを内挿計算するアプリ
*/
#ifndef QUATERNION_INTERPOLATOR_H_
#define QUATERNION_INTERPOLATOR_H_

#include <src_core/System/ApplicationManager/app_info.h>
#include <src_core/System/TimeManager/obc_time.h>
#include <src_core/TlmCmd/common_cmd_packet.h>
#include "../../../../Library/quaternion.h"

#define APP_QI_TARGET_MAX (20) // 衛星が確保可能な目標Quaternionの数

/**
 * @struct APP_QI_TargetElement
 * @brief  目標QuaternionとTIを合わせた構造体
 */
typedef struct
{
  Quaternion quaternion_target_i2t;  //!< 目標Quaternion
  cycle_t attitude_changed_ti;       //!< 姿勢変更完了予定TI
} APP_QI_TargetElement;

/**
 * @struct QuaternionInterpolator
 * @brief  目標Quaternionを内挿計算するためのテレメトリ
 */
typedef struct
{
  Quaternion quaternion_target_i2t;                    //!< 計算された目標Quaternion
  uint8_t is_enabled;                                  //!< 目標Quaternionをaocs_managerにセットするか否か
  APP_QI_TargetElement target_list[APP_QI_TARGET_MAX]; //!< 目標Quaternion＋TIのリスト（コマンドで指定する）
  uint8_t current_target_num;                          //!< 現在の目標の数 (< APP_QI_kTargetMax)
  uint8_t index;                                       //!< 現在参照しているリストのindex
  cycle_t previous_attitude_changed_ti;                //!< 前回の姿勢変更完了TI
  Quaternion previous_quaternion_target_i2t;           //!< 前回の目標Quaternion
} QuaternionInterpolator;

extern const QuaternionInterpolator* const quaternion_interpolator;

/**
 * @brief  目標姿勢計算アプリ生成関数
 * @param  void
 * @return AppInfo
 */
AppInfo APP_QI_create_app(void);

CCP_CmdRet Cmd_APP_QI_SET_NEXT_TARGET_QUATERNION(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_APP_QI_SET_PREV_TARGET_QUATERNION(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_APP_QI_SET_NEXT_TARGET_QUATERNION_JDAY(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_APP_QI_RESET_TARGET_QUATERNION(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_APP_QI_ENABLE(const CommonCmdPacket* packet);

#endif
