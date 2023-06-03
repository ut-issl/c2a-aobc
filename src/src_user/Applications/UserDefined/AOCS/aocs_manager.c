#pragma section REPRO
/**
* @file   aocs_manager.c
* @brief  AOCS系の変数などをH/Wと切り分けて管理するマネージャーアプリ
*/

#include "aocs_manager.h"
#include <math.h>
#include <src_core/TlmCmd/common_cmd_packet_util.h>
#include "../../../Library/c2a_math.h"
#include "../../../Library/math_constants.h"
#include "../../../Library/vector3.h"
#include "../../../Library/matrix33.h"
#include "../../../Library/ControlUtility/gyroscopic_effect.h"

#include "AttitudeControl/unloading.h"
#include "aocs_mode_manager.h"

// SatelliteParameters
#include "../../../Settings/SatelliteParameters/orbit_parameters.h"
#include "../../../Settings/SatelliteParameters/structure_parameters.h"

static AocsManager        aocs_manager_;
const  AocsManager* const aocs_manager = &aocs_manager_;

static void APP_AOCS_MANAGER_init_(void);
static void APP_AOCS_MANAGER_exec_(void);

/**
 * @brief  1Byteに複数のフラグを入れ込むための関数
 * @param[in] flags_in : 計算前の1Byteのフラグ
 * @param[in] flag_on  : 今回確認する1bitのフラグを1にするかどうか
 * @param[in] bit      : 最下位から何bit目を操作するか(1~8まで)
 * @return 計算後のフラグ
 */
static uint8_t APP_AOCS_MANAGER_bit_flag_control_(const uint8_t flags_in, const uint8_t flag_on, const uint8_t bit);

AppInfo APP_AOCS_MANAGER_create_app(void)
{
  return AI_create_app_info("AOCS Manager", APP_AOCS_MANAGER_init_, APP_AOCS_MANAGER_exec_);
}

static void APP_AOCS_MANAGER_init_(void)
{
  // 下記初期化値は物理的あり得る範囲でテキトウな値であり、全てsetter関数で意味のある値に変更されることを想定している

  // 衛星特性
  aocs_manager_.mass_sc_kg = STRUCTURE_PARAMETERS_mass_sc_kg;
  VECTOR3_initialize(aocs_manager_.rmm_sc_body_Am2, 0.1f);

  aocs_manager_.inertia_tensor_sc_body_kgm2[0][0] =  0.1f;
  aocs_manager_.inertia_tensor_sc_body_kgm2[0][1] =  0.0e-4f;
  aocs_manager_.inertia_tensor_sc_body_kgm2[0][2] =  0.0e-4f;
  aocs_manager_.inertia_tensor_sc_body_kgm2[1][0] =  0.0e-4f;
  aocs_manager_.inertia_tensor_sc_body_kgm2[1][1] =  0.2f;
  aocs_manager_.inertia_tensor_sc_body_kgm2[1][2] =  0.0e-4f;
  aocs_manager_.inertia_tensor_sc_body_kgm2[2][0] =  0.0e-4f;
  aocs_manager_.inertia_tensor_sc_body_kgm2[2][1] =  0.0e-4f;
  aocs_manager_.inertia_tensor_sc_body_kgm2[2][2] =  0.2f;
  // 角速度
  VECTOR3_initialize(aocs_manager_.ang_vel_obs_body_rad_s,    0.0f);
  VECTOR3_initialize(aocs_manager_.ang_vel_est_body_rad_s,    0.0f);
  VECTOR3_initialize(aocs_manager_.ang_vel_target_body_rad_s, 0.0f);
  VECTOR3_initialize(aocs_manager_.ang_vel_error_body_rad_s,  0.0f);
  aocs_manager_.limit_angular_velocity_rad_s = PHYSICAL_CONST_degree_to_radian(0.8f);
  // Quaternion
  aocs_manager_.quaternion_obs_i2b = QUATERNION_make_unit();
  aocs_manager_.quaternion_est_i2b = QUATERNION_make_unit();
  aocs_manager_.quaternion_target_i2t = QUATERNION_make_unit();
  aocs_manager_.quaternion_error_b2t = QUATERNION_make_unit();
  aocs_manager_.limit_maneuver_angle_rad = PHYSICAL_CONST_degree_to_radian(30.0f);
  // 太陽方向
  const float kSqrtOneThird = 0.57735f; // ノルム1になる値
  VECTOR3_initialize(aocs_manager_.sun_vec_obs_body,    kSqrtOneThird);
  VECTOR3_initialize(aocs_manager_.sun_vec_est_body,    kSqrtOneThird);
  VECTOR3_initialize(aocs_manager_.sun_vec_est_eci,     kSqrtOneThird);
  VECTOR3_initialize(aocs_manager_.sun_vec_target_body, kSqrtOneThird);
  aocs_manager_.sun_vec_error_rad = 0.0f;
  // 磁場ベクトル
  VECTOR3_initialize(aocs_manager_.mag_vec_obs_body_nT, 0.0f);
  VECTOR3_initialize(aocs_manager_.mag_vec_est_body_nT, 0.0f);
  VECTOR3_initialize(aocs_manager_.mag_vec_est_eci_nT,  0.0f);
  // トルク
  VECTOR3_initialize(aocs_manager_.internal_torque_target_body_Nm, 0.0f);
  VECTOR3_initialize(aocs_manager_.external_torque_target_body_Nm, 0.0f);
  VECTOR3_initialize(aocs_manager_.torque_est_body_Nm,             0.0f);
  VECTOR3_initialize(aocs_manager_.internal_torque_max_body_Nm,    5e-3f);  // これで進めるが、必要になったら修正してよい
  VECTOR3_initialize(aocs_manager_.external_torque_max_body_Nm,    3e-4f);  // これで進めるが、必要になったら修正してよい
  VECTOR3_initialize(aocs_manager_.constant_torque_body_Nm,        0.0f);
  aocs_manager_.constant_torque_permission = AOCS_MANAGER_CONSTANT_TORQUE_DISABLE;
  // 磁気モーメント
  VECTOR3_initialize(aocs_manager_.mag_moment_target_body_Am2, 0.0f);
  // MTQ情報
  for (int mtq_idx = 0; mtq_idx < AOCS_MANAGER_NUM_OF_MTQ; mtq_idx++)
  {
    VECTOR3_copy(aocs_manager_.mtq_magnetic_moment_direction_matrix[mtq_idx],
                mtq_seiren_driver[mtq_idx]->info.magnetic_moment_direction_b);
  }
  AOCS_MANAGER_set_mtq_magnetic_moment_direction_matrix(aocs_manager_.mtq_magnetic_moment_direction_matrix);
  aocs_manager_.mtq_output_state = AOCS_MANAGER_MTQ_OUTPUT_STATE_OFF;
  aocs_manager_.mag_exclusive_state = AOCS_MANAGER_MAG_EXCLUSIVE_STATE_ACTIVE;
  // RW情報
  float rw_rotation_direction_matrix[AOCS_MANAGER_NUM_OF_RW][PHYSICAL_CONST_THREE_DIM];
  for (int rw_idx = 0; rw_idx < AOCS_MANAGER_NUM_OF_RW; rw_idx++)
  {
    // 方向ベクトル情報のマスターはDI_RWとなっている点に注意
    VECTOR3_copy(rw_rotation_direction_matrix[rw_idx], rw0003_driver[rw_idx]->info.rotation_direction_b);
    // 慣性モーメント情報のマスターはDriverとなっている点に注意 TODO_L: 可変にするか要検討
    aocs_manager_.rw_moment_of_inertia_kgm2[rw_idx] = RW0003_kMomentOfInertiaKgm2;
  }
  AOCS_MANAGER_set_rw_rotation_direction_matrix(rw_rotation_direction_matrix);
  float rw_angular_velocity_rad_s[AOCS_MANAGER_NUM_OF_RW];
  VECTOR3_initialize(rw_angular_velocity_rad_s, 0.0f);
  AOCS_MANAGER_set_rw_angular_velocity_rad_s(rw_angular_velocity_rad_s);
  // 時刻
  aocs_manager_.current_gps_time_obs_msec = 0;
  aocs_manager_.current_gps_time_obs_week = 0;
  aocs_manager_.obct_gps_time_obs = OBCT_create(0, 0, 0);
  // aocs_manager_.current_gps_time_est_msec = 0;
  // aocs_manager_.current_gps_time_est_week = 0;
  aocs_manager_.obct_gps_time_est = OBCT_create(0, 0, 0);
  aocs_manager_.obct_reference  = OBCT_create(0, 0, 0);
  aocs_manager_.reference_jday = ORBIT_PARAMETERS_reference_jday;
  // センサ状態
  aocs_manager_.sun_visibility = AOCS_MANAGER_SUN_INVISIBILE;
  aocs_manager_.gps_visibility = AOCS_MANAGER_GPS_INVISIBILE;
  aocs_manager_.stt_availability = AOCS_MANAGER_STT_UNAVAILABLE;

  // 軌道
  VECTOR3_initialize_double(aocs_manager_.sat_pos_obs_ecef_m,    0.0);
  VECTOR3_initialize_double(aocs_manager_.sat_pos_obs_eci_m,     0.0);
  VECTOR3_initialize_double(aocs_manager_.sat_pos_est_ecef_m,    0.0);
  VECTOR3_initialize_double(aocs_manager_.sat_pos_est_eci_m,     0.0);
  VECTOR3_initialize_double(aocs_manager_.sat_pos_est_lla_rad_m, 0.0);
  VECTOR3_initialize_double(aocs_manager_.sat_vel_obs_ecef_m_s,  0.0);
  VECTOR3_initialize_double(aocs_manager_.sat_vel_obs_eci_m_s,   0.0);
  VECTOR3_initialize_double(aocs_manager_.sat_vel_est_ecef_m_s,  0.0);
  VECTOR3_initialize_double(aocs_manager_.sat_vel_est_eci_m_s,   0.0);

  // 各種フラグ
  aocs_manager_.flags_for_tlm = 0;
}

