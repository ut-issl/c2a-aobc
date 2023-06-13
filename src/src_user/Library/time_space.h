/**
* @file   time_space.h
* @brief  各種時刻換算や座標変換用の関数群の定義
*
*         どうしても桁が大きいものが出てくるので，一部関数はdoubleを扱う
*/
#ifndef TIME_SPACE_H_
#define TIME_SPACE_H_

#include "stdint.h"
#include "physical_constants.h"
#include "c2a_math.h"
#include "time_system/gps_time.h"

// 時刻換算関数
/**
 * @brief  GPSTimeからJulian Dayへの変換
 * @param  gps_time_week : GPSTime週番号 [week] (should be larger than zero)
 * @return Julian Day [day] (returns Julian day at the origin of GPSTime when the input is out-of-range )
 */
double TIME_SPACE_convert_gpstime_to_julian_day(const GPS_TIME gps_time);

/**
 * @brief  Julian DayからJulian Centuryへの変換
 *
 *         色々なところで使いまわす1次項だけ計算
 * @param  julian_day : JulianDay [day] (should be larger than JulainDay at Epoch of J2000)
 * @return Julian Century [century] (returns zero when the input is out-of-range )
 */
double TIME_SPACE_convert_julian_day_to_century(const double julian_day);

/**
 * @brief  Greenwitch Mean Sidereal Time (GMST) [rad]の計算
 * @param  julian_day : JulianDay [day] (should be larger than JulainDay at Epoch of J2000)
 * @return GMST [rad] (returns zero when the input is out-of-range)
 */
double TIME_SPACE_calc_gmst_rad(const double julian_day);

/**
 * @brief  Julian Dayからdecimal year(基準日からの10進数表記での経過年数)
 * @param  julian_day      : JulianDay [day] (should be larger than JulainDay at Epoch of J2000)
 * @param  julian_day_base : JulianDayで表した基準日 [day]
 * @return decimal year [year] (returns zero when the input is out-of-range )
 */
float TIME_SPACE_convert_julian_day_to_decimal_year(const double julian_day, const double julian_day_base);

/**
 * @brief  慣性太陽方向の計算
 *
 *         必要なのは方向だけなので，距離の計算は省略する
 * @param[in]   julian_century    : Julian Century [century] (should be larger than zero)
 * @param[out]  sun_direction_eci : 太陽方向ベクトル格納先 [-] (returns zero vector when the input is out-of-range)
 */
void TIME_SPACE_calc_sun_direction_eci(const double julian_century, float sun_direction_eci[3]);

/**
 * @brief      NEDからECEFへの座標変換
 * @param[out] dcm_ned_to_ecef : rotational matrix from ned to ecef
 * @param[in]  lat_rad         : geodetic lattitude [rad]
 * @param[in]  lon_rad         : geodetic longitude [rad]
 */
void TIME_SPACE_trans_ned_to_ecef(float dcm_ned_to_ecef[][PHYSICAL_CONST_THREE_DIM],
                            const float lat_rad, const float lon_rad);

/**
 * @brief      うるう秒 (GPSTime - UTC) の更新
 * @param[in]  leap_seconds_updated   : 更新後のうるう秒 [s]
 */
void TIME_SPACE_update_leap_seconds(const float leap_seconds_updated);

/**
 * @brief      ECIからECEFへの変換 R*N*P
 *
 *             歳差と章動を考慮した変換
 * @param[in]  julian_century  : Julian Century [century] (should be larger than zero)
 * @param[in]  gmst_rad        : Greenwitch Mean Sidereal Time [rad] (should be larger than zero)
 * @param[out] dcm_eci_to_ecef : coordinate transformation from eci to ecef
                                 (set unit matrix when the input is out-of-range)
 * @return     C2A_MATH_ERROR_RANGE_OVER : Julian CenturyまたはGreenwitch Mean Sidereal Timeが0以下
 * @return     C2A_MATH_ERROR_OK
 */
C2A_MATH_ERROR TIME_SPACE_trans_eci_to_ecef(const double julian_day, const double gmst_rad,
                                                  double dcm_eci_to_ecef[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM]);

/**
 * @brief      ECIからECEFへの変換 R
 *
 *             歳差と章動を考慮しない変換
 * @param[in]  gmst_rad        : Greenwitch Mean Sidereal Time [rad] (should be larger than zero)
 * @param[out] dcm_eci_to_ecef : coordinate transformation from eci to ecef
                                 (set unit matrix when the input is out-of-range)
 * @return     C2A_MATH_ERROR_RANGE_OVER : Greenwitch Mean Sidereal Timeが0以下
 * @return     C2A_MATH_ERROR_OK
 */
