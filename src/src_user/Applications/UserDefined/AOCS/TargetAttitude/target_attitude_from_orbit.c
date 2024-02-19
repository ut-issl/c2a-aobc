#pragma section REPRO
/**
 * @file   target_attitude_from_orbit.c
 * @brief  推定軌道から目標姿勢を計算するアプリ
 */

#include <src_core/TlmCmd/common_cmd_packet_util.h>
#include <src_core/System/EventManager/event_logger.h>
#include <src_user/Applications/app_registry.h>

#include "target_attitude_from_orbit.h"
#include <src_user/Applications/UserDefined/AOCS/aocs_manager.h>
#include <src_user/Library/math_constants.h>
#include <src_user/Library/c2a_math.h>
#include <src_user/Library/vector3.h>
#include <src_user/Library/time_space.h>

// SatelliteParameters
#include <src_user/Settings/SatelliteParameters/attitude_target_parameters.h>

static TargetAttitudeFromOrbit target_attitude_from_orbit_;
const TargetAttitudeFromOrbit *const target_attitude_from_orbit = &target_attitude_from_orbit_;

static void APP_TAFO_init_(void);
static void APP_TAFO_exec_(void);

/**
 * @brief  ベクトル直交化関数
 *         基準ベクトルに直交するベクトルに変換する
 * @param[out]  vec     : 変換対象ベクトル
 * @param[in]   ref_vec : 基準ベクトル
 * @return C2A_MATH_ERROR
 * @note   vecとref_vecが平行のときエラーを返す
 */
static C2A_MATH_ERROR APP_TAFO_make_vec_orthogonal_to_ref_vec_(float vec[PHYSICAL_CONST_THREE_DIM],
                                                               const float ref_vec[PHYSICAL_CONST_THREE_DIM]);

/**
 * @brief  目標方向ベクトル（ECI）計算関数
 *         衛星から見た対象の方向を単位ベクトルで返す
 * @param[out]  target_direction_vec_eci_normalized : 衛星から見た対象の方向を表す単位ベクトル（ECI座標系）
 * @param[in]   direction                           : 目標方向
 * @return void
 */
static void APP_TAFO_calc_target_direction_vec_eci_(float target_direction_vec_eci_normalized[PHYSICAL_CONST_THREE_DIM],
                                                    const APP_TAFO_TARGET_DIRECITON direction);

/**
 * @brief  目標方向チェック関数
 *         main，subの2目標方向が一致しているかどうか判定する
 * @param[in]  main_target : main目標
 * @param[in]  sub_target  : sub目標
 * @return 0:不一致，1:一致
 */
static uint8_t APP_TAFO_check_is_target_direction_same_(APP_TAFO_TARGET_DIRECITON main_target,
                                                        APP_TAFO_TARGET_DIRECITON sub_target);

AppInfo APP_TAFO_create_app(void)
{
  return AI_create_app_info("TargetAttitudeFromOrbit", APP_TAFO_init_, APP_TAFO_exec_);
}

static void APP_TAFO_init_(void)
{
  target_attitude_from_orbit_.main_target_direction = ATTITUDE_TARGET_PARAMETERS_main_target_direction;
  target_attitude_from_orbit_.sub_target_direction = ATTITUDE_TARGET_PARAMETERS_sub_target_direction;
  VECTOR3_copy(target_attitude_from_orbit_.vec_to_main_target_body, ATTITUDE_TARGET_PARAMETERS_vec_to_main_target_body);
  VECTOR3_copy(target_attitude_from_orbit_.vec_to_sub_target_body, ATTITUDE_TARGET_PARAMETERS_vec_to_sub_target_body);

  target_attitude_from_orbit_.offset_angle_axis = ATTITUDE_TARGET_PARAMETERS_offset_angle_axis;
  target_attitude_from_orbit_.offset_angle_rad = ATTITUDE_TARGET_PARAMETERS_offset_angle_rad;

  VECTOR3_copy(target_attitude_from_orbit_.target_lla_rad_m, ATTITUDE_TARGET_PARAMETERS_target_lla_rad_m);
}