static void APP_AOCS_MANAGER_exec_(void)
{
  // TLM生成用処理
  aocs_manager_.flags_for_tlm = APP_AOCS_MANAGER_bit_flag_control_(aocs_manager_.flags_for_tlm,
                                                                   aocs_manager_.sun_visibility == AOCS_MANAGER_SUN_VISIBLE,
                                                                   1);
  aocs_manager_.flags_for_tlm = APP_AOCS_MANAGER_bit_flag_control_(aocs_manager_.flags_for_tlm,
                                                                   aocs_manager_.gps_visibility == AOCS_MANAGER_GPS_VISIBLE,
                                                                   2);
  aocs_manager_.flags_for_tlm = APP_AOCS_MANAGER_bit_flag_control_(aocs_manager_.flags_for_tlm,
                                                                   aocs_manager_.stt_availability == AOCS_MANAGER_STT_AVAILABLE,
                                                                   3);
  aocs_manager_.flags_for_tlm = APP_AOCS_MANAGER_bit_flag_control_(aocs_manager_.flags_for_tlm,
                                                                   unloading->unloading_state[RW0003_IDX_ON_X] != APP_UNLOADING_STATE_OFF,
                                                                   4);
  aocs_manager_.flags_for_tlm = APP_AOCS_MANAGER_bit_flag_control_(aocs_manager_.flags_for_tlm,
                                                                   unloading->unloading_state[RW0003_IDX_ON_Y] != APP_UNLOADING_STATE_OFF,
                                                                   5);
  aocs_manager_.flags_for_tlm = APP_AOCS_MANAGER_bit_flag_control_(aocs_manager_.flags_for_tlm,
                                                                   unloading->unloading_state[RW0003_IDX_ON_Z] != APP_UNLOADING_STATE_OFF,
                                                                   6);
  aocs_manager_.flags_for_tlm = APP_AOCS_MANAGER_bit_flag_control_(aocs_manager_.flags_for_tlm,
                                                                   aocs_mode_manager->is_control_error_small == 1,
                                                                   7);

}

// 衛星特性
AOCS_MANAGER_ERROR AOCS_MANAGER_set_mass_sc_kg(const float mass_sc_kg)
{
  if (mass_sc_kg <= 0.0f) return AOCS_MANAGER_ERROR_NG;
  aocs_manager_.mass_sc_kg = mass_sc_kg;
  return AOCS_MANAGER_ERROR_OK;
}

