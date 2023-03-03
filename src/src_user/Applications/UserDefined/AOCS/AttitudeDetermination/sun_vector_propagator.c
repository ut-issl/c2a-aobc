#pragma section REPRO
/**
* @file    sun_vector_propagator.c
* @brief   太陽方向ベクトル伝搬アプリ
*
*          最後にサンセンサによって太陽方向が取れた時の姿勢を基準姿勢とする．基準姿勢から今の姿勢への変換クオータニオンを計算し，それを使って今の太陽方向ベクトルを計算する．
*/

#include "sun_vector_propagator.h"

#include "../../../../Library/vector3.h"
#include "../../../../Library/matrix33.h"
#include "../../../../Library/quaternion.h"
#include "../aocs_manager.h"

static SunVectorPropagator        sun_vector_propagator_;
const  SunVectorPropagator* const sun_vector_propagator = &sun_vector_propagator_;

static void APP_SUN_VEC_PROPAGATOR_init_(void);

static void APP_SUN_VEC_PROPAGATOR_exec_(void);

AppInfo APP_SUN_VEC_PROPAGATOR_create_app(void)
{
  return AI_create_app_info("SunVectorPropagator", APP_SUN_VEC_PROPAGATOR_init_, APP_SUN_VEC_PROPAGATOR_exec_);
}

static void APP_SUN_VEC_PROPAGATOR_init_(void)
{
  VECTOR3_initialize(sun_vector_propagator_.last_seen_sun_vec_body, 0.0f);
  sun_vector_propagator_.last_seen_sun_vec_body[0] = 1.0f;
  sun_vector_propagator_.quaternion_sun_ref_to_body = QUATERNION_make_unit();
  sun_vector_propagator_.previous_obc_time = TMGR_get_master_clock();
}

void APP_SUN_VEC_PROPAGATOR_exec_(void)
{
  if (aocs_manager->sun_visibility == AOCS_MANAGER_SUN_VISIBLE)
  {
    VECTOR3_copy(sun_vector_propagator_.last_seen_sun_vec_body, aocs_manager->sun_vec_est_body);
    // サンセンサ情報で太陽方向ベクトルが見えているときは，現在の姿勢が「最後にサンセンサによって太陽方向が取れた時の姿勢」と等しくなる
    sun_vector_propagator_.quaternion_sun_ref_to_body = QUATERNION_make_unit();
    // 太陽が見えているときでも，内部の時刻情報の更新だけはしておく
    sun_vector_propagator_.previous_obc_time = TMGR_get_master_clock();
    AOCS_MANAGER_set_sun_vec_est_body(sun_vector_propagator_.last_seen_sun_vec_body);
    return;
  }

  Quaternion quaternion_time_derivative;
  quaternion_time_derivative = QUATERNION_calc_time_derivative(sun_vector_propagator->quaternion_sun_ref_to_body,
                                                               aocs_manager->ang_vel_est_body_rad_s);

  ObcTime current_obc_time = TMGR_get_master_clock();
  float time_step_s = (float)OBCT_diff_in_sec(&(sun_vector_propagator_.previous_obc_time), &current_obc_time);
  sun_vector_propagator_.previous_obc_time = current_obc_time;

  // Quaternionライブラリにはスカラー倍や和がないので，q + dqを計算するために一度配列にしてから計算する
  float quaternion_previous_array[PHYSICAL_CONST_QUATERNION_DIM];        //!< 前回のquaternion_sun_ref_to_body
  float quaternion_time_derivative_array[PHYSICAL_CONST_QUATERNION_DIM]; //!< quaternion_sun_ref_to_bodyの時間微分
  float quaternion_diff_array[PHYSICAL_CONST_QUATERNION_DIM];            //!< quaternion_sun_ref_to_bodyの微小変化
  float quaternion_current_array[PHYSICAL_CONST_QUATERNION_DIM];         //!< 新しいquaternion_sun_ref_to_body

  QUATERNION_to_array(quaternion_previous_array, sun_vector_propagator->quaternion_sun_ref_to_body, QUATERNION_SCALAR_POSITION_LAST);
  QUATERNION_to_array(quaternion_time_derivative_array, quaternion_time_derivative, QUATERNION_SCALAR_POSITION_LAST);

  for (int i = 0; i < PHYSICAL_CONST_QUATERNION_DIM; i++)
  {
    quaternion_diff_array[i] = time_step_s * quaternion_time_derivative_array[i];
    quaternion_current_array[i] = quaternion_previous_array[i] + quaternion_diff_array[i];
  }

  // q + dqを規格化しながら，quaternion_sun_ref_to_bodyにセットする
  QUATERNION_make_from_array(&(sun_vector_propagator_.quaternion_sun_ref_to_body),
                             quaternion_current_array,
                             QUATERNION_SCALAR_POSITION_LAST);

  float sun_vec_body[PHYSICAL_CONST_THREE_DIM];
  QUATERNION_trans_coordinate(sun_vec_body,
                              sun_vector_propagator->quaternion_sun_ref_to_body,
                              sun_vector_propagator->last_seen_sun_vec_body);

  AOCS_MANAGER_set_sun_vec_est_body(sun_vec_body);
}

#pragma section
