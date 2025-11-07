/**
* @file   gpsr_orbit_propagator.h
* @brief  GPSRベースのケプラー軌道の伝搬を行うアプリ
*/
#ifndef GPSR_ORBIT_PROPAGATOR_H_
#define GPSR_ORBIT_PROPAGATOR_H_

#include <src_core/System/ApplicationManager/app_info.h>
#include <src_core/TlmCmd/common_cmd_packet.h>
#include <src_user/Library/Orbit/kepler_orbit.h>

/**
 * @struct GpsrOrbitPropagator
 * @brief  GPSRテレメを用いたケプラー軌道伝搬における軌道要素
 */
typedef struct
{
  KeplerOrbitalElements orbital_elements; //!< 軌道要素
  KeplerOrbitalElements orbital_elements_threshold; //!< GPSRテレメを軌道要素更新に用いるかを判断するための更新幅閾値
  KeplerOrbitConstants  orbit_constants;  //!< 軌道要素から計算される定数
} GpsrOrbitPropagator;

/**
 * @brief GPSRテレメを用いてケプラー運動でのECI座標系での位置・速度を求める
 * @note  無外乱で精度は低いのでfloatで扱う
 * @param[out] position_eci_km       : ECI座標系での位置ベクトル [km]
 * @param[out] velocity_eci_km_s     : ECI座標系での速度ベクトル [km/s]
 * @param[in]  current_time_jday     : 現在時刻 [ユリウス日], 軌道要素のepochと整合するように設定する
 * @return     C2A_MATH_ERRORに準ずる
 */
C2A_MATH_ERROR GPSR_ORBIT_PROPAGATOR_calc_position(float position_eci_km[PHYSICAL_CONST_THREE_DIM],
                                                   float velocity_eci_km_s[PHYSICAL_CONST_THREE_DIM],
                                                   const double current_time_jday);

extern const GpsrOrbitPropagator* const gpsr_orbit_propagator;

AppInfo APP_GPSROP_create_app(void);

CCP_CmdRet Cmd_APP_GPSROP_SET_ORBIT_ELEMENTS_THRESHOLD(const CommonCmdPacket* packet);

#endif