AOCS_MANAGER_ERROR AOCS_MANAGER_set_rmm_sc_body_Am2(const float rmm_sc_body_Am2[PHYSICAL_CONST_THREE_DIM])
{
  VECTOR3_copy(aocs_manager_.rmm_sc_body_Am2, rmm_sc_body_Am2);
  return AOCS_MANAGER_ERROR_OK;
}

AOCS_MANAGER_ERROR AOCS_MANAGER_set_inertia_tensor_sc_body_kgm2(const float inertia_tensor_sc_body_kgm2[][PHYSICAL_CONST_THREE_DIM])
{
  // 対角成分確認
  for (int i = 0; i < PHYSICAL_CONST_THREE_DIM; i++)
  {
    if (inertia_tensor_sc_body_kgm2[i][i] <= 0.0f)
    {
      return AOCS_MANAGER_ERROR_NG;
    }
  }
  // 対称行列確認
  if (MATRIX33_is_symmetry(inertia_tensor_sc_body_kgm2) != C2A_MATH_ERROR_OK) return AOCS_MANAGER_ERROR_NG;

  MATRIX33_copy(aocs_manager_.inertia_tensor_sc_body_kgm2, inertia_tensor_sc_body_kgm2);
  return AOCS_MANAGER_ERROR_OK;
}

// 角速度
AOCS_MANAGER_ERROR AOCS_MANAGER_set_ang_vel_obs_body_rad_s(const float ang_vel_obs_body_rad_s[PHYSICAL_CONST_THREE_DIM])
{
  VECTOR3_copy(aocs_manager_.ang_vel_obs_body_rad_s, ang_vel_obs_body_rad_s);
  return AOCS_MANAGER_ERROR_OK;
}

AOCS_MANAGER_ERROR AOCS_MANAGER_set_ang_vel_est_body_rad_s(const float ang_vel_est_body_rad_s[PHYSICAL_CONST_THREE_DIM])
{
  VECTOR3_copy(aocs_manager_.ang_vel_est_body_rad_s, ang_vel_est_body_rad_s);
  // 保有角運動量の計算
  GYROSCOPIC_EFFECT_calculate_total_momentum(aocs_manager_.ang_mom_est_body_Nms,
                                             aocs_manager_.inertia_tensor_sc_body_kgm2,
                                             aocs_manager_.ang_vel_est_body_rad_s,
                                             aocs_manager_.rw_angular_momentum_body_Nms);

  return AOCS_MANAGER_ERROR_OK;
}

AOCS_MANAGER_ERROR AOCS_MANAGER_set_ang_vel_target_body_rad_s(const float ang_vel_target_body_rad_s[PHYSICAL_CONST_THREE_DIM])
{
  float ang_vel_norm_rad_s = VECTOR3_norm(ang_vel_target_body_rad_s);
  if (ang_vel_norm_rad_s > aocs_manager_.limit_angular_velocity_rad_s)  return AOCS_MANAGER_ERROR_NG;
  VECTOR3_copy(aocs_manager_.ang_vel_target_body_rad_s, ang_vel_target_body_rad_s);
  return AOCS_MANAGER_ERROR_OK;
}

AOCS_MANAGER_ERROR AOCS_MANAGER_set_ang_vel_error_body_rad_s(const float ang_vel_error_body_rad_s[PHYSICAL_CONST_THREE_DIM])
{
  VECTOR3_copy(aocs_manager_.ang_vel_error_body_rad_s, ang_vel_error_body_rad_s);
  return AOCS_MANAGER_ERROR_OK;
}

AOCS_MANAGER_ERROR AOCS_MANAGER_set_limit_angular_velocity_rad_s(const float limit_angular_velocity_rad_s)
{
  if (limit_angular_velocity_rad_s < 0.0f) return AOCS_MANAGER_ERROR_NG;
  aocs_manager_.limit_angular_velocity_rad_s = limit_angular_velocity_rad_s;
  return AOCS_MANAGER_ERROR_OK;
}

// Quaternion
AOCS_MANAGER_ERROR AOCS_MANAGER_set_quaternion_obs_i2b(const Quaternion quaternion_obs_i2b)
{

  aocs_manager_.quaternion_obs_i2b = quaternion_obs_i2b;
  return AOCS_MANAGER_ERROR_OK;
}

AOCS_MANAGER_ERROR AOCS_MANAGER_set_quaternion_est_i2b(const Quaternion quaternion_est_i2b)
{
  aocs_manager_.quaternion_est_i2b = quaternion_est_i2b;
  return AOCS_MANAGER_ERROR_OK;
}

AOCS_MANAGER_ERROR AOCS_MANAGER_set_quaternion_target_i2t(const Quaternion quaternion_target_i2t)
{
  Quaternion q_diff = QUATERNION_divide(aocs_manager_.quaternion_est_i2b, quaternion_target_i2t);
  if (q_diff.scalar_part < 0.0f) q_diff.scalar_part = q_diff.scalar_part * -1.0f;
  float angle_rad = 2.0f * acosf(q_diff.scalar_part); // 必ず正の値
  if (angle_rad > aocs_manager_.limit_maneuver_angle_rad)  return AOCS_MANAGER_ERROR_NG;

  aocs_manager_.quaternion_target_i2t = quaternion_target_i2t;
  return AOCS_MANAGER_ERROR_OK;
}

AOCS_MANAGER_ERROR AOCS_MANAGER_set_quaternion_error_b2t(const Quaternion quaternion_error_b2t)
{
  aocs_manager_.quaternion_error_b2t = quaternion_error_b2t;
  return AOCS_MANAGER_ERROR_OK;
}

AOCS_MANAGER_ERROR AOCS_MANAGER_set_limit_maneuver_angle_rad(const float limit_maneuver_angle_rad)
{
  if (limit_maneuver_angle_rad < 0.0) return AOCS_MANAGER_ERROR_NG;
  if (limit_maneuver_angle_rad > MATH_CONST_PI) return AOCS_MANAGER_ERROR_NG;
  aocs_manager_.limit_maneuver_angle_rad = limit_maneuver_angle_rad;
  return AOCS_MANAGER_ERROR_OK;
}


