#pragma section REPRO
/**
* @file   unloading.c
* @brief  アンローディングのためのアプリケーション
*/

#include "unloading.h"

#include <math.h>

#include <src_core/Library/endian.h>
#include <src_core/TlmCmd/common_cmd_packet_util.h>
#include <src_user/Library/vector3.h>
#include <src_user/Applications/UserDefined/AOCS/HardwareDependent/ActuatorControllers/mtq_seiren_controller.h>

// Satellite Parameters
#include <src_user/Settings/SatelliteParameters/attitude_control_parameters.h>

static Unloading        unloading_;
const  Unloading* const unloading = &unloading_;

static void APP_UNLOADING_init_(void);

static void APP_UNLOADING_exec_(void);

/**
 * @brief  アンローディング状態更新関数
 *         RWの回転数から，そのRWにアンローディングのためのMTQ入力を入れるかどうか判断する関数
 * @param  void
 * @return void
 */
static void APP_UNLOADING_update_unloading_state_(void);

/**
 * @brief  MTQ出力トルク計算関数
 *         RWの回転数から，アンローディングトルクを計算する関数
 * @param  void
 * @return void
 */
static void APP_UNLOADING_calc_output_torque(void);

AppInfo APP_UNLOADING_create_app(void)
{
  return AI_create_app_info("Unloading", APP_UNLOADING_init_, APP_UNLOADING_exec_);
}

static void APP_UNLOADING_init_(void)
{
  unloading_.angular_velocity_upper_threshold_rad_s = ATTITUDE_CONTROL_PARAMETERS_unloading_angular_velocity_upper_threshold_rad_s;
  unloading_.angular_velocity_lower_threshold_rad_s = ATTITUDE_CONTROL_PARAMETERS_unloading_angular_velocity_lower_threshold_rad_s;
  unloading_.angular_velocity_target_rad_s = ATTITUDE_CONTROL_PARAMETERS_unloading_angular_velocity_target_rad_s;

  unloading_.control_gain = ATTITUDE_CONTROL_PARAMETERS_unloading_control_gain;

  unloading_.exec_is_enable = ATTITUDE_CONTROL_PARAMETERS_unloading_exec_is_enable;

  for (int idx = 0; idx < AOCS_MANAGER_NUM_OF_RW; idx++)
  {
    unloading_.unloading_state[idx] = APP_UNLOADING_STATE_OFF;
  }

  unloading_.number_of_unloading_wheels = 0;
  CROSS_PRODUCT_CONTROL_init(&unloading_.cross_product_cntrl);
}

void APP_UNLOADING_exec_(void)
{
  // アンローディング処理を実施しないならば，MTQ/磁気センサ排他制御はIDLEにしておく
  if (unloading_.exec_is_enable == APP_UNLOADING_EXEC_DISABLE)
  {
    AOCS_MANAGER_set_mag_exclusive_state(AOCS_MANAGER_MAG_EXCLUSIVE_STATE_IDLE);
  }

  APP_UNLOADING_update_unloading_state_();
  APP_UNLOADING_calc_output_torque();

  // アンローディング処理がenableになっているときのみ，出力目標トルクをAOCS Manegerにセットする
  // disableのときは，出力目標トルクのセットはしないが，トルク計算自体はcalc_torque_outputで行われている．
  if (unloading_.exec_is_enable == APP_UNLOADING_EXEC_ENABLE)
  {
    AOCS_MANAGER_set_external_torque_target_body_Nm(unloading_.output_torque_body_Nm);
    APP_MTQ_SEIREN_CONTROLLER_set_cross_product_control_output(unloading_.cross_product_cntrl);

    if (unloading_.number_of_unloading_wheels > 0)
    {
      AOCS_MANAGER_set_mag_exclusive_state(AOCS_MANAGER_MAG_EXCLUSIVE_STATE_ACTIVE);
    }
    else
    {
      AOCS_MANAGER_set_mag_exclusive_state(AOCS_MANAGER_MAG_EXCLUSIVE_STATE_IDLE);
    }
  }
}

static void APP_UNLOADING_update_unloading_state_(void)
{
  unloading_.number_of_unloading_wheels = 0;

  for (int idx = 0; idx < AOCS_MANAGER_NUM_OF_RW; idx++)
  {
    if (unloading_.unloading_state[idx] == APP_UNLOADING_STATE_OFF)
    {
      if (aocs_manager->rw_angular_velocity_rad_s[idx] > unloading_.angular_velocity_upper_threshold_rad_s)
      { // 回転数が上側閾値を超えたら，回転数を下げるためのアンローディングを開始する．
        unloading_.unloading_state[idx] = APP_UNLOADING_STATE_ON_DECREASE;
      }
      else if (aocs_manager->rw_angular_velocity_rad_s[idx] < unloading_.angular_velocity_lower_threshold_rad_s)
      { // 回転数が下側閾値を超えたら，回転数を上げるためのアンローディングを開始する．
        unloading_.unloading_state[idx] = APP_UNLOADING_STATE_ON_INCREASE;
      }
      else
      {
        // NOT REACHED
      }
    }
    else if (unloading_.unloading_state[idx] == APP_UNLOADING_STATE_ON_INCREASE)
    {
      // アンローディングによって回転数が目標値を上回ったら，アンローディングを終了する．
      if (aocs_manager->rw_angular_velocity_rad_s[idx] >= unloading_.angular_velocity_target_rad_s)
      {
        unloading_.unloading_state[idx] = APP_UNLOADING_STATE_OFF;
      }
    }
    else if (unloading_.unloading_state[idx] == APP_UNLOADING_STATE_ON_DECREASE)
    {
      // アンローディングによって回転数が目標値を下回ったら，アンローディングを終了する．
      if (aocs_manager->rw_angular_velocity_rad_s[idx] < unloading_.angular_velocity_target_rad_s)
      {
        unloading_.unloading_state[idx] = APP_UNLOADING_STATE_OFF;
      }
    }
    else
    {
      // NOT REACHED
    }

    // count up number of wheels under unloading
    if (unloading_.unloading_state[idx] != APP_UNLOADING_STATE_OFF)
    {
      unloading_.number_of_unloading_wheels++;
    }
  }
}

