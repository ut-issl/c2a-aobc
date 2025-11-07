/**
* @file   orbit_calculator.h
* @brief  軌道計算(伝搬・推定)をまとめるアプリ
*/
#ifndef ORBIT_CALCULATOR_H_
#define ORBIT_CALCULATOR_H_

#include <src_core/System/ApplicationManager/app_info.h>
#include <src_core/TlmCmd/common_cmd_packet.h>
#include <src_user/Library/Orbit/kepler_orbit.h>

/**
 * @enum   APP_ORBIT_CALC_METHOD
 * @brief  軌道計算に使う手法の選択
 * @note   uint8_tを想定
 */
typedef enum
{
  APP_ORBIT_CALC_METHOD_KEPLER = 0,  //!< ケプラー軌道伝搬
  APP_ORBIT_CALC_METHOD_SGP4,        //!< SGP4軌道伝搬
  APP_ORBIT_CALC_METHOD_KEPLER_GPSR, //!< GPS受信機ベースのケプラー軌道伝搬
  APP_ORBIT_CALC_METHOD_GPSR_EKF,    //!< GPS受信機ベースのEKF
  APP_ORBIT_CALC_METHOD_MAX
} APP_ORBIT_CALC_METHOD;

/**
 * @enum   APP_ORBIT_CALC_GPSR_UPDATE
 * @brief  GPSRテレメをもとにしたKepler軌道伝播を使用するフラグ
 * @note   uint8_tを想定
 */
typedef enum
{
  APP_ORBIT_CALC_GPSR_UPDATE_DISABLE = 0, //!< GPSRテレメをもとにしたKepler軌道伝播を使用しない
  APP_ORBIT_CALC_GPSR_UPDATE_ENABLE,      //!< GPSRテレメをもとにしたKepler軌道伝播を使用する
  APP_ORBIT_CALC_GPSR_UPDATE_MAX
} APP_ORBIT_CALC_GPSR_UPDATE;

/**
 * @struct OrbitCalculator
 * @brief  軌道計算まとめアプリのテレメトリ
 */
typedef struct
{
  APP_ORBIT_CALC_METHOD method; //!< 使う手法
  APP_ORBIT_CALC_GPSR_UPDATE gpsr_update; //!< GPSRテレメをもとにしたKepler軌道伝播を使用するフラグ
  //!< GPSRテレメをもとにしたKepler軌道伝播のECI座標系での位置ベクトル [km]
  float position_gpsr_kepler_eci_km[PHYSICAL_CONST_THREE_DIM];
  //!< GPSRテレメをもとにしたKepler軌道伝播のECI座標系での速度ベクトル [km/s]
  float velocity_gpsr_kepler_eci_km_s[PHYSICAL_CONST_THREE_DIM];
} OrbitCalculator;

extern const OrbitCalculator* const orbit_calculator;

AppInfo APP_ORBIT_CALC_create_app(void);

CCP_CmdRet Cmd_APP_ORBIT_CALC_SET_METHOD(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_APP_ORBIT_CALC_SET_GPSR_UPDATE(const CommonCmdPacket* packet);

#endif
