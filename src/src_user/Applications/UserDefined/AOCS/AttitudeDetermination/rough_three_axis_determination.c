#pragma section REPRO
/**
* @file    rough_three_axis_determination.c
* @brief   粗三軸姿勢決定アプリ
*
*          TRIADとQmethodを使い分ける
*/

#include "rough_three_axis_determination.h"

#include <math.h>
#include <src_core/TlmCmd/common_cmd_packet_util.h>
#include <src_user/Applications/UserDefined/AOCS/aocs_manager.h>
#include <src_user/Applications/UserDefined/AOCS/aocs_error.h>
#include <src_user/Library/vector3.h>
#include <src_user/Library/matrix33.h>
#include <src_user/Library/quaternion.h>
#include <src_user/Library/physical_constants.h>
#include <src_user/Library/math_constants.h>

// Satellite parameters
#include <src_user/Settings/SatelliteParameters/attitude_determination_parameters.h>

static RoughThreeAxisDetermination        rough_three_axis_determination_;
const  RoughThreeAxisDetermination* const rough_three_axis_determination = &rough_three_axis_determination_;

static void APP_RTAD_init_(void);
static void APP_RTAD_exec_(void);

// [TODO_L]以下の値は，必要になったら値を修正できるようにする
static const int APP_RTAD_kMaxNumOfIteration_      = 10;     //!< Qmethodにおけるニュートン・ラフソン法の最大計算実行回数
static const float APP_RTAD_kConvergenceThreshold_ = 1.0e-4; //!< Qmethodにおける固有方程式計算で，解が収束したとみなす閾値．センサの観測分散より小さくなるように決める

/**
* @brief        TRIAD法による三軸姿勢決定
* @param[in]    sun_ref_vec      : 基準の座標系における太陽方向単位ベクトル
* @param[in]    mag_ref_vec      : 基準の座標系における磁場単位ベクトル
* @param[in]    sun_obs_vec      : 衛星が観測した太陽方向単位ベクトル
* @param[in]    mag_obs_vec      : 衛星が観測した磁場単位ベクトル
* @return       q_eci_to_body    : TRIADによって計算されたECI->body変換クオータニオン
*/
static Quaternion APP_RTAD_triad_(const float sun_ref_vec[PHYSICAL_CONST_THREE_DIM],
                                  const float mag_ref_vec[PHYSICAL_CONST_THREE_DIM],
                                  const float sun_obs_vec[PHYSICAL_CONST_THREE_DIM],
                                  const float mag_obs_vec[PHYSICAL_CONST_THREE_DIM]);

/**
* @brief        TRIAD法で使う正規直交行列の生成関数
*
*               太陽方向ベクトルと磁場ベクトルから正規直交基底を生成し，行列形式にして格納する．
* @param[out]   triad_mat : 出力先の正規直交行列
* @param[in]    sun_vec   : 太陽方向単位ベクトル
* @param[in]    mag_vec   : 磁場単位ベクトル
* @return       void
*/
static void APP_RTAD_generate_triad_mat_(float triad_mat[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM],
                                   const float sun_vec[PHYSICAL_CONST_THREE_DIM],
                                   const float mag_vec[PHYSICAL_CONST_THREE_DIM]);

/**
* @brief        Qmethodによる三軸姿勢決定
* @param[in]    sun_ref_vec      : 基準の座標系における太陽方向単位ベクトル
* @param[in]    mag_ref_vec      : 基準の座標系における磁場単位ベクトル
* @param[in]    sun_obs_vec      : 衛星が観測した太陽方向単位ベクトル
* @param[in]    mag_obs_vec      : 衛星が観測した磁場単位ベクトル
* @param[in]    q_method_info     : QMethod関連情報
* @return       q_eci_to_body    : Qmethodによって計算されたECI->body変換クオータニオン
*/
static Quaternion APP_RTAD_q_method_(const float sun_ref_vec[PHYSICAL_CONST_THREE_DIM],
                                     const float mag_ref_vec[PHYSICAL_CONST_THREE_DIM],
                                     const float sun_obs_vec[PHYSICAL_CONST_THREE_DIM],
                                     const float mag_obs_vec[PHYSICAL_CONST_THREE_DIM],
                                     const QMethodInfo q_method_info);

