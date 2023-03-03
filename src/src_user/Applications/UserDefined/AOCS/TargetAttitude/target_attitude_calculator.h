/**
* @file   target_attitude_calculator.h
* @brief  目標姿勢計算をまとめるアプリ
*/
#ifndef TARGET_ATTITUDE_CALCULATOR_H_
#define TARGET_ATTITUDE_CALCULATOR_H_

#include <src_core/System/ApplicationManager/app_info.h>
#include <src_core/TlmCmd/common_cmd_packet.h>
#include "../../../../Library/quaternion.h"

/**
 * @enum   APP_TARGET_ATT_CALC_MODE
 * @brief  目標Quaternionの計算方法を指定する
 * @note   uint8_tを想定
 */
typedef enum
{
  APP_TARGET_ATT_CALC_MODE_MANUAL = 0,              //!< 目標Quaternionをコマンドで指定
  APP_TARGET_ATT_CALC_MODE_CALCULATOR_FROM_ORBIT,   //!< 推定軌道情報から目標Quaternion計算
  APP_TARGET_ATT_CALC_MODE_QUATERNION_INTERPOLATOR, //!< コマンド指定の目標Quaternionの内挿計算
  APP_TARGET_ATT_CALC_MODE_MAX
} APP_TARGET_ATT_CALC_MODE;

/**
 * @struct TargetAttitudeCalculator
 * @brief  目標姿勢計算方法を表すテレメトリ
 */
typedef struct
{
  APP_TARGET_ATT_CALC_MODE mode;                             //!< 目標Quaternion計算方法
  Quaternion quaternion_target_i2t;                          //!< 目標Quaternion
  Quaternion quaternion_target_i2t_previous;                 //!< 前回の目標Quaternion（目標角速度計算用）
  float ang_vel_target_body_rad_s[PHYSICAL_CONST_THREE_DIM]; //!< 目標角速度
  ObcTime obctime;                                           //!< 目標角速度計算用ObcTime
  uint8_t is_enabled;                                        //!< 目標Quaternionをaocs_managerにセットするか否か
} TargetAttitudeCalculator;

extern const TargetAttitudeCalculator* const target_attitude_calculator;

/**
 * @brief  目標姿勢計算アプリ生成関数
 * @param  void
 * @return AppInfo
 */
AppInfo APP_TARGET_ATT_CALC_create_app(void);

CCP_EXEC_STS Cmd_APP_TARGET_ATT_CALC_SET_MODE(const CommonCmdPacket* packet);
CCP_EXEC_STS Cmd_APP_TARGET_ATT_CALC_ENABLE(const CommonCmdPacket* packet);

#endif
