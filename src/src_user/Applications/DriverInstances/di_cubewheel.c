#pragma section REPRO
/**
* @file   di_cubewheel.c
* @brief  CUBEWHEELのDriver Instance
*/

#include "di_cubewheel.h"

#include <src_core/Library/print.h>
#include <src_user/Settings/port_config.h>
#include <src_user/Settings/CmdTlm/common_tlm_cmd_packet_define.h>
#include <src_user/Applications/UserDefined/Power/power_switch_control.h>
#include "../UserDefined/anomaly_control.h"
#include "../UserDefined/AOCS/aocs_manager.h"
#include <src_user/Library/user_utility.h>
#include <src_core/Drivers/Super/driver_super.h>
#include "../../Library/vector.h"
#include "../../Library/c2a_math.h"


static CUBEWHEEL_Driver cubewheel_driver_[CUBEWHEEL_IDX_MAX];
const  CUBEWHEEL_Driver* const cubewheel_driver[CUBEWHEEL_IDX_MAX] = { &cubewheel_driver_[CUBEWHEEL_IDX_ON_X],
                                                                      &cubewheel_driver_[CUBEWHEEL_IDX_ON_Y],
                                                                      &cubewheel_driver_[CUBEWHEEL_IDX_ON_Z] };

static float DI_CUBEWHEEL_rw_speed_rad_s_[CUBEWHEEL_IDX_MAX] = { 0.0f, 0.0f, 0.0f };
static uint8_t DI_CUBEWHEEL_is_initialized_[3] = { 0, 0, 0 };  //!< 0 = not initialized, 1 = initialized
static uint8_t DI_CUBEWHEEL_is_enabled_[3] = { 0, 0, 0 };  //!< 0 = not initialized, 1 = initialized

static void DI_CUBEWHEEL_init_(void);
static void DI_CUBEWHEEL_update_(void);

/**
 * @brief  AOCS MANAGERに方向行列をセットする
 * @return AOCS_MANAGER_ERRORに準拠
 */
static AOCS_MANAGER_ERROR DI_CUBEWHEEL_set_direction_matrix_to_aocs_manager_(void);

AppInfo DI_CUBEWHEEL_update(void)
{
  return create_app_info("update_DI_CUBEWHEEL", DI_CUBEWHEEL_init_, DI_CUBEWHEEL_update_);
}


static void DI_CUBEWHEEL_init_(void)
{
  CUBEWHEEL_init(&cubewheel_driver_[CUBEWHEEL_IDX_ON_X], PORT_CH_I2C_2, I2C_DEVICE_ADDR_RW_X, PORT_CH_GPIO_OUT_RWX_EN, RW_AXIS_ID_X);
  CUBEWHEEL_init(&cubewheel_driver_[CUBEWHEEL_IDX_ON_Y], PORT_CH_I2C_2, I2C_DEVICE_ADDR_RW_Y, PORT_CH_GPIO_OUT_RWY_EN, RW_AXIS_ID_Y);
  CUBEWHEEL_init(&cubewheel_driver_[CUBEWHEEL_IDX_ON_Z], PORT_CH_I2C_2, I2C_DEVICE_ADDR_RW_Z, PORT_CH_GPIO_OUT_RWZ_EN, RW_AXIS_ID_Z);

  // 回転方向設定
  float direction_x[] = { -1.0f, 0.0f, 0.0f };
  CUBEWHEEL_set_rotation_direction_b(&cubewheel_driver_[CUBEWHEEL_IDX_ON_X], direction_x);
  float direction_y[] = { 0.0f, -1.0f, 0.0f };
  CUBEWHEEL_set_rotation_direction_b(&cubewheel_driver_[CUBEWHEEL_IDX_ON_Y], direction_y);
  float direction_z[] = { 0.0f, 0.0f, -1.0f };
  CUBEWHEEL_set_rotation_direction_b(&cubewheel_driver_[CUBEWHEEL_IDX_ON_Z], direction_z);

  AOCS_MANAGER_ERROR ret_manager = DI_CUBEWHEEL_set_direction_matrix_to_aocs_manager_();
  if (ret_manager != AOCS_MANAGER_ERROR_OK) Printf("RW direction setting Failed ! %d \n", ret_manager);
}