/**
* @brief   Qmethodで使われるパラメータを計算関数
*
*          姿勢変換クオータニオンqの最尤推定値を与えるゲインファンクションg(A) = q^T*K*qにおける，Kの成分，および関連パラメータを計算する
*              (          z1 )
*          K = ( S - σE   z2 )
*              (          z3 )
*              ( z1 z2 z3 σ  )
* @param[out]   sigma       : 上式におけるσ
* @param[out]   delta       : det(s)
* @param[out]   kappa       : tr(adj(S))
* @param[out]   z_vector    : 上式におけるベクトルz = (z1, z2, z3)
* @param[out]   s_matrix    : 上式における3×3行列S
* @param[out]   s_square_matrix : S^2
* @param[in]    sun_ref_vec : 基準の座標系における太陽方向単位ベクトル
* @param[in]    mag_ref_vec : 基準の座標系における磁場単位ベクトル
* @param[in]    sun_obs_vec : 衛星が観測した太陽方向単位ベクトル
* @param[in]    mag_obs_vec : 衛星が観測した磁場単位ベクトル
* @param[in]    sun_weight  : アルゴリズムにおける太陽方向ベクトルの重み
* @param[in]    mag_weight  : アルゴリズムにおける磁場方向ベクトルの重み
* @return                   : void
*/
static void APP_RTAD_calculate_q_method_parameters_(float* sigma,
                                                   float* delta,
                                                   float* kappa,
                                                   float z_vector[PHYSICAL_CONST_THREE_DIM],
                                                   float s_matrix[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM],
                                                   float s_square_matrix[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM],
                                             const float sun_ref_vec[PHYSICAL_CONST_THREE_DIM],
                                             const float mag_ref_vec[PHYSICAL_CONST_THREE_DIM],
                                             const float sun_obs_vec[PHYSICAL_CONST_THREE_DIM],
                                             const float mag_obs_vec[PHYSICAL_CONST_THREE_DIM],
                                             const float sun_weight,
                                             const float mag_weight);

/**
* @brief        Qmethodにおける固有方程式求解関数
* @param[in]    sigma           : 固有方程式のパラメータ σ
* @param[in]    kappa           : 3×3行列Sに対するtr(adj(S))
* @param[in]    delta           : 3×3行列Sに対するdet(S)
* @param[in]    s_matrix        : 固有方程式のパラメータになる3×3行列S
* @param[in]    s_square_matrix : 3×3行列Sに対するS^2
* @return       lambda          : 固有方程式の解λ
*/
static float APP_RTAD_solve_q_method_characteristic_equation_(const float sigma,
                                                             const float kappa,
                                                             const float delta,
                                                             const float z_vector[PHYSICAL_CONST_THREE_DIM],
                                                             const float s_matrix[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM],
                                                             const float s_square_matrix[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM]);

/**
* @brief        Qmethodにおける，固有方程式の解から姿勢変換クオータニオンを計算する関数
* @param[in]    lambda          : 固有方程式の解
* @param[in]    sigma           : 固有方程式のパラメータσ
* @param[in]    kappa           : 3×3行列Sに対するtr(adj(S))
* @param[in]    delta           : 3×3行列Sに対するdet(S)
* @param[in]    s_matrix        : 固有方程式のパラメータになる3×3行列S
* @param[in]    s_square_matrix : 3×3行列Sに対するS^2
* @param[in]    z_vector        : 固有方程式のパラメータになるベクトルz
* @return       q_eci_to_body   : 固有方程式の解から計算されたECI->body変換クオータニオン
*/
Quaternion APP_RTAD_calculate_q_method_quaternion(const float lambda,
                                                  const float sigma,
                                                  const float kappa,
                                                  const float delta,
                                                  const float s_matrix[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM],
                                                  const float s_square_matrix[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM],
                                                  const float z_vector[PHYSICAL_CONST_THREE_DIM]);