// 太陽方向
AOCS_MANAGER_ERROR AOCS_MANAGER_set_sun_vec_obs_body(const float sun_vec_obs_body[PHYSICAL_CONST_THREE_DIM])
{
  if (VECTOR3_is_normalized(sun_vec_obs_body) != C2A_MATH_ERROR_OK) return AOCS_MANAGER_ERROR_NG;

  VECTOR3_copy(aocs_manager_.sun_vec_obs_body, sun_vec_obs_body);
  return AOCS_MANAGER_ERROR_OK;
}

AOCS_MANAGER_ERROR AOCS_MANAGER_set_sun_vec_est_body(const float sun_vec_est_body[PHYSICAL_CONST_THREE_DIM])
{
  if (VECTOR3_is_normalized(sun_vec_est_body) != C2A_MATH_ERROR_OK) return AOCS_MANAGER_ERROR_NG;

  VECTOR3_copy(aocs_manager_.sun_vec_est_body, sun_vec_est_body);
  return AOCS_MANAGER_ERROR_OK;
}

AOCS_MANAGER_ERROR AOCS_MANAGER_set_sun_vec_est_eci(const float sun_vec_est_eci[PHYSICAL_CONST_THREE_DIM])
{
  if (VECTOR3_is_normalized(sun_vec_est_eci) != C2A_MATH_ERROR_OK) return AOCS_MANAGER_ERROR_NG;

  VECTOR3_copy(aocs_manager_.sun_vec_est_eci, sun_vec_est_eci);
  return AOCS_MANAGER_ERROR_OK;
}

AOCS_MANAGER_ERROR AOCS_MANAGER_set_sun_vec_target_body(const float sun_vec_target_body[PHYSICAL_CONST_THREE_DIM])
{
  if (VECTOR3_is_normalized(sun_vec_target_body) != C2A_MATH_ERROR_OK) return AOCS_MANAGER_ERROR_NG;

  VECTOR3_copy(aocs_manager_.sun_vec_target_body, sun_vec_target_body);
  return AOCS_MANAGER_ERROR_OK;
}

AOCS_MANAGER_ERROR AOCS_MANAGER_set_sun_vec_error_rad(const float sun_vec_error_rad)
{
  aocs_manager_.sun_vec_error_rad = C2A_MATH_normalize_zero_2pi(sun_vec_error_rad);
  return AOCS_MANAGER_ERROR_OK;
}

// 磁場ベクトル
AOCS_MANAGER_ERROR AOCS_MANAGER_set_mag_vec_obs_body_nT(const float mag_vec_obs_body_nT[PHYSICAL_CONST_THREE_DIM])
{
  VECTOR3_copy(aocs_manager_.mag_vec_obs_body_nT, mag_vec_obs_body_nT);
  return AOCS_MANAGER_ERROR_OK;
}

AOCS_MANAGER_ERROR AOCS_MANAGER_set_mag_vec_est_body_nT(const float mag_vec_est_body_nT[PHYSICAL_CONST_THREE_DIM])
{
  VECTOR3_copy(aocs_manager_.mag_vec_est_body_nT, mag_vec_est_body_nT);
  return AOCS_MANAGER_ERROR_OK;
}

AOCS_MANAGER_ERROR AOCS_MANAGER_set_mag_vec_est_eci_nT(const float mag_vec_est_eci_nT[PHYSICAL_CONST_THREE_DIM])
{
  VECTOR3_copy(aocs_manager_.mag_vec_est_eci_nT, mag_vec_est_eci_nT);
  return AOCS_MANAGER_ERROR_OK;
}

// トルク
AOCS_MANAGER_ERROR AOCS_MANAGER_set_internal_torque_target_body_Nm(const float internal_torque_target_body_Nm[PHYSICAL_CONST_THREE_DIM])
{
  float clipped_value[PHYSICAL_CONST_THREE_DIM];
  for (uint8_t i = 0; i < PHYSICAL_CONST_THREE_DIM; i++)
  {
    clipped_value[i] = C2A_MATH_clip_value_abs(internal_torque_target_body_Nm[i],
                                               aocs_manager_.internal_torque_max_body_Nm[i]);
  }
  VECTOR3_copy(aocs_manager_.internal_torque_target_body_Nm, clipped_value);
  return AOCS_MANAGER_ERROR_OK;
}

AOCS_MANAGER_ERROR AOCS_MANAGER_set_external_torque_target_body_Nm(const float external_torque_target_body_Nm[PHYSICAL_CONST_THREE_DIM])
{
  float clipped_value[PHYSICAL_CONST_THREE_DIM];
  for (uint8_t i = 0; i < PHYSICAL_CONST_THREE_DIM; i++)
  {
    clipped_value[i] = C2A_MATH_clip_value_abs(external_torque_target_body_Nm[i],
                                               aocs_manager_.external_torque_max_body_Nm[i]);
  }
  VECTOR3_copy(aocs_manager_.external_torque_target_body_Nm, clipped_value);
  return AOCS_MANAGER_ERROR_OK;
}

AOCS_MANAGER_ERROR AOCS_MANAGER_set_internal_torque_max_body_Nm(const float internal_torque_max_body_Nm[PHYSICAL_CONST_THREE_DIM])
{
  for (uint8_t i = 0; i < PHYSICAL_CONST_THREE_DIM; i++)
  {
    if (internal_torque_max_body_Nm[i] < 0.0f) return AOCS_MANAGER_ERROR_NG;
  }
  VECTOR3_copy(aocs_manager_.internal_torque_max_body_Nm, internal_torque_max_body_Nm);
  return AOCS_MANAGER_ERROR_OK;
}

AOCS_MANAGER_ERROR AOCS_MANAGER_set_external_torque_max_body_Nm(const float external_torque_max_body_Nm[PHYSICAL_CONST_THREE_DIM])
{
  for (uint8_t i = 0; i < PHYSICAL_CONST_THREE_DIM; i++)
  {
    if (external_torque_max_body_Nm[i] < 0.0f) return AOCS_MANAGER_ERROR_NG;
  }
  VECTOR3_copy(aocs_manager_.external_torque_max_body_Nm, external_torque_max_body_Nm);
  return AOCS_MANAGER_ERROR_OK;
}

