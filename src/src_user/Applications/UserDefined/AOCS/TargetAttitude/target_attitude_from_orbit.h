/**
* @file   target_attitude_from_orbit.h
* @brief  推定軌道から目標姿勢を計算するアプリ
*/
#ifndef TARGET_ATTITUDE_FROM_ORBIT_H_
#define TARGET_ATTITUDE_FROM_ORBIT_H_

#include <src_core/System/ApplicationManager/app_info.h>
#include <src_core/TlmCmd/common_cmd_packet.h>
#include <src_user/Library/physical_constants.h>
#include <src_user/Library/matrix33.h>
#include <src_user/Library/quaternion.h>

/**
 * @enum   APP_TAFO_TARGET_DIRECITON TODO Fix typo (This will be major update)
 * @brief  機体座標系の特定軸を向ける方向を指定する
 * @note   uint8_tを想定
 */
typedef enum
{
  APP_TAFO_TARGET_DIRECITON_SUN = 0,                  //!< 太陽方向
  APP_TAFO_TARGET_DIRECITON_SAT_VELOCITY,             //!< 衛星速度方向
  APP_TAFO_TARGET_DIRECITON_SAT_RELATIVE_VELOCITY,    //!< 衛星相対速度方向
  APP_TAFO_TARGET_DIRECITON_EARTH_CENTER,             //!< 地心方向
  APP_TAFO_TARGET_DIRECITON_EARTH_SURFACE,            //!< 地表点
  APP_TAFO_TARGET_DIRECITON_ORBIT_NORMAL,             //!< 軌道面外方向
  APP_TAFO_TARGET_DIRECITON_MAX
} APP_TAFO_TARGET_DIRECITON;

/**
 * @struct TargetAttitudeFromOrbit
 * @brief  推定軌道から目標姿勢を計算するアプリ
 */
typedef struct
{
  Quaternion quaternion_target_i2t;                         //!< 計算された目標Quaternion
  uint8_t is_enabled;                                       //!< 目標Quaternionをaocs_managerにセットするか否か
  APP_TAFO_TARGET_DIRECITON main_target_direction;          //!< vec_to_main_target_bodyの目標方向
  APP_TAFO_TARGET_DIRECITON sub_target_direction;           //!< vec_to_sub_target_bodyの目標方向
  float vec_to_main_target_body[PHYSICAL_CONST_THREE_DIM];  //!< main_target_directionに向ける機体座標系ベクトル
  float vec_to_sub_target_body[PHYSICAL_CONST_THREE_DIM];   //!< 姿勢を一意に決定するために指定する機体座標系ベクトル
  MATRIX33_ROTATION_AXIS offset_angle_axis;                   //!< オフセット角を与える軸
  float offset_angle_rad;                                   //!< オフセット角, rad
  float target_lla_rad_m[PHYSICAL_CONST_THREE_DIM];         //!< 目標地表点の緯度・経度・高度, rad+rad+m
} TargetAttitudeFromOrbit;

extern const TargetAttitudeFromOrbit* const target_attitude_from_orbit;

/**
 * @brief  目標姿勢計算アプリ生成関数
 * @param  void
 * @return AppInfo
 */
AppInfo APP_TAFO_create_app(void);

CCP_CmdRet Cmd_APP_TAFO_SET_MAIN_TARGET_DIRECTION(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_APP_TAFO_SET_SUB_TARGET_DIRECTION(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_APP_TAFO_SET_OFFSET_ANGLE_AND_AXIS(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_APP_TAFO_SET_TARGET_LLA(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_APP_TAFO_ENABLE(const CommonCmdPacket* packet);

#endif