/**
* @brief   三軸姿勢決定メソッドが利用可能であるかを判定する関数
*
*          太陽方向ベクトル・磁場ベクトル・MTQ出力状態・太陽の可視状態を考慮して，TRIADやQmethodによる三軸姿勢決定が使用可能か調べる．
*          MTQがON，または消磁中の場合            ：MTQと磁気センサが干渉して正しい磁場観測値が得られないので，AOCS_ERROR_OTHERSを返す
*          太陽が不可視の場合                    ：太陽方向ベクトルが観測できないので，AOCS_ERROR_OTHERSを返す
*          太陽方向ベクトルと磁場ベクトルが平行の場合：外積のノルムが0になって正規直交基底を生成できないので，AOCS_ERROR_SINGULARを返す
* @param   sun_ref_vec      : 基準の座標系における太陽方向単位ベクトル
* @param   mag_ref_vec      : 基準の座標系における磁場単位ベクトル
* @param   sun_obs_vec      : 衛星が観測した太陽方向単位ベクトル
* @param   mag_obs_vec      : 衛星が観測した磁場単位ベクトル
* @param   mtq_output_state : MTQ出力状態
* @param   sun_visibility   : 太陽の可視状態
* @return  AOCS_ERRORに準拠
*/
static AOCS_ERROR APP_RTAD_judge_availability_(const float sun_ref_vec[PHYSICAL_CONST_THREE_DIM],
                                               const float mag_ref_vec[PHYSICAL_CONST_THREE_DIM],
                                               const float sun_obs_vec[PHYSICAL_CONST_THREE_DIM],
                                               const float mag_obs_vev[PHYSICAL_CONST_THREE_DIM],
                                               const APP_AOCS_MANAGER_MAGNETIC_EXCLUSIVE_CONTROL_STATE magnetic_exclusive_control_state,
                                               const AOCS_MANAGER_SUN_VISIBILITY   sun_visibility);

AppInfo APP_RTAD_create_app(void)
{
  return AI_create_app_info("Rough Three Axis Determination", APP_RTAD_init_, APP_RTAD_exec_);
}

static void APP_RTAD_init_(void)
{
  rough_three_axis_determination_.previous_obc_time = TMGR_get_master_clock();
  rough_three_axis_determination_.method = ATTITUDE_DETERMINATION_PARAMETERS_rtad_method;
  rough_three_axis_determination_.sun_invisible_propagation = ATTITUDE_DETERMINATION_PARAMETERS_rtad_sun_invisible_propagation;
  rough_three_axis_determination_.q_method_info.sun_vec_weight = ATTITUDE_DETERMINATION_PARAMETERS_q_method_sun_vec_weight;
  rough_three_axis_determination_.q_method_info.mag_vec_weight = 1.0f - rough_three_axis_determination_.q_method_info.sun_vec_weight;
}