AOCS_MANAGER_ERROR AOCS_MANAGER_set_torque_est_body_Nm(const float torque_est_body_Nm[PHYSICAL_CONST_THREE_DIM])
{
  VECTOR3_copy(aocs_manager_.torque_est_body_Nm, torque_est_body_Nm);
  return AOCS_MANAGER_ERROR_OK;
}

AOCS_MANAGER_ERROR AOCS_MANAGER_set_constant_torque_body_Nm(const float constant_torque_body_Nm[PHYSICAL_CONST_THREE_DIM])
{
  for (uint8_t i = 0; i < PHYSICAL_CONST_THREE_DIM; i++)
  {
    C2A_MATH_ERROR ret = C2A_MATH_check_abs_range_violation(constant_torque_body_Nm[i],
                                                            aocs_manager_.internal_torque_max_body_Nm[i]);
    if (ret == C2A_MATH_ERROR_RANGE_OVER) return AOCS_MANAGER_ERROR_NG;
  }
  VECTOR3_copy(aocs_manager_.constant_torque_body_Nm, constant_torque_body_Nm);
  return AOCS_MANAGER_ERROR_OK;
}

AOCS_MANAGER_ERROR AOCS_MANAGER_set_constant_torque_permission(const AOCS_MANAGER_CONSTANT_TORQUE_PERMISSION constant_torque_permission)
{
  aocs_manager_.constant_torque_permission = constant_torque_permission;
  return AOCS_MANAGER_ERROR_OK;
}

AOCS_MANAGER_ERROR AOCS_MANAGER_set_mag_moment_target_body_Am2(const float mag_moment_target_body_Am2[PHYSICAL_CONST_THREE_DIM])
{
  VECTOR3_copy(aocs_manager_.mag_moment_target_body_Am2, mag_moment_target_body_Am2);
  return AOCS_MANAGER_ERROR_OK;
}

AOCS_MANAGER_ERROR AOCS_MANAGER_set_mtq_magnetic_moment_direction_matrix(
  const float mtq_magnetic_moment_direction_matrix[AOCS_MANAGER_NUM_OF_MTQ][PHYSICAL_CONST_THREE_DIM])
{
  for (int idx = 0; idx < AOCS_MANAGER_NUM_OF_MTQ; idx++)
  {
    VECTOR3_copy(aocs_manager_.mtq_magnetic_moment_direction_matrix[idx], mtq_magnetic_moment_direction_matrix[idx]);
  }
  // TODO_L: MTQが4つ以上になったら修正が必要
  C2A_MATH_ERROR ret = MATRIX33_inverse(aocs_manager_.mtq_distribution_matrix, aocs_manager_.mtq_magnetic_moment_direction_matrix);
  if (ret != C2A_MATH_ERROR_OK) return AOCS_MANAGER_ERROR_NG;

  return AOCS_MANAGER_ERROR_OK;
}

AOCS_MANAGER_ERROR AOCS_MANAGER_set_mtq_output_state(const AOCS_MANAGER_MTQ_OUTPUT_STATE mtq_output_state)
{
  aocs_manager_.mtq_output_state = mtq_output_state;
  return AOCS_MANAGER_ERROR_OK;
}

AOCS_MANAGER_ERROR AOCS_MANAGER_set_mag_exclusive_state(const AOCS_MANAGER_MAG_EXCLUSIVE_STATE mag_exclusive_state)
{
  aocs_manager_.mag_exclusive_state = mag_exclusive_state;
  return AOCS_MANAGER_ERROR_OK;
}

AOCS_MANAGER_ERROR AOCS_MANAGER_set_rw_angular_velocity_rad_s(const float rw_angular_velocity_rad_s[AOCS_MANAGER_NUM_OF_RW])
{
  VECTOR3_copy(aocs_manager_.rw_angular_velocity_rad_s, rw_angular_velocity_rad_s);
  // 角運動量の計算
  for (int idx = 0; idx < AOCS_MANAGER_NUM_OF_RW; idx++)
  {
    aocs_manager_.rw_angular_momentum_Nms[idx] = aocs_manager_.rw_angular_velocity_rad_s[idx] * aocs_manager_.rw_moment_of_inertia_kgm2[idx];
  }
  // 角運動量の座標変換 TODO_L: RWが4つになったら修正が必要
  MATRIX33_multiply_matrix_vector(aocs_manager_.rw_angular_momentum_body_Nms,
                                aocs_manager_.rw_rotation_direction_matrix,
                                aocs_manager_.rw_angular_momentum_Nms);

  return AOCS_MANAGER_ERROR_OK;
}

AOCS_MANAGER_ERROR AOCS_MANAGER_set_rw_rotation_direction_matrix(
  const float rw_rotation_direction_matrix[AOCS_MANAGER_NUM_OF_RW][PHYSICAL_CONST_THREE_DIM])
{
  for (int idx = 0; idx < AOCS_MANAGER_NUM_OF_RW; idx++)
  {
    VECTOR3_copy(aocs_manager_.rw_rotation_direction_matrix[idx], rw_rotation_direction_matrix[idx]);
  }
  // TODO_L: RWが4つ以上になったら修正が必要
  C2A_MATH_ERROR ret = MATRIX33_inverse(aocs_manager_.rw_distribution_matrix, aocs_manager_.rw_rotation_direction_matrix);
  if (ret != C2A_MATH_ERROR_OK) return AOCS_MANAGER_ERROR_NG;

  return AOCS_MANAGER_ERROR_OK;
}

// 時刻
AOCS_MANAGER_ERROR AOCS_MANAGER_set_current_gps_time_obs(const uint32_t current_gps_time_obs_msec,
                                                         const uint16_t current_gps_time_obs_week,
                                                         const ObcTime  obct_gps_time_obs)
{
  aocs_manager_.current_gps_time_obs_msec = current_gps_time_obs_msec;
  aocs_manager_.current_gps_time_obs_week = current_gps_time_obs_week;
  aocs_manager_.obct_gps_time_obs = obct_gps_time_obs;
  return AOCS_MANAGER_ERROR_OK;
}