static void APP_TAFO_exec_(void)
{
  // 目標方向決定
  float main_target_direction_vec_eci_normalized[PHYSICAL_CONST_THREE_DIM];
  float sub_target_direction_vec_eci_normalized[PHYSICAL_CONST_THREE_DIM];
  C2A_MATH_ERROR error;
  APP_TAFO_calc_target_direction_vec_eci_(main_target_direction_vec_eci_normalized,
                                          target_attitude_from_orbit_.main_target_direction);
  APP_TAFO_calc_target_direction_vec_eci_(sub_target_direction_vec_eci_normalized,
                                          target_attitude_from_orbit_.sub_target_direction);
  error = APP_TAFO_make_vec_orthogonal_to_ref_vec_(sub_target_direction_vec_eci_normalized,
                                                   main_target_direction_vec_eci_normalized);

  if (error != C2A_MATH_ERROR_OK)
  {
    EL_record_event(EL_GROUP_CALCULATION_ERROR, (uint32_t)AR_APP_TARGET_ATTITUDE_FROM_ORBIT, EL_ERROR_LEVEL_LOW, (uint32_t)error);
    return;
  }

  // 目標DCM計算
  float dcm_eci_to_body[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM];
  float dcm_target_to_eci[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM];
  float dcm_eci_to_target[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM];
  float dcm_target_to_body[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM];
  float dcm_offset[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM];
  error = MATRIX33_make_dcm_from_two_vectors(dcm_target_to_body,
                                             target_attitude_from_orbit_.vec_to_main_target_body,
                                             target_attitude_from_orbit_.vec_to_sub_target_body);
  if (error != C2A_MATH_ERROR_OK)
  {
    // TODO_L: noteについてもう少し整理する
    EL_record_event(EL_GROUP_CALCULATION_ERROR, (uint32_t)AR_APP_TARGET_ATTITUDE_FROM_ORBIT, EL_ERROR_LEVEL_LOW, (uint32_t)(error + 0x80));
    return;
  }
  error = MATRIX33_make_dcm_from_two_vectors(dcm_target_to_eci,
                                             main_target_direction_vec_eci_normalized,
                                             sub_target_direction_vec_eci_normalized);
  if (error != C2A_MATH_ERROR_OK)
  {
    // TODO_L: noteについてもう少し整理する
    EL_record_event(EL_GROUP_CALCULATION_ERROR, (uint32_t)AR_APP_TARGET_ATTITUDE_FROM_ORBIT, EL_ERROR_LEVEL_LOW, (uint32_t)(error + 0xc0));
    return;
  }
  MATRIX33_transpose(dcm_eci_to_target, dcm_target_to_eci);
  MATRIX33_multiply_matrix_matrix(dcm_eci_to_body, dcm_target_to_body, dcm_eci_to_target);

  // オフセット角の付与
  MATRIX33_make_rotation(dcm_offset,
                         target_attitude_from_orbit_.offset_angle_rad,
                         target_attitude_from_orbit_.offset_angle_axis);
  MATRIX33_multiply_matrix_matrix(dcm_eci_to_body, dcm_offset, dcm_eci_to_body);

  // 目標Quaternion計算
  QUATERNION_make_from_matrix(&target_attitude_from_orbit_.quaternion_target_i2t, dcm_eci_to_body);
  if (target_attitude_from_orbit_.quaternion_target_i2t.scalar_part < 0.0f)
  {
    target_attitude_from_orbit_.quaternion_target_i2t =
        QUATERNION_scalar_product(-1.0f, target_attitude_from_orbit_.quaternion_target_i2t);
  }

  if (target_attitude_from_orbit_.is_enabled)
  {
    AOCS_MANAGER_set_quaternion_target_i2t(target_attitude_from_orbit_.quaternion_target_i2t);
  }

  return;
}