static void APP_RTAD_exec_(void)
{
  // タイマアップデート
  ObcTime current_obc_time = TMGR_get_master_clock();
  float time_step_s = (float)OBCT_diff_in_sec(&(rough_three_axis_determination_.previous_obc_time), &current_obc_time);
  rough_three_axis_determination_.previous_obc_time = current_obc_time;
  if (time_step_s < 0.0f) return;  // 時間差が負の場合は一旦飛ばす
  if (time_step_s > aocs_manager->obct_diff_max_limit_s) return; // 時間差が大きすぎる場合は一旦飛ばす

  float sun_ref_vec[PHYSICAL_CONST_THREE_DIM]; //!< 基準の座標系における太陽方向単位ベクトル
  float mag_ref_vec[PHYSICAL_CONST_THREE_DIM]; //!< 基準の座標系における磁場単位ベクトル
  float sun_obs_vec[PHYSICAL_CONST_THREE_DIM]; //!< 衛星が観測した太陽方向単位ベクトル
  float mag_obs_vec[PHYSICAL_CONST_THREE_DIM]; //!< 衛星が観測した磁場単位ベクトル

  VECTOR3_normalize(sun_ref_vec, aocs_manager->sun_vec_est_eci);
  VECTOR3_normalize(mag_ref_vec, aocs_manager->mag_vec_est_eci_nT);
  VECTOR3_normalize(sun_obs_vec, aocs_manager->sun_vec_est_body);
  VECTOR3_normalize(mag_obs_vec, aocs_manager->mag_vec_est_body_nT);

  AOCS_ERROR sensor_availability = APP_RTAD_judge_availability_(sun_ref_vec, mag_ref_vec, sun_obs_vec, mag_obs_vec,
                                                                aocs_manager->magnetic_exclusive_control_timer_state, aocs_manager->sun_visibility);

  Quaternion q_eci_to_body;

  // 太陽センサ非可視時の対応
  if (sensor_availability != AOCS_ERROR_OK)
  {
    if (rough_three_axis_determination_.sun_invisible_propagation == APP_RTAD_SUN_INVISIBLE_PROPAGATION_QUATERNION)
    {
      // 三軸姿勢決定はせずにQuaternion伝播を行う
      q_eci_to_body =  QUATERNION_euler_propagate(aocs_manager->quaternion_est_i2b, aocs_manager->ang_vel_obs_body_rad_s, time_step_s);
      AOCS_MANAGER_set_quaternion_est_i2b(q_eci_to_body);
      return;
    }
    else
    {
      // 伝搬した太陽ベクトルと観測した磁場情報をつかって三軸姿勢決定を行う
      // 代入時にsun_vec_estを使っているので、ここでは何もする必要はない
    }
  }

  if (rough_three_axis_determination_.method == APP_RTAD_METHOD_TRIAD)
  {
    q_eci_to_body = APP_RTAD_triad_(sun_ref_vec, mag_ref_vec, sun_obs_vec, mag_obs_vec);
    AOCS_MANAGER_set_quaternion_est_i2b(q_eci_to_body);
  }
  else
  {
    q_eci_to_body = APP_RTAD_q_method_(sun_ref_vec, mag_ref_vec, sun_obs_vec, mag_obs_vec, rough_three_axis_determination_.q_method_info);
    AOCS_MANAGER_set_quaternion_est_i2b(q_eci_to_body);
  }
}

Quaternion APP_RTAD_triad_(const float sun_ref_vec[PHYSICAL_CONST_THREE_DIM],
                           const float mag_ref_vec[PHYSICAL_CONST_THREE_DIM],
                           const float sun_obs_vec[PHYSICAL_CONST_THREE_DIM],
                           const float mag_obs_vec[PHYSICAL_CONST_THREE_DIM])
{
  float triad_ref_mat[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM]; //!< 基準ベクトルから生成される正規直交行列
  float triad_obs_mat[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM]; //!< 観測ベクトルから生成される正規直交行列

  APP_RTAD_generate_triad_mat_(triad_ref_mat, sun_ref_vec, mag_ref_vec);
  APP_RTAD_generate_triad_mat_(triad_obs_mat, sun_obs_vec, mag_obs_vec);

  float triad_ref_mat_transpose[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM]; //!< triad_ref_matの転置行列
  MATRIX33_transpose(triad_ref_mat_transpose, triad_ref_mat);

  float dcm_eci_to_body[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM]; //!< ECI->body変換行列
  MATRIX33_multiply_matrix_matrix(dcm_eci_to_body, triad_obs_mat, triad_ref_mat_transpose);

  Quaternion q_eci_to_body;
  QUATERNION_make_from_matrix(&q_eci_to_body, dcm_eci_to_body);

  return q_eci_to_body;
}

