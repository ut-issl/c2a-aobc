/**
 * @file
 * @brief  ケプラー軌道伝搬に関するライブラリ
 */
#ifndef KEPLER_ORBIT_H_
#define KEPLER_ORBIT_H_

#include <src_user/Library/physical_constants.h>
#include <src_user/Library/c2a_math.h>

/**
 * @struct KeplerOrbitalElements
 * @brief  軌道要素の構造体
 */
typedef struct
{
  // 形状
  float semi_major_axis_km;  //!< 軌道長半径 [km]
  float eccentricity;        //!< 離心率 [-]
  // 傾き
  float inclination_rad;     //!< 軌道傾斜角 [rad]
  float raan_rad;            //!< 昇交点赤経 [rad] Right Ascension of the Ascending Node
  float arg_perigee_rad;     //!< 近点引数   [rad] Argument of Perigee
  // 時間
  double epoch_jday;         //!< 元期 [ユリウス日]
} KeplerOrbitalElements;

/**
 * @struct KeplerOrbitConstants
 * @brief  ケプラー運動軌道計算における定数値
 */
typedef struct
{
  float semi_major_axis_km;  //!< 軌道長半径 [km]
  float eccentricity;        //!< 離心率 [-]
  float mean_motion_rad_s;   //!< 平均運動 [rad/s]
  double epoch_jday;         //!< 元期 [ユリウス日]
  float dcm_inplane_to_eci[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM]; //!< 軌道面内座標からECI座標への変換DCM
} KeplerOrbitConstants;

/**
 * @brief 軌道要素から軌道計算に使う定数値を求める
 * @param[out] kepler_orbit_constant   : ケプラー運動の定数
 * @param[in]  ortibal_elements        : 軌道要素
 */
void KEPLER_ORBIT_init_constants(KeplerOrbitConstants* kepler_orbit_constants,
                                 const KeplerOrbitalElements orbital_elements);

/**
 * @brief ケプラー運動でのECI座標系での位置・速度を求める
 * @note  無外乱で精度は低いのでfloatで扱う
 * @param[out] position_eci_km       : ECI座標系での位置ベクトル [km]
 * @param[out] velocity_eci_km_s     : ECI座標系での速度ベクトル [km/s]
 * @param[in]  kepler_orbit_constant : ケプラー運動の定数
 * @param[in]  current_time_jday     : 現在時刻 [ユリウス日], 軌道要素のepochと整合するように設定する
 * @return     C2A_MATH_ERRORに準ずる
 */
C2A_MATH_ERROR KEPLER_ORBIT_calc_position(float position_eci_km[PHYSICAL_CONST_THREE_DIM],
                                          float velocity_eci_km_s[PHYSICAL_CONST_THREE_DIM],
                                          const KeplerOrbitConstants kepler_orbit_constants,
                                          const double current_time_jday);

/**
 * @brief 1次収束法でケプラー方程式を解き、離心近点離角を求める
 * @note  離心率が小さいときは問題ないが、離心率が大きい場合はニュートン法を使う方が効率的
 * @param[in]  eccentricity          : 離心率
 * @param[in]  mean_anomaly_rad      : 平均近点離角 [rad]
 * @param[in]  angle_limit_rad       : 収束判定角度しきい値 [rad]
 * @param[in]  iteration_limit       : イタレーションしきい値 [回数]
 * @return     eccentric_anomaly_rad : 離心近点離角 [rad]
 */
float KEPLER_ORBIT_solve_kepler_equation_with_first_order(const float eccentricity,
                                                          const float mean_anomaly_rad,
                                                          const float angle_limit_rad,
                                                          const float iteration_limit);

/**
 * @brief ECI座標系での位置・速度から接触軌道要素を求める
 * @param[out] orbital_elements  : 軌道要素
 * @param[in]  position_eci_km   : ECI座標系での位置ベクトル [km]
 * @param[in]  velocity_eci_km_s : ECI座標系での速度ベクトル [km/s]
 * @param[in]  current_time_jday : 現在時刻 [ユリウス日]
 */
C2A_MATH_ERROR KEPLER_ORBIT_calc_oe_from_pos_vel(KeplerOrbitalElements* orbital_elements,
                                                 const float position_eci_km[PHYSICAL_CONST_THREE_DIM],
                                                 const float velocity_eci_km_s[PHYSICAL_CONST_THREE_DIM],
                                                 const double current_time_jday);

#endif