static C2A_MATH_ERROR APP_TAFO_make_vec_orthogonal_to_ref_vec_(float vec[PHYSICAL_CONST_THREE_DIM],
                                                               const float ref_vec[PHYSICAL_CONST_THREE_DIM])
{
  float vec_outer_product[PHYSICAL_CONST_THREE_DIM];
  float vec_outer_product_norm;

  VECTOR3_outer_product(vec_outer_product,
                        ref_vec,
                        vec);
  vec_outer_product_norm = VECTOR3_norm(vec_outer_product);
  if (vec_outer_product_norm < MATH_CONST_NORMAL_CHECK_THRESHOLD)
  {
    return C2A_MATH_ERROR_SINGULAR;
  }

  VECTOR3_outer_product(vec,
                        vec_outer_product,
                        ref_vec);
  VECTOR3_normalize(vec, vec);

  return C2A_MATH_ERROR_OK;
}

static void APP_TAFO_calc_target_direction_vec_eci_(float target_direction_vec_eci_normalized[PHYSICAL_CONST_THREE_DIM],
                                                    const APP_TAFO_TARGET_DIRECITON direction)
{
  switch (direction)
  {
  case APP_TAFO_TARGET_DIRECITON_SUN:
  {
    float sun_vec_est_eci_normalized[PHYSICAL_CONST_THREE_DIM];

    VECTOR3_normalize(sun_vec_est_eci_normalized, aocs_manager->sun_vec_est_eci);
    VECTOR3_copy(target_direction_vec_eci_normalized, sun_vec_est_eci_normalized);
    break;
  }
  case APP_TAFO_TARGET_DIRECITON_SAT_VELOCITY:
  {
    float float_sat_vel_est_eci_m_s[PHYSICAL_CONST_THREE_DIM];
    float sat_vel_vec_est_eci_normalized[PHYSICAL_CONST_THREE_DIM];

    for (int idx = 0; idx < PHYSICAL_CONST_THREE_DIM; idx++)
    {
      float_sat_vel_est_eci_m_s[idx] = (float)aocs_manager->sat_vel_est_eci_m_s[idx];
    }
    VECTOR3_normalize(sat_vel_vec_est_eci_normalized, float_sat_vel_est_eci_m_s);
    VECTOR3_copy(target_direction_vec_eci_normalized, sat_vel_vec_est_eci_normalized);
    break;
  }
  case APP_TAFO_TARGET_DIRECTION_SAT_RELATIVE_VELOCITY:
  {
    double dcm_ecef_to_eci[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM];
    double sat_rel_vel_est_ecef_m_s[PHYSICAL_CONST_THREE_DIM];
    float float_sat_vel_est_ecef_m_s[PHYSICAL_CONST_THREE_DIM];
    float float_sat_rel_vel_est_ecef_m_s[PHYSICAL_CONST_THREE_DIM];
    float sat_vel_rel_vec_est_ecef_m_s_normalized[PHYSICAL_CONST_THREE_DIM];

    for (int idx = 0; idx < PHYSICAL_CONST_THREE_DIM; idx++)
    {
      float_sat_vel_est_ecef_m_s[idx] = (float)aocs_manager->sat_vel_est_ecef_m_s[idx];
    }
    MATRIX33_transpose_double(dcm_ecef_to_eci, aocs_manager->dcm_eci_to_ecef);
    VECTOR3_multiply_matrix(sat_rel_vel_est_ecef_m_s,
                            dcm_ecef_to_eci,
                            float_sat_vel_est_ecef_m_s);
    for (int idx = 0; idx < PHYSICAL_CONST_THREE_DIM; idx++)
    {
      float_sat_rel_vel_est_ecef_m_s[idx] = (float)sat_rel_vel_est_ecef_m_s[idx];
    }
    VECTOR3_normalize(sat_vel_rel_vec_est_ecef_m_s_normalized, float_sat_rel_vel_est_ecef_m_s);
    VECTOR3_copy(target_direction_vec_eci_normalized, sat_vel_rel_vec_est_ecef_m_s_normalized);
  }
  case APP_TAFO_TARGET_DIRECITON_EARTH_CENTER:
  {
    float float_sat_pos_est_eci_m[PHYSICAL_CONST_THREE_DIM];
    float sat_pos_vec_est_eci_normalized[PHYSICAL_CONST_THREE_DIM];
    float earth_vec_est_eci_normalized[PHYSICAL_CONST_THREE_DIM];

    // [TODO_L] Vector libにdouble -> float, float -> doubleキャスト関数を実装する
    for (int idx = 0; idx < PHYSICAL_CONST_THREE_DIM; idx++)
    {
      float_sat_pos_est_eci_m[idx] = (float)aocs_manager->sat_pos_est_eci_m[idx];
    }
    VECTOR3_normalize(sat_pos_vec_est_eci_normalized, float_sat_pos_est_eci_m);
    VECTOR3_scalar_product(earth_vec_est_eci_normalized, -1.0f, sat_pos_vec_est_eci_normalized);
    VECTOR3_copy(target_direction_vec_eci_normalized, earth_vec_est_eci_normalized);
    break;
  }
  case APP_TAFO_TARGET_DIRECITON_EARTH_SURFACE:
  {
    double target_lla_rad_m[PHYSICAL_CONST_THREE_DIM];
    double target_pos_ecef_m[PHYSICAL_CONST_THREE_DIM];
    double target_pos_eci_m[PHYSICAL_CONST_THREE_DIM];
    double dcm_ecef_to_eci[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM];
    float float_sat_pos_est_eci_m[PHYSICAL_CONST_THREE_DIM];
    float float_target_pos_eci_m[PHYSICAL_CONST_THREE_DIM];
    float target_pos_eci_from_sat_m[PHYSICAL_CONST_THREE_DIM];
    float earth_ground_point_vec_eci_normalized[PHYSICAL_CONST_THREE_DIM];

    for (int idx = 0; idx < PHYSICAL_CONST_THREE_DIM; idx++)
    {
      target_lla_rad_m[idx] = (double)target_attitude_from_orbit_.target_lla_rad_m[idx];
    }
    TIME_SPACE_convert_lla_to_ecef(target_pos_ecef_m, target_lla_rad_m);
    MATRIX33_transpose_double(dcm_ecef_to_eci, aocs_manager->dcm_eci_to_ecef);
    MATRIX33_multiply_matrix_vector_double(target_pos_eci_m,
                                           dcm_ecef_to_eci,
                                           target_pos_ecef_m);
    for (int idx = 0; idx < PHYSICAL_CONST_THREE_DIM; idx++)
    {
      float_sat_pos_est_eci_m[idx] = (float)aocs_manager->sat_pos_est_eci_m[idx];
      float_target_pos_eci_m[idx] = (float)target_pos_eci_m[idx];
    }
    VECTOR3_subtract(target_pos_eci_from_sat_m, float_target_pos_eci_m, float_sat_pos_est_eci_m);
    VECTOR3_normalize(earth_ground_point_vec_eci_normalized, target_pos_eci_from_sat_m);
    VECTOR3_copy(target_direction_vec_eci_normalized, earth_ground_point_vec_eci_normalized);
    break;
  }
  case APP_TAFO_TARGET_DIRECITON_ORBIT_NORMAL:
  {
    float float_sat_pos_est_eci_m[PHYSICAL_CONST_THREE_DIM];
    float float_sat_vel_est_eci_m_s[PHYSICAL_CONST_THREE_DIM];
    float orbit_normal_vec[PHYSICAL_CONST_THREE_DIM];
    float orbit_normal_vec_est_eci_normalized[PHYSICAL_CONST_THREE_DIM];

    for (int idx = 0; idx < PHYSICAL_CONST_THREE_DIM; idx++)
    {
      float_sat_pos_est_eci_m[idx] = (float)aocs_manager->sat_pos_est_eci_m[idx];
      float_sat_vel_est_eci_m_s[idx] = (float)aocs_manager->sat_vel_est_eci_m_s[idx];
    }
    VECTOR3_outer_product(orbit_normal_vec, float_sat_pos_est_eci_m, float_sat_vel_est_eci_m_s);
    VECTOR3_normalize(orbit_normal_vec_est_eci_normalized, orbit_normal_vec);
    VECTOR3_copy(target_direction_vec_eci_normalized, orbit_normal_vec_est_eci_normalized);
    break;
  }
  default:
    // NOT REACHED
    break;
  }
}