static void APP_RTAD_generate_triad_mat_(float triad_mat[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM],
                                   const float sun_vec[PHYSICAL_CONST_THREE_DIM],
                                   const float mag_vec[PHYSICAL_CONST_THREE_DIM])
{
  float triad_vec_1[PHYSICAL_CONST_THREE_DIM]; //!< 太陽方向単位ベクトルと磁場単位ベクトルから生成される正規直交基底の1番目
  float triad_vec_2[PHYSICAL_CONST_THREE_DIM]; //!< 太陽方向単位ベクトルと磁場単位ベクトルから生成される正規直交基底の2番目
  float triad_vec_3[PHYSICAL_CONST_THREE_DIM]; //!< 太陽方向単位ベクトルと磁場単位ベクトルから生成される正規直交基底の3番目

  // 正規直交基底を生成する．
  // sun_vecとmag_vecが平行である場合は，それらの外積であるtriad_vec_2のノルムが0となり，正規化計算ができなくなるが，
  // そのような場合はあらかじめAPP_RTAD_judge_availability_で弾いている
  VECTOR3_copy(triad_vec_1, sun_vec);
  VECTOR3_outer_product(triad_vec_2, sun_vec, mag_vec);
  VECTOR3_normalize(triad_vec_2, triad_vec_2);
  VECTOR3_outer_product(triad_vec_3, triad_vec_1, triad_vec_2);
  VECTOR3_normalize(triad_vec_3, triad_vec_3);

  for (int i = 0; i < PHYSICAL_CONST_THREE_DIM; i++)
  {
    triad_mat[i][0] = triad_vec_1[i];
    triad_mat[i][1] = triad_vec_2[i];
    triad_mat[i][2] = triad_vec_3[i];
  }
}

Quaternion APP_RTAD_q_method_(const float sun_ref_vec[PHYSICAL_CONST_THREE_DIM],
                              const float mag_ref_vec[PHYSICAL_CONST_THREE_DIM],
                              const float sun_obs_vec[PHYSICAL_CONST_THREE_DIM],
                              const float mag_obs_vec[PHYSICAL_CONST_THREE_DIM],
                              const QMethodInfo q_method_info)
{
  float sigma;                                                               //!< 姿勢変換クオータニオンの最尤推定値を与えるゲインファンクションの，行列内σ成分
  float z_vector[PHYSICAL_CONST_THREE_DIM];                                  //!< 姿勢変換クオータニオンの最尤推定値を与えるゲインファンクションの，行列内z成分
  float s_matrix[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM];        //!< 姿勢変換クオータニオンの最尤推定値を与えるゲインファンクションの，行列内S成分
  float s_square_matrix[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM]; //!< S^2
  float delta;                                                               //!< det(S)
  float kappa;                                                               //!< tr(adj(S))
  float sun_weight = q_method_info.sun_vec_weight;
  float mag_weight = q_method_info.mag_vec_weight;

  // パラメータが多いので，関数でまとめて事前計算する
  APP_RTAD_calculate_q_method_parameters_(&sigma, &delta, &kappa, z_vector, s_matrix, s_square_matrix,
                                         sun_ref_vec, mag_ref_vec, sun_obs_vec, mag_obs_vec, sun_weight, mag_weight);

  float lambda; //!< 姿勢変換クオータニオンqに対する固有方程式 Kq = λqの解 λ
  lambda = APP_RTAD_solve_q_method_characteristic_equation_(sigma, kappa, delta, z_vector, s_matrix, s_square_matrix);

  Quaternion q_eci_to_body;
  q_eci_to_body = APP_RTAD_calculate_q_method_quaternion(lambda, sigma, kappa, delta, s_matrix, s_square_matrix, z_vector);

  return q_eci_to_body;
}