// 22_08_24時点で使用箇所がないため，一旦コメントアウト
// AOCS_MANAGER_ERROR AOCS_MANAGER_set_current_gps_time_est(const uint32_t current_gps_time_est_msec,
//                                                          const uint16_t current_gps_time_est_week,
//                                                          const ObcTime  obct_gps_time_est)
// {
//   aocs_manager_.current_gps_time_est_msec = current_gps_time_est_msec;
//   aocs_manager_.current_gps_time_est_week = current_gps_time_est_week;
//   aocs_manager_.obct_gps_time_est = obct_gps_time_est;
//   return AOCS_MANAGER_ERROR_OK;
// }

AOCS_MANAGER_ERROR AOCS_MANAGER_set_reference_jday(const double reference_jday, const ObcTime obct_reference)
{
  if (reference_jday <= 0.0) return AOCS_MANAGER_ERROR_NG;

  aocs_manager_.reference_jday = reference_jday;
  aocs_manager_.obct_reference = obct_reference;
  return AOCS_MANAGER_ERROR_OK;
}

// センサ状態
AOCS_MANAGER_ERROR AOCS_MANAGER_set_sun_visibility(const AOCS_MANAGER_SUN_VISIBILITY sun_visibility)
{
  aocs_manager_.sun_visibility = sun_visibility;
  return AOCS_MANAGER_ERROR_OK;
}

AOCS_MANAGER_ERROR AOCS_MANAGER_set_stt_availability(const AOCS_MANAGER_STT_AVAILABILITY stt_availibility)
{
  aocs_manager_.stt_availability = stt_availibility;
  return AOCS_MANAGER_ERROR_OK;
}

AOCS_MANAGER_ERROR AOCS_MANAGER_set_gps_visibility(const AOCS_MANAGER_GPS_VISIBILITY gps_visibility)
{
  aocs_manager_.gps_visibility = gps_visibility;
  return AOCS_MANAGER_ERROR_OK;
}

// 軌道
AOCS_MANAGER_ERROR AOCS_MANAGER_set_sat_pos_vel_obs_ecef_m(const double sat_pos_obs_ecef_m  [PHYSICAL_CONST_THREE_DIM],
                                                           const double sat_vel_obs_ecef_m_s[PHYSICAL_CONST_THREE_DIM])
{
  VECTOR3_copy_double(aocs_manager_.sat_pos_obs_ecef_m, sat_pos_obs_ecef_m);
  VECTOR3_copy_double(aocs_manager_.sat_vel_obs_ecef_m_s, sat_vel_obs_ecef_m_s);
  return AOCS_MANAGER_ERROR_OK;
}

AOCS_MANAGER_ERROR AOCS_MANAGER_set_sat_pos_vel_obs_eci_m(const double sat_pos_obs_eci_m[PHYSICAL_CONST_THREE_DIM],
                                                          const double sat_vel_obs_eci_m_s[PHYSICAL_CONST_THREE_DIM])
{
  VECTOR3_copy_double(aocs_manager_.sat_pos_obs_eci_m, sat_pos_obs_eci_m);
  VECTOR3_copy_double(aocs_manager_.sat_vel_obs_eci_m_s, sat_vel_obs_eci_m_s);
  return AOCS_MANAGER_ERROR_OK;
}

AOCS_MANAGER_ERROR AOCS_MANAGER_set_sat_pos_vel_est_ecef_m(const double sat_pos_est_ecef_m  [PHYSICAL_CONST_THREE_DIM],
                                                           const double sat_vel_est_ecef_m_s[PHYSICAL_CONST_THREE_DIM])
{
  VECTOR3_copy_double(aocs_manager_.sat_pos_est_ecef_m, sat_pos_est_ecef_m);
  VECTOR3_copy_double(aocs_manager_.sat_vel_est_ecef_m_s, sat_vel_est_ecef_m_s);
  return AOCS_MANAGER_ERROR_OK;
}

AOCS_MANAGER_ERROR AOCS_MANAGER_set_sat_pos_vel_est_eci_m(const double sat_pos_est_eci_m  [PHYSICAL_CONST_THREE_DIM],
                                                          const double sat_vel_est_eci_m_s[PHYSICAL_CONST_THREE_DIM])
{
  VECTOR3_copy_double(aocs_manager_.sat_pos_est_eci_m, sat_pos_est_eci_m);
  VECTOR3_copy_double(aocs_manager_.sat_vel_est_eci_m_s, sat_vel_est_eci_m_s);
  return AOCS_MANAGER_ERROR_OK;
}

AOCS_MANAGER_ERROR AOCS_MANAGER_set_sat_pos_est_lla_rad_m(const double sat_pos_est_lla_rad_m[PHYSICAL_CONST_THREE_DIM])
{
  VECTOR3_copy_double(aocs_manager_.sat_pos_est_lla_rad_m, sat_pos_est_lla_rad_m);
  return AOCS_MANAGER_ERROR_OK;
}

// 座標変換
AOCS_MANAGER_ERROR AOCS_MANAGER_set_dcm_eci_to_ecef(const double dcm_eci_to_ecef[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM])
{
  MATRIX33_copy_double(aocs_manager_.dcm_eci_to_ecef, dcm_eci_to_ecef);
  return AOCS_MANAGER_ERROR_OK;
}


// 計算関数
AOCS_MANAGER_ERROR AOCS_MANAGER_add_constant_torque_body_nm(float out_torque_body_Nm[PHYSICAL_CONST_THREE_DIM],
                                                            const float in_torque_body_Nm[PHYSICAL_CONST_THREE_DIM])
{
  if (aocs_manager_.constant_torque_permission != AOCS_MANAGER_CONSTANT_TORQUE_ENABLE)
  {
    VECTOR3_copy(out_torque_body_Nm, in_torque_body_Nm);
    return AOCS_MANAGER_ERROR_NG;
  }
  VECTOR3_add(out_torque_body_Nm, in_torque_body_Nm, aocs_manager_.constant_torque_body_Nm);
  return AOCS_MANAGER_ERROR_OK;
}

AOCS_MANAGER_ERROR AOCS_MANAGER_add_gyroscopic_torque_body_nm(float out_torque_body_Nm[PHYSICAL_CONST_THREE_DIM],
                                                              const float in_torque_body_Nm[PHYSICAL_CONST_THREE_DIM])
{
  float gyroscopic_effect_trq_Nm[PHYSICAL_CONST_THREE_DIM];
  GYROSCOPIC_EFFECT_calculate_gyroscopic_torque(gyroscopic_effect_trq_Nm,
                                                aocs_manager->ang_vel_est_body_rad_s,
                                                aocs_manager->ang_mom_est_body_Nms);

  VECTOR3_add(out_torque_body_Nm, in_torque_body_Nm, gyroscopic_effect_trq_Nm);

  return AOCS_MANAGER_ERROR_OK;
}