static uint8_t APP_TAFO_check_is_target_direction_same_(APP_TAFO_TARGET_DIRECITON main_target,
                                                        APP_TAFO_TARGET_DIRECITON sub_target)
{
  if (main_target == sub_target)
  {
    return 1;
  }
  else
  {
    return 0;
  }
}

CCP_CmdRet Cmd_APP_TAFO_SET_MAIN_TARGET_DIRECTION(const CommonCmdPacket *packet)
{
  float vec_to_main_target_body[PHYSICAL_CONST_THREE_DIM] = {1.0f, 0.0f, 0.0f};

  uint8_t main_target_direction = CCP_get_param_from_packet(packet, 0, uint8_t);
  if (main_target_direction >= APP_TAFO_TARGET_DIRECITON_MAX)
  {
    return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
  }

  for (int i = 0; i < PHYSICAL_CONST_THREE_DIM; i++)
  {
    vec_to_main_target_body[i] = CCP_get_param_from_packet(packet, 1 + i, float);
  }

  C2A_MATH_ERROR is_normalized;
  is_normalized = VECTOR3_is_normalized(vec_to_main_target_body);
  if (is_normalized != C2A_MATH_ERROR_OK)
  {
    return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
  }

  // main_target_directionとsub_target_directionが一致するのを禁止
  uint8_t is_target_direction_same = APP_TAFO_check_is_target_direction_same_((APP_TAFO_TARGET_DIRECITON)main_target_direction,
                                                                              target_attitude_from_orbit_.sub_target_direction);
  if (is_target_direction_same)
  {
    return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_CONTEXT);
  }

  // vec_to_main_target_bodyとvec_to_sub_target_bodyが一致するのを禁止
  C2A_MATH_ERROR c2a_math_error;
  c2a_math_error = APP_TAFO_make_vec_orthogonal_to_ref_vec_(target_attitude_from_orbit_.vec_to_sub_target_body,
                                                            vec_to_main_target_body);
  if (c2a_math_error != C2A_MATH_ERROR_OK)
  {
    return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_CONTEXT);
  }

  target_attitude_from_orbit_.main_target_direction = (APP_TAFO_TARGET_DIRECITON)main_target_direction;
  VECTOR3_copy(target_attitude_from_orbit_.vec_to_main_target_body, vec_to_main_target_body);

  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

