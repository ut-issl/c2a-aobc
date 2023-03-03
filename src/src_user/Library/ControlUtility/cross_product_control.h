/**
 * @file
 * @brief  クロスプロダクト制御を行うライブラリ
 */
#ifndef CROSS_PRODUCT_CONTROL_H_
#define CROSS_PRODUCT_CONTROL_H_

#include "../physical_constants.h"
#include "../c2a_math.h"

/**
 * @enum   CROSS_PRODUCT_CONTROL_ERROR
 * @brief  クロスプロダクト則計算に関するエラーコード
 * @note   uint8_tを想定
 */
typedef enum
{
  CROSS_PRODUCT_CONTROL_ERROR_OK = 0,             //!< エラーなし
  CROSS_PRODUCT_CONTROL_ERROR_INVALID_PARAMS,     //!< パラメータが不適切
  CROSS_PRODUCT_CONTROL_ERROR_LARGE_ERROR_TORQUE, //!< 出力トルク精度劣化が許容できない (誤差トルク > 指令トルク * 許容率)
  CROSS_PRODUCT_CONTROL_ERROR_MAX
} CROSS_PRODUCT_CONTROL_ERROR;

/**
 * @struct CrossProductControl
 * @brief  CrossProduct制御パラメータの構造体
 */
typedef struct
{
  float allowable_error_ratio;
  float torque_scale_factor;

} CrossProductControl;

/**
 * @brief  自家中毒対策パラメータの初期化
 */
void CROSS_PRODUCT_CONTROL_init(CrossProductControl* cross_product_control);

/**
 * @brief  外力トルクから磁気モーメント出力を計算する関数
 * @param[out] mag_moment_target_Am2 : 磁気モーメント出力を格納するベクトル
 * @param[in]  torque_target_body_Nm : 機体固定座標目標外力トルク [Nm]
 * @param[in]  mag_vec_est_body_nT   : 機体固定座標推定磁場 [nT]
 * @return CROSS_PRODUCT_CONTROL_ERRORを参照
 */
CROSS_PRODUCT_CONTROL_ERROR CROSS_PRODUCT_CONTROL_calc_mag_moment_from_ext_torque(const CrossProductControl cross_product_control,
                                                                                  float mag_moment_target_Am2[PHYSICAL_CONST_THREE_DIM],
                                                                                  const float torque_target_body_Nm[PHYSICAL_CONST_THREE_DIM],
                                                                                  const float mag_vec_est_body_nT[PHYSICAL_CONST_THREE_DIM],
                                                                                  float* error_ratio);

/**
 * @brief  自家中毒対策パラメータの設定関数
 * @param[in]  allowable_error_ratio : 地磁場方向に起因する誤差トルクの指令トルクに対する許容率 [0 ~ 1]
 * @param[in]  torque_scale_factor   : 許容トルク誤差を超える誤差トルクが発生した際に
                                       磁気モーメント換算する前の指令トルクに掛けるスケールファクタ [0 ~ 1]
 * @return 不正な設定値に対しC2A_MATH_ERROR_RANGE_OVERを返す
 * @note   allowable_error_ratioを超える誤差トルクが出る際に，指令トルクにtorque_scale_factorを掛けた値で磁気モーメントを計算する
 */
C2A_MATH_ERROR CROSS_PRODUCT_CONTROL_set_anti_intoxication_parameters(CrossProductControl* cross_product_control,
                                                                      const float allowable_error_ratio,
                                                                      const float torque_scale_factor);

#endif