// Static functions
static uint8_t APP_AOCS_MANAGER_bit_flag_control_(const uint8_t flags_in, const uint8_t flag_on, const uint8_t bit)
{
  uint8_t bit_field = 0x01 << (bit - 1);
  uint8_t flags_out;
  if (flag_on)
  {
    flags_out = flags_in | bit_field;
  }
  else
  {
    flags_out = flags_in & ~bit_field;
  }
  return flags_out;
}


// Command functions
// Structure
CCP_CmdRet Cmd_APP_AOCS_MANAGER_SET_MASS(const CommonCmdPacket* packet)
{
  const uint8_t* param = CCP_get_param_head(packet);
  float mass_sc_kg;
  ENDIAN_memcpy(&mass_sc_kg, param, sizeof(float));

  AOCS_MANAGER_ERROR ret = AOCS_MANAGER_set_mass_sc_kg(mass_sc_kg);
  if (ret != AOCS_MANAGER_ERROR_OK) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);

  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

CCP_CmdRet Cmd_APP_AOCS_MANAGER_SET_INERTIA_TENSOR(const CommonCmdPacket* packet)
{
  const uint8_t* param = CCP_get_param_head(packet);
  int param_id = 0;
  float inertia_tensor_sc_body_kgm2[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM];

  for (int i = 0; i < PHYSICAL_CONST_THREE_DIM; i++)
  {
    ENDIAN_memcpy(&inertia_tensor_sc_body_kgm2[i][i], param + param_id, sizeof(float));
    param_id += (int)sizeof(float);
  }

  ENDIAN_memcpy(&inertia_tensor_sc_body_kgm2[0][1], param + param_id, sizeof(float));
  param_id += (int)sizeof(float);
  inertia_tensor_sc_body_kgm2[1][0] = inertia_tensor_sc_body_kgm2[0][1];

  ENDIAN_memcpy(&inertia_tensor_sc_body_kgm2[0][2], param + param_id, sizeof(float));
  param_id += (int)sizeof(float);
  inertia_tensor_sc_body_kgm2[2][0] = inertia_tensor_sc_body_kgm2[0][2];

  ENDIAN_memcpy(&inertia_tensor_sc_body_kgm2[1][2], param + param_id, sizeof(float));
  param_id += (int)sizeof(float);
  inertia_tensor_sc_body_kgm2[2][1] = inertia_tensor_sc_body_kgm2[1][2];

  AOCS_MANAGER_ERROR ret = AOCS_MANAGER_set_inertia_tensor_sc_body_kgm2(inertia_tensor_sc_body_kgm2);
  if (ret != AOCS_MANAGER_ERROR_OK) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);

  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

CCP_CmdRet Cmd_APP_AOCS_MANAGER_SET_RMM(const CommonCmdPacket* packet)
{
  const uint8_t* param = CCP_get_param_head(packet);
  int param_id = 0;
  float rmm_sc_body_Am2[PHYSICAL_CONST_THREE_DIM];

  for (int i = 0; i < PHYSICAL_CONST_THREE_DIM; i++)
  {
    ENDIAN_memcpy(&rmm_sc_body_Am2[i], param + param_id, sizeof(float));
    param_id += (int)sizeof(float);
  }

  AOCS_MANAGER_ERROR ret = AOCS_MANAGER_set_rmm_sc_body_Am2(rmm_sc_body_Am2);
  if (ret != AOCS_MANAGER_ERROR_OK) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);

  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

// Target
CCP_CmdRet Cmd_APP_AOCS_MANAGER_SET_TARGET_QUATERNION(const CommonCmdPacket* packet)
{
  const uint8_t* param = CCP_get_param_head(packet);
  int param_id = 0;

  float quaternion_target_i2t_array[PHYSICAL_CONST_QUATERNION_DIM];
  for (int i = 0; i < PHYSICAL_CONST_QUATERNION_DIM; i++)
  {
    ENDIAN_memcpy(&quaternion_target_i2t_array[i], param + param_id, sizeof(float));
    param_id += (int)sizeof(float);
  }

  Quaternion quaternion_target_i2t;
  C2A_MATH_ERROR ret_c2a_math = QUATERNION_make_from_array(&quaternion_target_i2t,
                                                           quaternion_target_i2t_array,
                                                           QUATERNION_SCALAR_POSITION_LAST);
  if (ret_c2a_math != C2A_MATH_ERROR_OK) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);

  AOCS_MANAGER_ERROR ret_aocs_manager = AOCS_MANAGER_set_quaternion_target_i2t(quaternion_target_i2t);
  if (ret_aocs_manager != AOCS_MANAGER_ERROR_OK) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_CONTEXT);

  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

CCP_CmdRet Cmd_APP_AOCS_MANAGER_SET_TARGET_QUATERNION_FROM_CURRENT_ATTITUDE(const CommonCmdPacket* packet)
{
  const uint8_t* param = CCP_get_param_head(packet);
  int param_id = 0;

  MATRIX33_ROTATION_AXIS axis = (MATRIX33_ROTATION_AXIS)param[0];
  param_id += (int)sizeof(uint8_t);
  if (axis >= MATRIX33_ROTATION_MAX) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);

  float angle_deg = 0.0f;
  ENDIAN_memcpy(&angle_deg, param + param_id, sizeof(float));
  param_id += (int)sizeof(float);
  if ((angle_deg < -180.0f) || (180.0f < angle_deg)) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
  float angle_rad = PHYSICAL_CONST_degree_to_radian(angle_deg);

  Quaternion q_diff;
  switch (axis)
  {
  case MATRIX33_ROTATION_AXIS_X:
    q_diff = QUATERNION_make_x_rot(angle_rad);
    break;
  case MATRIX33_ROTATION_AXIS_Y:
    q_diff = QUATERNION_make_y_rot(angle_rad);
    break;
  case MATRIX33_ROTATION_AXIS_Z:
    q_diff = QUATERNION_make_z_rot(angle_rad);
    break;
  default:
    break;
  }

  Quaternion q_current = aocs_manager_.quaternion_est_i2b;
  Quaternion q_target = QUATERNION_product(q_current, q_diff);

  AOCS_MANAGER_ERROR ret = AOCS_MANAGER_set_quaternion_target_i2t(q_target);
  if (ret != AOCS_MANAGER_ERROR_OK) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_CONTEXT);

  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