CCP_CmdRet Cmd_APP_TAFO_SET_SUB_TARGET_DIRECTION(const CommonCmdPacket *packet)
{
  float vec_to_sub_target_body[PHYSICAL_CONST_THREE_DIM] = {1.0f, 0.0f, 0.0f};

  uint8_t sub_target_direction = CCP_get_param_from_packet(packet, 0, uint8_t);
  if (sub_target_direction >= APP_TAFO_TARGET_DIRECITON_MAX)
  {
    return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
  }

  for (int i = 0; i < PHYSICAL_CONST_THREE_DIM; i++)
  {
    vec_to_sub_target_body[i] = CCP_get_param_from_packet(packet, 1 + i, float);
  }

  C2A_MATH_ERROR is_normalized;
  is_normalized = VECTOR3_is_normalized(vec_to_sub_target_body);
  if (is_normalized != C2A_MATH_ERROR_OK)
  {
    return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
  }

  // main_target_directionとsub_target_directionが一致するのを禁止
  uint8_t is_target_direction_same = APP_TAFO_check_is_target_direction_same_(target_attitude_from_orbit_.main_target_direction,
                                                                              (APP_TAFO_TARGET_DIRECITON)sub_target_direction);
  if (is_target_direction_same)
  {
    return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_CONTEXT);
  }

  // vec_to_main_target_bodyとvec_to_sub_target_bodyが一致するのを禁止
  C2A_MATH_ERROR c2a_math_error;
  c2a_math_error = APP_TAFO_make_vec_orthogonal_to_ref_vec_(vec_to_sub_target_body,
                                                            target_attitude_from_orbit_.vec_to_main_target_body);
  if (c2a_math_error != C2A_MATH_ERROR_OK)
  {
    return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_CONTEXT);
  }

  target_attitude_from_orbit_.sub_target_direction = (APP_TAFO_TARGET_DIRECITON)sub_target_direction;
  VECTOR3_copy(target_attitude_from_orbit_.vec_to_sub_target_body, vec_to_sub_target_body);

  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