static void DI_CUBEWHEEL_update_(void)
{
  uint8_t i;

  // 電源OFFの場合、角速度は0に設定する
  if (power_switch_control->switch_state_unreg[APP_PSC_UNREG_IDX_RW] == APP_PSC_STATE_OFF)
  {
    for (i = 0; i < CUBEWHEEL_IDX_MAX; i++)
    {
      DI_CUBEWHEEL_is_initialized_[i] = 0;
      DI_CUBEWHEEL_rw_speed_rad_s_[i] = 0.0f;
    }
    return;
  }

  for (i = 0; i < CUBEWHEEL_IDX_MAX; i++)
  {
    if (cubewheel_driver[i]->info.valid)
    {
      uint8_t last_result;

      if (cubewheel_driver[i]->info.backup_mode_state == 1)
      {
        last_result = CUBEWHEEL_GetWheelDuty(&cubewheel_driver_[i]);
        DI_CUBEWHEEL_rw_speed_rad_s_[i] = PHYSICAL_CONST_rpm_to_rad_sec(cubewheel_driver_[i].info.speed_backup_in_rpm);
      }
      else
      {
        last_result = CUBEWHEEL_GetWheelData(&cubewheel_driver_[i]);
        DI_CUBEWHEEL_rw_speed_rad_s_[i] = PHYSICAL_CONST_rpm_to_rad_sec(cubewheel_driver_[i].info.speed_in_rpm);
      }
      AOCS_MANAGER_set_rw_angular_velocity_rad_s(DI_CUBEWHEEL_rw_speed_rad_s_);

      // アノマリ処理
      APP_ANOMALY_IDX idx = (APP_ANOMALY_IDX)(APP_ANOMALY_IDX_COMM_RWX + i);
      if (APP_ANOMALY_SET_LAST_RESULT(idx, (DS_CMD_ERR_CODE)last_result) == APP_ANOMALY_STATE_TO_ON)
      {
        // Printf("to initial by RW(%d) comm error.\n", i);App内のPrintは避ける
        APP_ANOMALY_TRANSITION_TO_INITIAL();
      }
    }
  }
}

static AOCS_MANAGER_ERROR DI_CUBEWHEEL_set_direction_matrix_to_aocs_manager_(void)
{
  float rw_rotation_direction_matrix[AOCS_MANAGER_NUM_OF_RW][PHYSICAL_CONST_THREE_DIM];
  for (int idx = 0; idx < AOCS_MANAGER_NUM_OF_RW; idx++)
  {
    VECTOR_copy(rw_rotation_direction_matrix[idx], cubewheel_driver_[idx].info.rotation_direction_b);
  }
  AOCS_MANAGER_ERROR ret = AOCS_MANAGER_set_rw_rotation_direction_matrix(rw_rotation_direction_matrix);
  return ret;
}

CCP_EXEC_STS Cmd_DI_CUBEWHEEL_Enable(const CTCP* packet)
{
  DS_CMD_ERR_CODE ret;

  // TODO: 処理を書く
  for (int idx = 0; idx < CUBEWHEEL_IDX_MAX; idx++)
  {
    ret = CUBEWHEEL_Enable(&cubewheel_driver_[idx]);
    if (ret == DS_CMD_OK)
    {
      DI_CUBEWHEEL_is_enabled_[idx] = 1;
    }
  }

  return DS_conv_cmd_err_to_ccp_exec_sts(ret);
}

CCP_EXEC_STS Cmd_DI_CUBEWHEEL_Init(const CTCP* packet)
{
  DS_CMD_ERR_CODE ret;

  // TODO: 処理を書く
  for (int idx = 0; idx < CUBEWHEEL_IDX_MAX; idx++)
  {
    if (DI_CUBEWHEEL_is_enabled_[idx] == 1)
    {
      ret = CUBEWHEEL_Startup(&cubewheel_driver_[idx]);
      if (ret == DS_CMD_OK)
      {
        DI_CUBEWHEEL_is_initialized_[idx] = 1;
      }
    }
  }

  return DS_conv_cmd_err_to_ccp_exec_sts(ret);
}