C2A_MATH_ERROR TIME_SPACE_trans_eci_to_ecef_rotation_only(const double gmst_rad,
                                                                double dcm_eci_to_ecef[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM]);

/**
 * @brief      測地経緯度, 高度を地心余緯度, 地心距離に変換
 * @param[out] colat_rad : co-lattitude in geocentric [rad]
 * @param[out] radious_m : radious distance in geocentric [m]
 * @param[in]  lla_rad_m : lat, lon, alt in geodetic
 * @return     C2A_MATH_ERROR_OK
 */
C2A_MATH_ERROR TIME_SPACE_convert_geodetic_to_geocentric(float* colat_rad, float* radious_m,
                                                   const double lla_rad_m[PHYSICAL_CONST_THREE_DIM]);

/**
 * @brief      ECEF カルテシアン位置を経緯度・高度に換算
 * @param[out] lla_rad_m  : lat, lon, altitude [rad,m]
 * @param[in]  pos_ecef_m : satellite position vector in ecef [m]
 * @return     C2A_MATH_ERROR_OK
 */
C2A_MATH_ERROR TIME_SPACE_convert_ecef_to_lla(double lla_rad_m[PHYSICAL_CONST_THREE_DIM],
                                        const double pos_ecef_m[PHYSICAL_CONST_THREE_DIM]);

/**
 * @brief      経緯度・高度をECEF カルテシアン位置に換算
 * @param[out] pos_ecef_m : satellite position vector in ecef [m]
 * @param[in]  lla_rad_m  : lat, lon, altitude [rad,m]
 * @return     C2A_MATH_ERROR_OK
 */
C2A_MATH_ERROR TIME_SPACE_convert_lla_to_ecef(double pos_ecef_m[PHYSICAL_CONST_THREE_DIM],
                                        const double lla_rad_m[PHYSICAL_CONST_THREE_DIM]);

/**
 * @brief      ECEF位置・速度をECI位置・速度に換算
 * @param[out] pos_eci_m        : satellite position vector in eci [m]
 * @param[out] vel_eci_m_s      : satellite velocity vector in eci [m/s]
 * @param[in]  dcm_ecef_to_eci  : coordinate transformation from ecef to eci
 * @param[in]  pos_ecef_m       : satellite position vector in ecef [m]
 * @param[in]  vel_ecef_m_s     : satellite velocity vector in ecef [m/s]
 * @return     C2A_MATH_ERROR_OK
 * @note       位置変換のみの場合はDCM取得後に行列をかけるだけなので，関数化しない
 */
C2A_MATH_ERROR TIME_SPACE_convert_posvel_ecef_to_eci(double pos_eci_m[PHYSICAL_CONST_THREE_DIM],
                                                     double vel_eci_m_s[PHYSICAL_CONST_THREE_DIM],
                                               const double dcm_ecef_to_eci[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM],
                                               const double pos_ecef_m[PHYSICAL_CONST_THREE_DIM],
                                               const double vel_ecef_m_s[PHYSICAL_CONST_THREE_DIM]);

/**
 * @brief      ECI位置・速度をECEF位置・速度に換算
 * @param[out] pos_ecef_m       : satellite position vector in ecef [m]
 * @param[out] vel_ecef_m_s     : satellite velocity vector in ecef [m/s]
 * @param[in]  dcm_eci_to_ecef  : coordinate transformation from ecef to eci
 * @param[in]  pos_eci_m        : satellite position vector in eci [m]
 * @param[in]  vel_eci_m_s      : satellite velocity vector in eci [m/s]
 * @return     C2A_MATH_ERROR_OK
 * @note       位置変換のみの場合はDCM取得後に行列をかけるだけなので，関数化しない
 */
C2A_MATH_ERROR TIME_SPACE_convert_posvel_eci_to_ecef(double pos_ecef_m[PHYSICAL_CONST_THREE_DIM],
                                                     double vel_ecef_m_s[PHYSICAL_CONST_THREE_DIM],
                                               const double dcm_eci_to_ecef[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM],
                                               const double pos_eci_m[PHYSICAL_CONST_THREE_DIM],
                                               const double vel_eci_m_s[PHYSICAL_CONST_THREE_DIM]);

#endif