void APP_UNLOADING_calc_output_torque(void)
{
  VECTOR3_initialize(unloading_.output_torque_body_Nm, 0.0f);

  for (int idx = 0; idx < AOCS_MANAGER_NUM_OF_RW; idx++)
  {
    // アンローディングが必要なRWについては，必要トルクを計算し，出力トルクに入れる
    if (unloading_.unloading_state[idx] != APP_UNLOADING_STATE_OFF)
    {
      float exceed_angular_velocity_rad_s = aocs_manager->rw_angular_velocity_rad_s[idx] - unloading_.angular_velocity_target_rad_s;
      float exceed_angular_velocity_vec_rad_s[PHYSICAL_CONST_THREE_DIM]; //!< 目標回転角速度に対して、余分に持っている回転角速度ベクトル
      VECTOR3_scalar_product(exceed_angular_velocity_vec_rad_s, exceed_angular_velocity_rad_s, aocs_manager->rw_rotation_direction_matrix[idx]);
      float output_torque_tmp_Nm[PHYSICAL_CONST_THREE_DIM]; //!< その軸のRWをアンローディングするのに必要な出力トルク
      VECTOR3_scalar_product(output_torque_tmp_Nm, unloading_.control_gain, exceed_angular_velocity_vec_rad_s);
      VECTOR3_add(unloading_.output_torque_body_Nm, unloading_.output_torque_body_Nm, output_torque_tmp_Nm);
    }
  }
}

CCP_CmdRet Cmd_APP_UNLOADING_SET_ENABLE(const CommonCmdPacket* packet)
{
  const uint8_t* param = CCP_get_param_head(packet);
  uint8_t exec_is_enable;

  ENDIAN_memcpy(&exec_is_enable, param, sizeof(uint8_t));

  if (((APP_UNLOADING_EXEC)exec_is_enable != APP_UNLOADING_EXEC_ENABLE) && ((APP_UNLOADING_EXEC)exec_is_enable != APP_UNLOADING_EXEC_DISABLE))
  {
    return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
  }

  unloading_.exec_is_enable = (APP_UNLOADING_EXEC)exec_is_enable;
  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

CCP_CmdRet Cmd_APP_UNLOADING_SET_ANGULAR_VEROCITY_THRESHOLD(const CommonCmdPacket* packet)
{
  const uint8_t* param = CCP_get_param_head(packet);
  int16_t angular_velocity_upper_threshold_rpm;
  int16_t angular_velocity_target_rpm;
  int16_t angular_velocity_lower_threshold_rpm;

  ENDIAN_memcpy(&angular_velocity_upper_threshold_rpm, param, sizeof(int16_t));
  ENDIAN_memcpy(&angular_velocity_target_rpm, param + sizeof(int16_t),  sizeof(int16_t));
  ENDIAN_memcpy(&angular_velocity_lower_threshold_rpm, param + sizeof(int16_t) + sizeof(int16_t), sizeof(int16_t));

  float angular_velocity_upper_threshold_rad_s = PHYSICAL_CONST_rpm_to_rad_sec((float)angular_velocity_upper_threshold_rpm);
  float angular_velocity_target_rad_s = PHYSICAL_CONST_rpm_to_rad_sec((float)angular_velocity_target_rpm);
  float angular_velocity_lower_threshold_rad_s = PHYSICAL_CONST_rpm_to_rad_sec((float)angular_velocity_lower_threshold_rpm);

  // RW回転数の閾値について，下限値 < 目標値 < 上限値 が満たされていないとアンローディング動作がおかしくなるので，満たしていないときはセットせずにリターン
  if ((angular_velocity_upper_threshold_rad_s < angular_velocity_target_rad_s) &&
      (angular_velocity_lower_threshold_rad_s > angular_velocity_target_rad_s))
  {
    return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_CONTEXT);
  }

  unloading_.angular_velocity_upper_threshold_rad_s = angular_velocity_upper_threshold_rad_s;
  unloading_.angular_velocity_target_rad_s = angular_velocity_target_rad_s;
  unloading_.angular_velocity_lower_threshold_rad_s = angular_velocity_lower_threshold_rad_s;

  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

CCP_CmdRet Cmd_APP_UNLOADING_SET_CONTROL_GAIN(const CommonCmdPacket* packet)
{
  const uint8_t* param = CCP_get_param_head(packet);
  float control_gain;

  ENDIAN_memcpy(&control_gain, param, sizeof(float));

  if (control_gain > 0.0)
  {
    return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
  }

  unloading_.control_gain = control_gain;
  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}
#pragma section
