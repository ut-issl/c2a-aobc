#pragma section REPRO
/**
* @file   kepler_orbit.c
* @brief  ケプラー軌道伝搬に関するライブラリ
*/

#include "kepler_orbit.h"

#include <math.h>
#include <src_user/Library/matrix33.h>
#include <src_user/Library/vector3.h>
#include <src_user/Library/math_constants.h>
#include <src_core/Library/print.h>

#define KEPLER_ORBIT_ECCENTRICITY_THRESHOLD (1.0e-5f) //!< 円軌道とみなす離心率閾値

// TODO_L:次の2つの定数をコマンドで変えれるようにするか、リプロで良いか検討する
static const float   KEPLER_ORBIT_kKeplerAngleLimit_rad_ = 1e-5; //!< ケプラー方程式を解くときの角度リミット
static const uint8_t KEPLER_ORBIT_kKeplerIterationLimit_ = 10;   //!< ケプラー方程式を解くときのイタレーションリミット

void KEPLER_ORBIT_init_constants(KeplerOrbitConstants* kepler_orbit_constants,
                                 const KeplerOrbitalElements orbital_elements)
{
  // 平均運動を求める TODO_L:計算精度を考えると平均運動をアップリンクするほうが良いかも
  float earth_gravity_const_km3_s2 = PHYSICAL_CONST_EARTH_GRAVITY_CONST_m3_s2 / 1.0e9f;
  float semi_major_axis_km3 = powf(orbital_elements.semi_major_axis_km, 3.0f);
  kepler_orbit_constants->mean_motion_rad_s = sqrtf(earth_gravity_const_km3_s2 / semi_major_axis_km3);

  // 座標変換を求める
  float dcm_arg_perigee[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM];
  float dcm_inclination[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM];
  float dcm_raan[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM];
  float dcm_inc_arg[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM];
  MATRIX33_make_rotation(dcm_arg_perigee, -1.0f * orbital_elements.arg_perigee_rad, MATRIX33_ROTATION_AXIS_Z);
  MATRIX33_make_rotation(dcm_inclination, -1.0f * orbital_elements.inclination_rad, MATRIX33_ROTATION_AXIS_X);
  MATRIX33_make_rotation(dcm_raan,        -1.0f * orbital_elements.raan_rad,        MATRIX33_ROTATION_AXIS_Z);
  MATRIX33_multiply_matrix_matrix(dcm_inc_arg, dcm_inclination, dcm_arg_perigee);
  MATRIX33_multiply_matrix_matrix(kepler_orbit_constants->dcm_inplane_to_eci, dcm_raan, dcm_inc_arg);

  // そのまま代入する
  kepler_orbit_constants->semi_major_axis_km = orbital_elements.semi_major_axis_km;
  kepler_orbit_constants->eccentricity       = orbital_elements.eccentricity;
  kepler_orbit_constants->epoch_jday         = orbital_elements.epoch_jday;

  return;
}

C2A_MATH_ERROR KEPLER_ORBIT_calc_position(float position_eci_km[PHYSICAL_CONST_THREE_DIM],
                                          float velocity_eci_km_s[PHYSICAL_CONST_THREE_DIM],
                                          const KeplerOrbitConstants kepler_orbit_constants,
                                          const double current_time_jday)
{
  // 一旦短い変数に置く
  float  a_km = kepler_orbit_constants.semi_major_axis_km;
  float  e    = kepler_orbit_constants.eccentricity;
  float  n_rad_s  = kepler_orbit_constants.mean_motion_rad_s;
  double dt_s = (current_time_jday - kepler_orbit_constants.epoch_jday) * PHYSICAL_CONST_EARTH_SOLAR_DAY_s;

  float mean_anomaly_rad = kepler_orbit_constants.mean_motion_rad_s * (float)dt_s;
  float l_rad = C2A_MATH_normalize_zero_2pi(mean_anomaly_rad);

  // Kepler方程式を解く
  float eccentric_anomaly_rad;
  eccentric_anomaly_rad = KEPLER_ORBIT_solve_kepler_equation_with_first_order(e, l_rad,
                                                                              KEPLER_ORBIT_kKeplerAngleLimit_rad_,
                                                                              KEPLER_ORBIT_kKeplerIterationLimit_);
  float u_rad = C2A_MATH_normalize_zero_2pi(eccentric_anomaly_rad);

  // 軌道面内の位置速度を求める
  float cos_u = cosf(u_rad);
  float sin_u = sinf(u_rad);
  float a_sqrt_e_km = a_km * sqrtf(1.0f - e * e);
  float e_cos_u = 1.0f - e * cos_u;

  float pos_inplane_km[PHYSICAL_CONST_THREE_DIM];
  pos_inplane_km[0] = a_km * (cos_u - e);
  pos_inplane_km[1] = a_sqrt_e_km * sin_u;
  pos_inplane_km[2] = 0.0f;

  float vel_inplane_km_s[PHYSICAL_CONST_THREE_DIM];
  vel_inplane_km_s[0] = -1.0f * a_km * n_rad_s * sin_u / e_cos_u;
  vel_inplane_km_s[1] =  n_rad_s * a_sqrt_e_km * cos_u / e_cos_u;
  vel_inplane_km_s[2] = 0.0f;

  // ECIに座標変換する
  MATRIX33_multiply_matrix_vector(position_eci_km,
                                kepler_orbit_constants.dcm_inplane_to_eci,
                                pos_inplane_km);
  MATRIX33_multiply_matrix_vector(velocity_eci_km_s,
                                kepler_orbit_constants.dcm_inplane_to_eci,
                                vel_inplane_km_s);

  return C2A_MATH_ERROR_OK;
}

