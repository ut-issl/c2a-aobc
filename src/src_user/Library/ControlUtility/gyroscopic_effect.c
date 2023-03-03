#pragma section REPRO
/**
 * @file
 * @brief  ジャイロ効果(wb x hb)の計算および関連する各種計算
 */

#include "gyroscopic_effect.h"

#include "../vector3.h"
#include "../matrix33.h"


void GYROSCOPIC_EFFECT_calculate_gyroscopic_torque(float gyroscopic_trq_Nm[PHYSICAL_CONST_THREE_DIM],
                                                   const float body_rate_rad_s[PHYSICAL_CONST_THREE_DIM],
                                                   const float body_momentum_Nms[PHYSICAL_CONST_THREE_DIM])
{
  VECTOR3_outer_product(gyroscopic_trq_Nm, body_rate_rad_s, body_momentum_Nms);
  return;
}


void GYROSCOPIC_EFFECT_calculate_total_momentum(float momentum_Nms[PHYSICAL_CONST_THREE_DIM],
                                                const float sat_inertia_Kgm2[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM],
                                                const float body_rate_rad_s[PHYSICAL_CONST_THREE_DIM],
                                                const float rw_momentum_Nms[PHYSICAL_CONST_THREE_DIM])
{
  float sat_momentum_Nms[PHYSICAL_CONST_THREE_DIM];

  MATRIX33_multiply_matrix_vector(sat_momentum_Nms, sat_inertia_Kgm2, body_rate_rad_s);
  VECTOR3_add(momentum_Nms, sat_momentum_Nms, rw_momentum_Nms);

  return;
}


#pragma section
