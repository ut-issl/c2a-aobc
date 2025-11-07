/**
 * @file
 * @brief  ジャイロ効果(wb x hb)の計算および関連する各種計算
 */

#ifndef GYROSCOPIC_EFFECT_H_
#define GYROSCOPIC_EFFECT_H_

#include <src_user/Library/physical_constants.h>


 /**
  * @brief        ジャイロ効果トルク ωb x hbの計算
  * @param[out]   gyroscopic_trq_Nm  : ジャイロ効果トルクωb x hb [Nm]
  * @param[in]    body_rate_rad_s    : 姿勢レート制御使用値 at 機体座標系 [rad/s]
  * @param[in]    body_momentum_Nms  : 機体系 (RW込み) の角運動量 at 機体座標系 [Nms]
  * @return       void
  */
void GYROSCOPIC_EFFECT_calculate_gyroscopic_torque(float gyroscopic_trq_Nm[PHYSICAL_CONST_THREE_DIM],
                                                   const float body_rate_rad_s[PHYSICAL_CONST_THREE_DIM],
                                                   const float body_momentum_Nms[PHYSICAL_CONST_THREE_DIM]);


/**
 * @brief        機体系 (RW込み) の全角運動量の計算
 * @param[out]   momentum_Nms       : 機体系 (RW込み) の角運動量 at 機体座標系 [Nms]
 * @param[in]    sat_inertia_Kgm2   : 姿勢慣性モーメント at 機体座標系 [kgm^2]
 * @param[in]    body_rate_rad_s    : 姿勢レート制御使用値 at 機体座標系 [rad/s]
 * @param[in]    rw_momentum_Nms    : RWの角運動量 at 機体座標系 [Nms]
 * @return       void
 */
void GYROSCOPIC_EFFECT_calculate_total_momentum(float momentum_Nms[PHYSICAL_CONST_THREE_DIM],
                                                const float sat_inertia_Kgm2[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM],
                                                const float body_rate_rad_s[PHYSICAL_CONST_THREE_DIM],
                                                const float rw_momentum_Nms[PHYSICAL_CONST_THREE_DIM]);


#endif