float KEPLER_ORBIT_solve_kepler_equation_with_first_order(const float eccentricity,
                                                          const float mean_anomaly_rad,
                                                          const float angle_limit_rad,
                                                          const float iteration_limit)
{
  float u_prev_rad = mean_anomaly_rad;
  float u_rad = 0.0f;
  int i;
  for (i = 0; i < iteration_limit; i++)
  {
    u_rad = mean_anomaly_rad + eccentricity * sinf(u_prev_rad);

    float diff_abs_rad;
    diff_abs_rad = fabsf(u_rad - u_prev_rad);
    if (diff_abs_rad < angle_limit_rad) break;

    u_prev_rad = u_rad;
  }

  return u_rad;
}

C2A_MATH_ERROR KEPLER_ORBIT_calc_oe_from_pos_vel(KeplerOrbitalElements* orbital_elements,
                                                 const float position_eci_km[PHYSICAL_CONST_THREE_DIM],
                                                 const float velocity_eci_km_s[PHYSICAL_CONST_THREE_DIM],
                                                 const double current_time_jday)
{
  float semi_major_axis_km;
  float eccentricity;
  float inclination_rad;
  float raan_rad;
  float arg_perigee_rad;
  double epoch_jday;
  const float mu_km3_s2 = PHYSICAL_CONST_EARTH_GRAVITY_CONST_km3_s2; // 短い変数に置き換える

  // semi_major_axis_km
  float r_km = VECTOR3_norm(position_eci_km);
  float epsilon_km2_s2 = VECTOR3_inner_product(velocity_eci_km_s, velocity_eci_km_s) / 2.0f - mu_km3_s2 / r_km;
  if (fabsf(epsilon_km2_s2) < MATH_CONST_PROHIBIT_DIVISION_VALUE) return C2A_MATH_ERROR_SINGULAR;
  semi_major_axis_km = - mu_km3_s2 / 2.0f / epsilon_km2_s2;

  // eccentricity
  float h[PHYSICAL_CONST_THREE_DIM];
  VECTOR3_outer_product(h, position_eci_km, velocity_eci_km_s);
  float v_temp1[PHYSICAL_CONST_THREE_DIM];
  float v_temp2[PHYSICAL_CONST_THREE_DIM];
  float P[PHYSICAL_CONST_THREE_DIM];
  VECTOR3_scalar_product(v_temp1, - mu_km3_s2 / r_km, position_eci_km);
  VECTOR3_outer_product(v_temp2, h, velocity_eci_km_s);
  VECTOR3_subtract(P, v_temp1, v_temp2);
  eccentricity = VECTOR3_norm(P) / mu_km3_s2;

  // inclination_rad
  float W_hat[PHYSICAL_CONST_THREE_DIM];
  VECTOR3_normalize(W_hat, h);
  inclination_rad = acosf(W_hat[2]);
  if (inclination_rad < MATH_CONST_PROHIBIT_DIVISION_VALUE) return C2A_MATH_ERROR_SINGULAR; // TODO_L i->0に対応

  // raan_rad
  raan_rad = atan2f(W_hat[0], - W_hat[1]);

  // arg_perigee_rad
  if (eccentricity > KEPLER_ORBIT_ECCENTRICITY_THRESHOLD)
  {
    float N_hat[PHYSICAL_CONST_THREE_DIM];
    float P_hat[PHYSICAL_CONST_THREE_DIM];
    const float k_hat[PHYSICAL_CONST_THREE_DIM] = { 0.0f, 0.0f, 1.0f }; // ECI座標系z方向単位ベクトル
    VECTOR3_outer_product(v_temp1, k_hat, h);
    VECTOR3_normalize(N_hat, v_temp1);
    VECTOR3_normalize(P_hat, P);
    arg_perigee_rad = acosf(VECTOR3_inner_product(N_hat, P_hat));
    if (P_hat[2] < 0.0f)
    {
      arg_perigee_rad = MATH_CONST_2PI - arg_perigee_rad;
    }
  }
  else // e->0のとき近点引数を定義できないため、代わりに緯度引数(argument of latitude)を用いる
  {
    float x_bar = position_eci_km[0] * cosf(raan_rad) + position_eci_km[1] * sinf(raan_rad);
    float y_bar = (-position_eci_km[0] * sinf(raan_rad) + position_eci_km[1] * cosf(raan_rad)) * cosf(inclination_rad) +
                  position_eci_km[2] * sinf(inclination_rad);
    arg_perigee_rad = atan2f(y_bar, x_bar);
  }

  // epoch_jday
  if (eccentricity > KEPLER_ORBIT_ECCENTRICITY_THRESHOLD)
  {
    float e_cosE = 1.0f - r_km / semi_major_axis_km;
    float e_sinE = VECTOR3_inner_product(position_eci_km, velocity_eci_km_s) / sqrtf(mu_km3_s2 * semi_major_axis_km);
    float E_rad = atan2f(e_sinE, e_cosE);
    float n = sqrtf(mu_km3_s2 / semi_major_axis_km / semi_major_axis_km / semi_major_axis_km);
    epoch_jday = current_time_jday - (double)((E_rad - e_sinE) / n / PHYSICAL_CONST_EARTH_SOLAR_DAY_s);
  }
  else // e->0のとき近点引数の代わりに緯度引数を用いるため、現在時刻がそのまま近心点通過時刻となる
  {
    epoch_jday = current_time_jday;
  }

  orbital_elements->semi_major_axis_km = semi_major_axis_km;
  orbital_elements->eccentricity = eccentricity;
  orbital_elements->inclination_rad = inclination_rad;
  orbital_elements->raan_rad = raan_rad;
  orbital_elements->arg_perigee_rad = arg_perigee_rad;
  orbital_elements->epoch_jday = epoch_jday;

  return C2A_MATH_ERROR_OK;
}

#pragma section