CCP_EXEC_STS Cmd_DI_CUBEWHEEL_EndProcess(const CTCP* packet)
{
  DS_CMD_ERR_CODE ret;

  // TODO: 処理を書く
  for (int idx = 0; idx < CUBEWHEEL_IDX_MAX; idx++)
  {
    ret = CUBEWHEEL_Disable(&cubewheel_driver_[idx]);
    DI_CUBEWHEEL_is_enabled_[idx] = 0;
  }

  return DS_conv_cmd_err_to_ccp_exec_sts(ret);
}

CCP_EXEC_STS Cmd_DI_CUBEWHEEL_SetSpeed(const CTCP* packet)
{
  const uint8_t* param = CCP_get_param_head(packet);
  CUBEWHEEL_IDX cubewheel_idx;
  uint8_t raw_val[4] = { 0,0,0,0 };
  float f_speed = 0.0f;

  cubewheel_idx = (CUBEWHEEL_IDX)param[0];
  if (cubewheel_idx > CUBEWHEEL_IDX_MAX)
  {
    return CCP_EXEC_ILLEGAL_PARAMETER;
  }

  raw_val[0] = param[1];
  raw_val[1] = param[2];

  int16_t int16_val = 0;
  *(((unsigned char*)&int16_val) + 0) = raw_val[1];
  *(((unsigned char*)&int16_val) + 1) = raw_val[0];

  int16_t speed = int16_val;

  CUBEWHEEL_SetReferenceSpeed(&cubewheel_driver_[cubewheel_idx], speed);

  // return DS_conv_cmd_err_to_ccp_exec_sts(ret);
  return CCP_EXEC_SUCCESS;

}

CCP_EXEC_STS Cmd_DI_CUBEWHEEL_SetDutyCycle(const CTCP* packet)
{
  const uint8_t* param = CCP_get_param_head(packet);
  CUBEWHEEL_IDX cubewheel_idx;
  uint8_t raw_val[4] = { 0,0,0,0 };
  float f_duty = 0.0f;

  cubewheel_idx = (CUBEWHEEL_IDX)param[0];
  if (cubewheel_idx > CUBEWHEEL_IDX_MAX)
  {
    return CCP_EXEC_ILLEGAL_PARAMETER;
  }

  raw_val[0] = param[1];
  raw_val[1] = param[2];

  int16_t int16_val = 0;
  *(((unsigned char*)&int16_val) + 0) = raw_val[1];
  *(((unsigned char*)&int16_val) + 1) = raw_val[0];

  int16_t duty = int16_val;

  CUBEWHEEL_SetDutyCycle(&cubewheel_driver_[cubewheel_idx], duty);

  // return DS_conv_cmd_err_to_ccp_exec_sts(ret);
  return CCP_EXEC_SUCCESS;
}

CCP_EXEC_STS Cmd_DI_CUBEWHEEL_SetControlMode(const CTCP* packet)
{
  const uint8_t* param = CCP_get_param_head(packet);
  CUBEWHEEL_IDX cubewheel_idx;

  cubewheel_idx = (CUBEWHEEL_IDX)param[0];
  if (cubewheel_idx > CUBEWHEEL_IDX_MAX)
  {
    return CCP_EXEC_ILLEGAL_PARAMETER;
  }

  uint8_t control_mode = param[1];

  CUBEWHEEL_SetControlMode(&cubewheel_driver_[cubewheel_idx], control_mode);

  // return DS_conv_cmd_err_to_ccp_exec_sts(ret);
  return CCP_EXEC_SUCCESS;
}