void APP_RTAD_calculate_q_method_parameters_(float* sigma,
                                            float* delta,
                                            float* kappa,
                                            float z_vector[PHYSICAL_CONST_THREE_DIM],
                                            float s_matrix[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM],
                                            float s_square_matrix[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM],
                                      const float sun_ref_vec[PHYSICAL_CONST_THREE_DIM],
                                      const float mag_ref_vec[PHYSICAL_CONST_THREE_DIM],
                                      const float sun_obs_vec[PHYSICAL_CONST_THREE_DIM],
                                      const float mag_obs_vec[PHYSICAL_CONST_THREE_DIM],
                                      const float sun_weight,
                                      const float mag_weight)
{
  // σの計算
  *sigma = sun_weight * VECTOR3_inner_product(sun_obs_vec, sun_ref_vec) +
           mag_weight * VECTOR3_inner_product(mag_obs_vec, mag_ref_vec);

  // zの計算
  float sun_outer_product[PHYSICAL_CONST_THREE_DIM]; //!< 衛星が観測した太陽方向単位ベクトルと，基準の座標系における太陽方向単位ベクトルとの外積
  float mag_outer_product[PHYSICAL_CONST_THREE_DIM]; //!< 衛星が観測した磁場方向単位ベクトルと，基準の座標系における磁場方向単位ベクトルとの外積
  VECTOR3_outer_product(sun_outer_product, sun_obs_vec, sun_ref_vec);
  VECTOR3_outer_product(mag_outer_product, mag_obs_vec, mag_ref_vec);
  float weighed_sun_outer_product[PHYSICAL_CONST_THREE_DIM]; //!< sun_outer_productに重みをつけたベクトル
  float weighed_mag_outer_product[PHYSICAL_CONST_THREE_DIM]; //!< mag_outer_productに重みをつけたベクトル
  VECTOR3_scalar_product(weighed_sun_outer_product, sun_weight, sun_outer_product);
  VECTOR3_scalar_product(weighed_mag_outer_product, mag_weight, mag_outer_product);
  VECTOR3_add(z_vector, weighed_sun_outer_product, weighed_mag_outer_product);

  // Sの計算
  float sun_tensor_product[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM]; //!< 衛星が観測した太陽方向単位ベクトルと，基準の座標系における太陽方向単位ベクトルとのテンソル積
  float mag_tensor_product[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM]; //!< 衛星が観測した太陽方向単位ベクトルと，基準の座標系における太陽方向単位ベクトルとのテンソル積
  VECTOR3_tensor_product(sun_tensor_product, sun_obs_vec, sun_ref_vec);
  VECTOR3_tensor_product(mag_tensor_product, mag_obs_vec, mag_ref_vec);
  float weighed_sun_tensor_product[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM]; //!< sun_outer_productに重みをつけた行列
  float weighed_mag_tensor_product[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM]; //!< mag_outer_productに重みをつけた行列
  MATRIX33_multiply_scalar_matrix(weighed_sun_tensor_product, sun_weight, sun_tensor_product);
  MATRIX33_multiply_scalar_matrix(weighed_mag_tensor_product, mag_weight, mag_tensor_product);
  float b_matrix[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM]; //!< Sを計算するための中間変数B
  MATRIX33_add(b_matrix, weighed_sun_tensor_product, weighed_mag_tensor_product);
  float b_matrix_transpose[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM]; //!< 行列Bの転置
  MATRIX33_transpose(b_matrix_transpose, b_matrix);
  MATRIX33_add(s_matrix, b_matrix, b_matrix_transpose);

  // 行列Sから求まるパラメータ（Δ，κ，S^2）を計算する
  *delta = MATRIX33_determinant(s_matrix);
  float s_matrix_adjugate[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM]; //!< Sの余因子行列
  MATRIX33_adjugate(s_matrix_adjugate, s_matrix);
  *kappa = MATRIX33_trace(s_matrix_adjugate);
  MATRIX33_multiply_matrix_matrix(s_square_matrix, s_matrix, s_matrix);
}

