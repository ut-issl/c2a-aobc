#pragma section REPRO
/**
* @file   rw_controller.h
* @brief  aocs_managerの目標トルク情報から，RWの出力トルクを計算・設定するアプリ
*/

#include "rw_controller.h"

#include <src_user/Library/vector3.h>
#include <src_user/Library/matrix33.h>
#include <src_user/Library/physical_constants.h>
#include <src_user/Library/math_constants.h>

#include <src_core/System/EventManager/event_logger.h>
#include <src_user/Applications/app_registry.h>

static void APP_RW_CONTROLLER_init_(void);
static void APP_RW_CONTROLLER_exec_(void);

AppInfo APP_RW_CONTROLLER_create_app(void)
{
  return AI_create_app_info("RwController", APP_RW_CONTROLLER_init_, APP_RW_CONTROLLER_exec_);
}

void APP_RW_CONTROLLER_init_(void)
{
}

void APP_RW_CONTROLLER_exec_(void)
{
  float torque_rw_Nm[RW0003_IDX_MAX];
  // TODO_L: 4つ以上のRWを搭載することになったら4つ以上のRWに対応できるようにする
  MATRIX33_multiply_matrix_vector(torque_rw_Nm, aocs_manager->rw_distribution_matrix,
                                aocs_manager->internal_torque_target_body_Nm);
  VECTOR3_scalar_product(torque_rw_Nm, -1.0f, torque_rw_Nm); // RW司令は逆方向

  for (int idx_rw = 0; idx_rw < RW0003_IDX_MAX; idx_rw++)
  {
    AOCS_ERROR ret = DI_RW0003_set_torque_Nm((RW0003_IDX)idx_rw, torque_rw_Nm[idx_rw]);  // 実機確認で問題ないことを確認したが要注意
    if (ret != AOCS_ERROR_OK)
    {
      EL_GROUP el_group_error = (EL_GROUP)(EL_GROUP_ERROR_RW0003X + idx_rw);
      EL_record_event(el_group_error, (uint32_t)idx_rw, EL_ERROR_LEVEL_LOW, (uint32_t)RW0003_EL_NOTE_TORQUE_CMD);
    }
  }


  /*
    // 回転数指令でのRW制御    TODO:今後トルク指令との場合分けを行う
    static int cnt_come_here = 0;
    if (0 < cnt_come_here++ % rw_controller_.set_speed_interval)
    {
      return;
    }

    // 時刻差分計算
    ObcTime current_obc_time = TMGR_get_master_clock();
    float dt_s = (float)OBCT_diff_in_sec(&rw_controller_.previous_obc_time, &current_obc_time);
    if (dt_s < 0.0f || dt_s > 10.0f)
    {
      // 時刻差分が異常値の場合は何もせずに終了し、次のタイミングで正常になることを期待する
      rw_controller_.previous_obc_time = TMGR_get_master_clock();
      return;
    }

    float acceleration_rw_rad_s2[CUBE_WHEEL_IDX_MAX];
    float calc_speed_rw_rad_s[CUBE_WHEEL_IDX_MAX];
    float filtered_target_speed_rw_rad_s[CUBE_WHEEL_IDX_MAX];
    float target_speed_rw_rpm[CUBE_WHEEL_IDX_MAX];
    for (uint8_t idx = 0; idx < CUBE_WHEEL_IDX_MAX; idx++)
    {
      // トルク制限
      float max_torque = 0.00023f;
      float min_torque = -max_torque;
      if (max_torque < torque_rw_Nm[idx])
      {
        torque_rw_Nm[idx] = max_torque;
      }
      else if (torque_rw_Nm[idx] < min_torque)
      {
        torque_rw_Nm[idx] = min_torque;
      }

      // 積分計算
      acceleration_rw_rad_s2[idx] = torque_rw_Nm[idx] / aocs_manager->rw_moment_of_inertia_kgm2[idx] * rw_controller_.torque_scaler;
      calc_speed_rw_rad_s[idx] = aocs_manager->rw_angular_velocity_rad_s[idx] + acceleration_rw_rad_s2[idx] * dt_s;
      // LPF
      filtered_target_speed_rw_rad_s[idx] = Z_FILTER_calc_output(&rw_controller_.lpf_ctrl[idx], calc_speed_rw_rad_s[idx]);
      // 指令
      target_speed_rw_rpm[idx] = PHYSICAL_CONST_rad_sec_to_rpm(filtered_target_speed_rw_rad_s[idx]);
      DI_CUBE_WHEEL_set_speed_rpm((CUBE_WHEEL_IDX)idx, target_speed_rw_rpm[idx]);
    }
    rw_controller_.previous_obc_time = TMGR_get_master_clock();
  */
}

#pragma section
