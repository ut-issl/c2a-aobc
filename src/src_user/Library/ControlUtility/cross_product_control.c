#pragma section REPRO
/**
 * @file
 * @brief  クロスプロダクト制御を行うライブラリ
 */

#include "cross_product_control.h"

#include <math.h>

#include "../vector3.h"
#include "../math_constants.h"


void CROSS_PRODUCT_CONTROL_init(CrossProductControl* cross_product_control)
{
  cross_product_control->allowable_error_ratio = 1.0f;
  cross_product_control->torque_scale_factor   = 1.0f;

  return;
}

CROSS_PRODUCT_CONTROL_ERROR CROSS_PRODUCT_CONTROL_calc_mag_moment_from_ext_torque(const CrossProductControl cross_product_control,
                                                                                  float mag_moment_target_Am2[PHYSICAL_CONST_THREE_DIM],
                                                                                  const float torque_target_body_Nm[PHYSICAL_CONST_THREE_DIM],
                                                                                  const float mag_vec_est_body_nT[PHYSICAL_CONST_THREE_DIM],
                                                                                  float* error_ratio)
{
  const float kBillion = 1.0e9f; // nT->Tの変換を行う

  float trq_vec_normalized[PHYSICAL_CONST_THREE_DIM];
  float mag_vec_normalized[PHYSICAL_CONST_THREE_DIM];
  CROSS_PRODUCT_CONTROL_ERROR error_status = CROSS_PRODUCT_CONTROL_ERROR_OK;
  VECTOR3_initialize(mag_moment_target_Am2, 0.0f);

  // 機体MOIが小さく，かつ指令値更新周期も遅いので，CrossProductで求まる出力トルク誤差 (擾乱) の感受性が高く，自家中毒になりやすい
  // さりとて，自家中毒を無理やり抑え込むには，今度はMTQ最大出力が低く，指令値の分解能も粗い…．(誤差を振り切る広帯域制御はできない)
  // よって，CrossProductで導かれるMTQ出力について，指令トルクのTBD%の擾乱が出る場合には，指令トルクに比例ゲインを掛けて弱めることで擾乱を下げる
  C2A_MATH_ERROR torque_non_zero_check = VECTOR3_normalize(trq_vec_normalized, torque_target_body_Nm);
  C2A_MATH_ERROR mag_non_zero_check    = VECTOR3_normalize(mag_vec_normalized, mag_vec_est_body_nT);
  if ((torque_non_zero_check == C2A_MATH_ERROR_SINGULAR) || (mag_non_zero_check == C2A_MATH_ERROR_SINGULAR))
  {
    return CROSS_PRODUCT_CONTROL_ERROR_INVALID_PARAMS;
  }

  float cos_factor = fabsf(VECTOR3_inner_product(trq_vec_normalized, mag_vec_normalized));
  *error_ratio = cos_factor;
  float torque_scale_factor = 1.0f;
  if (cos_factor >= cross_product_control.allowable_error_ratio)
  {
    torque_scale_factor = cross_product_control.torque_scale_factor;
    error_status = CROSS_PRODUCT_CONTROL_ERROR_LARGE_ERROR_TORQUE;
  }

  // 以降は通常のCrossProductと同じ
  float mag_moment_temp_Am2[PHYSICAL_CONST_THREE_DIM];
  float mag_vec_est_body_T[PHYSICAL_CONST_THREE_DIM];

  VECTOR3_scalar_product(mag_vec_est_body_T, 1.0f / kBillion, mag_vec_est_body_nT);
  float mag_norm_T = VECTOR3_norm(mag_vec_est_body_T);

  VECTOR3_outer_product(mag_moment_temp_Am2, mag_vec_est_body_T, torque_target_body_Nm);
  VECTOR3_scalar_product(mag_moment_target_Am2, torque_scale_factor / mag_norm_T / mag_norm_T, mag_moment_temp_Am2);

  return error_status;
}

C2A_MATH_ERROR CROSS_PRODUCT_CONTROL_set_anti_intoxication_parameters(CrossProductControl* cross_product_control,
                                                                      const float allowable_error_ratio,
                                                                      const float torque_scale_factor)
{
  C2A_MATH_ERROR ratio_check = C2A_MATH_check_range_violation(allowable_error_ratio, 1.0f, 0.0f);
  C2A_MATH_ERROR scale_factor_check = C2A_MATH_check_range_violation(torque_scale_factor, 1.0f, 0.0f);

  if ((ratio_check != C2A_MATH_ERROR_OK) || (scale_factor_check != C2A_MATH_ERROR_OK))
  {
    return C2A_MATH_ERROR_RANGE_OVER;
  }

  cross_product_control->allowable_error_ratio = allowable_error_ratio;
  cross_product_control->torque_scale_factor   = torque_scale_factor;

  return C2A_MATH_ERROR_OK;
}

#pragma section