CCP_CmdRet Cmd_APP_TAFO_SET_OFFSET_ANGLE_AND_AXIS(const CommonCmdPacket *packet)
{

  MATRIX33_ROTATION_AXIS offset_angle_axis = (MATRIX33_ROTATION_AXIS)CCP_get_param_from_packet(packet, 0, uint8_t);
  if (offset_angle_axis >= MATRIX33_ROTATION_MAX)
  {
    return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
  }

  float offset_angle_deg = CCP_get_param_from_packet(packet, 1, float);
  if ((offset_angle_deg <= -180.0f) || (180.0f < offset_angle_deg))
  {
    return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
  }

  target_attitude_from_orbit_.offset_angle_axis = (MATRIX33_ROTATION_AXIS)offset_angle_axis;
  target_attitude_from_orbit_.offset_angle_rad = PHYSICAL_CONST_degree_to_radian(offset_angle_deg);

  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

CCP_CmdRet Cmd_APP_TAFO_SET_TARGET_LLA(const CommonCmdPacket *packet)
{
  float latitude_deg = CCP_get_param_from_packet(packet, 0, float);
  if (latitude_deg < -90.0f || 90.0f < latitude_deg)
  {
    return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
  }

  float longitude_deg = CCP_get_param_from_packet(packet, 1, float);
  if (longitude_deg <= -180.0f || 180.0f < longitude_deg)
  {
    return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
  }

  float altitude_m = CCP_get_param_from_packet(packet, 2, float);
  if (altitude_m < 0.0f)
  {
    return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
  }

  target_attitude_from_orbit_.target_lla_rad_m[0] = PHYSICAL_CONST_degree_to_radian(latitude_deg);
  target_attitude_from_orbit_.target_lla_rad_m[1] = PHYSICAL_CONST_degree_to_radian(longitude_deg);
  target_attitude_from_orbit_.target_lla_rad_m[2] = altitude_m;

  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

CCP_CmdRet Cmd_APP_TAFO_ENABLE(const CommonCmdPacket *packet)
{
  uint8_t is_enabled = CCP_get_param_from_packet(packet, 0, uint8_t);
  if (is_enabled > 1)
  {
    return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
  }

  // main_target_directionとsub_target_directionが一致しているとき，
  // is_enabledの有効化を禁止
  uint8_t is_target_direction_same = APP_TAFO_check_is_target_direction_same_(target_attitude_from_orbit_.main_target_direction,
                                                                              target_attitude_from_orbit_.sub_target_direction);
  if (is_target_direction_same)
  {
    return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_CONTEXT);
  }

  target_attitude_from_orbit_.is_enabled = is_enabled;

  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

#pragma section