CCP_EXEC_STS Cmd_DI_CUBEWHEEL_SetPwmGain(const CTCP* packet)
{
  const uint8_t* param = CCP_get_param_head(packet);
  CUBEWHEEL_IDX cubewheel_idx;

  cubewheel_idx = (CUBEWHEEL_IDX)param[0];
  if (cubewheel_idx > CUBEWHEEL_IDX_MAX)
  {
    return CCP_EXEC_ILLEGAL_PARAMETER;
  }

  int16_t Ki = (int16_t)(param[1] << 8 | param[2]);
  uint8_t KiMultiplier = param[3];

  CUBEWHEEL_SetPwmGain(&cubewheel_driver_[cubewheel_idx], Ki, KiMultiplier);

  // return DS_conv_cmd_err_to_ccp_exec_sts(ret);
  return CCP_EXEC_SUCCESS;
}

CCP_EXEC_STS Cmd_DI_CUBEWHEEL_SetMainGain(const CTCP* packet)
{
  const uint8_t* param = CCP_get_param_head(packet);
  CUBEWHEEL_IDX cubewheel_idx;

  cubewheel_idx = (CUBEWHEEL_IDX)param[0];
  if (cubewheel_idx > CUBEWHEEL_IDX_MAX)
  {
    return CCP_EXEC_ILLEGAL_PARAMETER;
  }

  uint16_t Ki = (uint16_t)(param[1] << 8 | param[2]);
  uint8_t KiMultiplier = param[3];
  uint16_t Kd = (uint16_t)(param[4] << 8 | param[5]);
  uint8_t KdMultiplier = param[6];

  CUBEWHEEL_SetMainGain(&cubewheel_driver_[cubewheel_idx], Ki, KiMultiplier, Kd, KdMultiplier);



  // return DS_conv_cmd_err_to_ccp_exec_sts(ret);
  return CCP_EXEC_SUCCESS;
}

CCP_EXEC_STS Cmd_DI_CUBEWHEEL_GetStatus(const CTCP* packet)
{
  DS_CMD_ERR_CODE ret;

  // TODO: 処理を書く
  for (int idx = 0; idx < CUBEWHEEL_IDX_MAX; idx++)
  {
    ret = CUBEWHEEL_GetStatus(&cubewheel_driver_[idx]);
  }

  // return DS_conv_cmd_err_to_ccp_exec_sts(ret);
  return CCP_EXEC_SUCCESS;
}

CCP_EXEC_STS Cmd_DI_CUBEWHEEL_SetBackupmode(const CTCP* packet)
{
  const uint8_t* param = CCP_get_param_head(packet);
  CUBEWHEEL_IDX cubewheel_idx;
  uint8_t enable_status = 0;

  cubewheel_idx = (CUBEWHEEL_IDX)param[0];
  if (cubewheel_idx > CUBEWHEEL_IDX_MAX)
  {
    return CCP_EXEC_ILLEGAL_PARAMETER;
  }

  enable_status = param[1];

  CUBEWHEEL_SetBackupmode(&cubewheel_driver_[cubewheel_idx], enable_status);

  // return DS_conv_cmd_err_to_ccp_exec_sts(ret);
  return CCP_EXEC_SUCCESS;
}

CCP_EXEC_STS Cmd_DI_CUBEWHEEL_EnableEncoder(const CTCP* packet)
{
  const uint8_t* param = CCP_get_param_head(packet);
  CUBEWHEEL_IDX cubewheel_idx;
  uint8_t enable_status = 0;

  cubewheel_idx = (CUBEWHEEL_IDX)param[0];
  if (cubewheel_idx > CUBEWHEEL_IDX_MAX)
  {
    return CCP_EXEC_ILLEGAL_PARAMETER;
  }

  enable_status = param[1];

  CUBEWHEEL_EnableEncoder(&cubewheel_driver_[cubewheel_idx], enable_status);

  // return DS_conv_cmd_err_to_ccp_exec_sts(ret);
  return CCP_EXEC_SUCCESS;
}