float APP_RTAD_solve_q_method_characteristic_equation_(const float sigma,
                                                      const float kappa,
                                                      const float delta,
                                                      const float z_vector[PHYSICAL_CONST_THREE_DIM],
                                                      const float s_matrix[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM],
                                                      const float s_square_matrix[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM])
{
  float a, b, c, d; //!< 固有方程式の係数
  a = sigma * sigma - kappa;
  b = sigma * sigma + VECTOR3_inner_product(z_vector, z_vector);
  c = delta + MATRIX33_multiply_matrix_vector_quadratic(s_matrix, z_vector);
  d = MATRIX33_multiply_matrix_vector_quadratic(s_square_matrix, z_vector);

  // ニュートン・ラフソン法により固有方程式を解く
  float lambda = 1.0f;                  //!< 固有方程式の解
  float lhs;                            //!< 固有方程式 f(λ) = 0の左辺
  float lhs_derivative;                  //!< f(λ)の微分

  for (int i = 0; i < APP_RTAD_kMaxNumOfIteration_; i++)
  {
    lhs = lambda * lambda * lambda * lambda - (a + b) * lambda * lambda - c * lambda + (a * b + c * sigma - d);
    lhs_derivative = 4.0f * lambda * lambda * lambda - 2.0f * (a + b) * lambda - c;
    float diff = lhs / lhs_derivative; //!< 解の変化量
    lambda -= diff;

    // 計算が収束したら，forループを抜ける TODO_L: 収束せずにイタレーション最大回数を迎えた場合の挙動を検討する
    if (fabs(diff) < APP_RTAD_kConvergenceThreshold_)
    {
      break;
    }
  }

  return lambda;
}

Quaternion APP_RTAD_calculate_q_method_quaternion(const float lambda,
                                                  const float sigma,
                                                  const float kappa,
                                                  const float delta,
                                                  const float s_matrix[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM],
                                                  const float s_square_matrix[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM],
                                                  const float z_vector[PHYSICAL_CONST_THREE_DIM])
{
  // 簡単のためにパラメータを定義する
  float alpha, beta, gamma; //!< 固有方程式の解から姿勢クオータニオンを計算するためのパラメータ
  alpha = lambda * lambda - sigma * sigma + kappa;
  beta = lambda - sigma;
  gamma = (lambda + sigma) * alpha - delta; // q_eci_to_bodyのスカラー部になる

  // 姿勢変換クオータニオンq_eci_to_bodyのベクトル部になる部分の計算を行う
  float x_vector[PHYSICAL_CONST_THREE_DIM]; //!< q_eci_to_bodyのベクトル成分を計算するための中間変数（Xベクトル）

  // X = (αI + βS + S^2)zを計算する
  float unit_matrix[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM]; // 単位行列
  MATRIX33_make_unit(unit_matrix);
  float alpha_i_matrix[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM]; //!< αI
  MATRIX33_multiply_scalar_matrix(alpha_i_matrix, alpha, unit_matrix);
  float beta_s_matrix[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM]; //!< βS
  MATRIX33_multiply_scalar_matrix(beta_s_matrix, beta, s_matrix);
  // (αI + βS + S^2)は一発で計算できないので，まずは(αI + βS)を計算し，それにS^2を足す
  float alpha_i_beta_s[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM]; //!< αI + βS
  MATRIX33_add(alpha_i_beta_s, alpha_i_matrix, beta_s_matrix);
  float alpha_i_beta_s_s_square[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM]; //!< αI + βS + S^2
  MATRIX33_add(alpha_i_beta_s_s_square, alpha_i_beta_s, s_square_matrix);
  MATRIX33_multiply_matrix_vector(x_vector, alpha_i_beta_s_s_square, z_vector);

  // 姿勢変換クオータニオン normalize(X, γ) の計算
  Quaternion q_eci_to_body; //!< ECI->body変換クオータニオン
  QUATERNION_make_from_vector_scalar(&q_eci_to_body, x_vector, gamma);

  return q_eci_to_body;
}