CCP_CmdRet Cmd_APP_AOCS_MANAGER_SET_LIMIT_MANEUVER_ANGLE(const CommonCmdPacket* packet)
{
  const uint8_t* param = CCP_get_param_head(packet);
  float angle_deg;
  ENDIAN_memcpy(&angle_deg, param, sizeof(float));
  if ((angle_deg < 0.0f) || (180.0f < angle_deg)) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
  float angle_rad = PHYSICAL_CONST_degree_to_radian(angle_deg);

  AOCS_MANAGER_ERROR ret = AOCS_MANAGER_set_limit_maneuver_angle_rad(angle_rad);
  if (ret != AOCS_MANAGER_ERROR_OK) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);

  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

CCP_CmdRet Cmd_APP_AOCS_MANAGER_SET_LIMIT_ANGULAR_VELOCITY(const CommonCmdPacket* packet)
{
  float ang_vel_deg_s = CCP_get_param_from_packet(packet, 0, float);
  if (ang_vel_deg_s < 0.0f) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
  float ang_vel_rad_s = PHYSICAL_CONST_degree_to_radian(ang_vel_deg_s);

  AOCS_MANAGER_ERROR ret = AOCS_MANAGER_set_limit_angular_velocity_rad_s(ang_vel_rad_s);
  if (ret != AOCS_MANAGER_ERROR_OK) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);

  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

// Others
CCP_CmdRet Cmd_APP_AOCS_MANAGER_SET_REFERENCE_TIME(const CommonCmdPacket* packet)
{
  const uint8_t* param = CCP_get_param_head(packet);
  double reference_jday;
  ENDIAN_memcpy(&reference_jday, param, sizeof(double));

  ObcTime obct_reference = TMGR_get_master_clock();

  AOCS_MANAGER_ERROR ret = AOCS_MANAGER_set_reference_jday(reference_jday, obct_reference);
  if (ret != AOCS_MANAGER_ERROR_OK) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);

  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

CCP_CmdRet Cmd_APP_AOCS_MANAGER_SET_CONSTANT_TORQUE(const CommonCmdPacket* packet)
{
  const uint8_t* param = CCP_get_param_head(packet);
  int param_id = 0;
  float constant_torque[PHYSICAL_CONST_THREE_DIM] = {0.0f, 0.0f, 0.0f};

  for (int i = 0; i < PHYSICAL_CONST_THREE_DIM; i++)
  {
    ENDIAN_memcpy(&constant_torque[i], param + param_id, sizeof(float));
    param_id += (int)sizeof(float);
  }

  AOCS_MANAGER_ERROR ret = AOCS_MANAGER_set_constant_torque_body_Nm(constant_torque);
  if (ret != AOCS_MANAGER_ERROR_OK) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_CONTEXT);

  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

CCP_CmdRet Cmd_APP_AOCS_MANAGER_SET_CONSTANT_TORQUE_PERMISSION(const CommonCmdPacket* packet)
{
  const uint8_t* param = CCP_get_param_head(packet);
  AOCS_MANAGER_CONSTANT_TORQUE_PERMISSION permission_flag;

  permission_flag = (AOCS_MANAGER_CONSTANT_TORQUE_PERMISSION)param[0];
  if (permission_flag >= AOCS_MANAGER_CONSTANT_TORQUE_MAX) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);

  AOCS_MANAGER_ERROR ret = AOCS_MANAGER_set_constant_torque_permission(permission_flag);
  if (ret != AOCS_MANAGER_ERROR_OK) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);

  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

CCP_CmdRet Cmd_APP_AOCS_MANAGER_SET_MAG_EXCLUSIVE_STATE(const CommonCmdPacket* packet)
{
  const uint8_t* param = CCP_get_param_head(packet);
  AOCS_MANAGER_MAG_EXCLUSIVE_STATE mag_exclusive_state;

  mag_exclusive_state = (AOCS_MANAGER_MAG_EXCLUSIVE_STATE)param[0];
  if (mag_exclusive_state >= AOCS_MANAGER_MAG_EXCLUSIVE_STATE_MAX) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);

  AOCS_MANAGER_ERROR ret = AOCS_MANAGER_set_mag_exclusive_state(mag_exclusive_state);
  if (ret != AOCS_MANAGER_ERROR_OK) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);

  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

CCP_CmdRet Cmd_APP_AOCS_MANAGER_SET_MAX_IN_TORQUE(const CommonCmdPacket* packet)
{
  const uint8_t* param = CCP_get_param_head(packet);
  int param_id = 0;
  float max_torque[PHYSICAL_CONST_THREE_DIM];

  for (int i = 0; i < PHYSICAL_CONST_THREE_DIM; i++)
  {
    ENDIAN_memcpy(&max_torque[i], param + param_id, sizeof(float));
    param_id += (int)sizeof(float);
  }

  AOCS_MANAGER_ERROR ret = AOCS_MANAGER_set_internal_torque_max_body_Nm(max_torque);
  if (ret != AOCS_MANAGER_ERROR_OK) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);

  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

CCP_CmdRet Cmd_APP_AOCS_MANAGER_SET_MAX_EXT_TORQUE(const CommonCmdPacket* packet)
{
  const uint8_t* param = CCP_get_param_head(packet);
  int param_id = 0;
  float max_torque[PHYSICAL_CONST_THREE_DIM];

  for (int i = 0; i < PHYSICAL_CONST_THREE_DIM; i++)
  {
    ENDIAN_memcpy(&max_torque[i], param + param_id, sizeof(float));
    param_id += (int)sizeof(float);
  }

  AOCS_MANAGER_ERROR ret = AOCS_MANAGER_set_external_torque_max_body_Nm(max_torque);
  if (ret != AOCS_MANAGER_ERROR_OK) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);

  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

#pragma section