CCP_EXEC_STS Cmd_DI_CUBEWHEEL_EnableHallsensor(const CTCP* packet)
{
  const uint8_t* param = CCP_get_param_head(packet);
  CUBEWHEEL_IDX cubewheel_idx;
  uint8_t enable_status = 0;

  cubewheel_idx = (CUBEWHEEL_IDX)param[0];
  if (cubewheel_idx > CUBEWHEEL_IDX_MAX)
  {
    return CCP_EXEC_ILLEGAL_PARAMETER;
  }

  enable_status = param[1];

  CUBEWHEEL_EnableHallsensor(&cubewheel_driver_[cubewheel_idx], enable_status);

  // return DS_conv_cmd_err_to_ccp_exec_sts(ret);
  return CCP_EXEC_SUCCESS;
}

CCP_EXEC_STS Cmd_DI_CUBEWHEEL_SET_ROTATION_DIRECTION_VECTOR(const CTCP* packet)
{
  const uint8_t* param = CCP_get_param_head(packet);
  CUBEWHEEL_IDX idx = (CUBEWHEEL_IDX)param[0];
  if (idx >= CUBEWHEEL_IDX_MAX) return CCP_EXEC_ILLEGAL_PARAMETER;

  float rotation_direction_b[PHYSICAL_CONST_THREE_DIM];
  for (int body_axis = 0; body_axis < PHYSICAL_CONST_THREE_DIM; body_axis++)
  {
    endian_memcpy(&rotation_direction_b[body_axis], param + 1 + body_axis * sizeof(float), sizeof(float));
  }

  C2A_MATH_ERROR ret_math = CUBEWHEEL_set_rotation_direction_b(&cubewheel_driver_[idx], rotation_direction_b);
  if (ret_math != C2A_MATH_ERROR_OK) return CCP_EXEC_ILLEGAL_PARAMETER;

  AOCS_MANAGER_ERROR ret_manager = DI_CUBEWHEEL_set_direction_matrix_to_aocs_manager_();
  if (ret_manager != AOCS_MANAGER_ERROR_OK) return CCP_EXEC_ILLEGAL_CONTEXT;

  return CCP_EXEC_SUCCESS;
}

int DI_CUBEWHEEL_set_torque_Nm(CUBEWHEEL_IDX idx, float torque_rw_Nm)
{
  if (idx >= CUBEWHEEL_IDX_MAX) return AOCS_ERROR_RANGE_OVER;
  if (DI_CUBEWHEEL_is_initialized_[idx] != 1) return AOCS_ERROR_OTHERS;

  float f_duty = 0.0;
  int duty = 0;
  CUBEWHEEL_IDX cubewheel_idx = idx;

  float max_torque_rw_Nm = (float)(cubewheel_driver_[cubewheel_idx].info.max_torque / 1000.0);
  f_duty = (float)(torque_rw_Nm / max_torque_rw_Nm);

  duty = (int)(f_duty * 100);

  // set max/min limit to duty value
  if (duty > 100)
  {
    duty = 100;
  }
  else if (duty < -100)
  {
    duty = -100;
  }

  CUBEWHEEL_SetDutyCycle(&cubewheel_driver_[cubewheel_idx], duty);
  return 0;
}

int DI_CUBEWHEEL_set_speed_rpm(CUBEWHEEL_IDX idx, float speed_rw_rpm)
{
  if (idx >= CUBEWHEEL_IDX_MAX) return AOCS_ERROR_RANGE_OVER;
  if (DI_CUBEWHEEL_is_initialized_[idx] != 1) return AOCS_ERROR_OTHERS;

  // speed制限
  float max_rpm = 8000;
  float min_rpm = -max_rpm;
  if (max_rpm < speed_rw_rpm)
  {
    speed_rw_rpm = max_rpm;
  }
  else if (speed_rw_rpm < min_rpm)
  {
    speed_rw_rpm = min_rpm;
  }

  int16_t speed_int = (int16_t)speed_rw_rpm;

  CUBEWHEEL_SetReferenceSpeed(&cubewheel_driver_[idx], speed_int);
  return 0;
}

#pragma section