static AOCS_ERROR APP_RTAD_judge_availability_(const float sun_ref_vec[PHYSICAL_CONST_THREE_DIM],
                                                const float mag_ref_vec[PHYSICAL_CONST_THREE_DIM],
                                                const float sun_obs_vec[PHYSICAL_CONST_THREE_DIM],
                                                const float mag_obs_vev[PHYSICAL_CONST_THREE_DIM],
                                                const APP_AOCS_MANAGER_MAGNETIC_EXCLUSIVE_CONTROL_STATE magnetic_exclusive_control_state,
                                                const AOCS_MANAGER_SUN_VISIBILITY   sun_visibility)
{
  float ref_outer_product[PHYSICAL_CONST_THREE_DIM]; //!< 基準となる座標系における太陽方向単位ベクトルと磁場単位ベクトルとの外積
  float obs_outer_product[PHYSICAL_CONST_THREE_DIM]; //!< 衛星が観測した太陽方向単位ベクトルと磁場単位ベクトルとの外積
  VECTOR3_outer_product(ref_outer_product, sun_ref_vec, mag_ref_vec);
  VECTOR3_outer_product(obs_outer_product, sun_obs_vec, mag_obs_vev);

  if (magnetic_exclusive_control_state != APP_AOCS_MANAGER_MAGNETIC_EXCLUSIVE_CONTROL_STATE_OBSERVE)
  {
    return AOCS_ERROR_OTHERS;
  }
  else if (sun_visibility != AOCS_MANAGER_SUN_VISIBLE)
  {
    return AOCS_ERROR_OTHERS;
  }
  else if (VECTOR3_norm(ref_outer_product) < MATH_CONST_PROHIBIT_DIVISION_VALUE)
  {
    return AOCS_ERROR_SINGULAR;
  }
  else if (VECTOR3_norm(obs_outer_product) < MATH_CONST_PROHIBIT_DIVISION_VALUE)
  {
    return AOCS_ERROR_SINGULAR;
  }
  else
  {
    return AOCS_ERROR_OK;
  }
}

CCP_CmdRet Cmd_APP_RTAD_SET_METHOD(const CommonCmdPacket* packet)
{
  const uint8_t* param = CCP_get_param_head(packet);

  APP_RTAD_METHOD method = (APP_RTAD_METHOD)param[0];
  APP_RTAD_SUN_INVISIBLE_PROPAGATION sun_invisible_propagation = (APP_RTAD_SUN_INVISIBLE_PROPAGATION)param[1];

  if (method >= APP_RTAD_METHOD_MAX) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
  if (sun_invisible_propagation >= APP_RTAD_SUN_INVISIBLE_PROPAGATION_MAX) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);

  rough_three_axis_determination_.method = method;
  rough_three_axis_determination_.sun_invisible_propagation = sun_invisible_propagation;

  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

CCP_CmdRet Cmd_APP_RTAD_SET_QMETHOD_SUN_VEC_WEIGHT(const CommonCmdPacket* packet)
{
  const uint8_t* param = CCP_get_param_head(packet);

  float sun_vec_weight;

  ENDIAN_memcpy(&sun_vec_weight, param, sizeof(float));

  C2A_MATH_ERROR ret;
  ret = C2A_MATH_check_range_violation(sun_vec_weight, 1.0f, 0.0f);

  if (ret != C2A_MATH_ERROR_OK)
  {
    return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
  }

  rough_three_axis_determination_.q_method_info.sun_vec_weight = sun_vec_weight;
  rough_three_axis_determination_.q_method_info.mag_vec_weight = 1.0f - sun_vec_weight;

  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

#pragma section
