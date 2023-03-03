#pragma section REPRO
/**
* @file   di_rw0003.c
* @brief  RW0003のDriver Instance
*/

#include "di_rw0003.h"

#include <src_core/Library/print.h>
#include <src_core/Library/endian_memcpy.h>
#include <src_core/TlmCmd/common_cmd_packet_util.h>
#include <src_core/System/EventManager/event_logger.h>

#include "../UserDefined/Power/power_switch_control.h"
#include "../../Settings/port_config.h"
#include "../UserDefined/AOCS/aocs_manager.h"
#include "../../Library/vector3.h"

static RW0003_Driver rw0003_driver_[RW0003_IDX_MAX];
const  RW0003_Driver* const rw0003_driver[RW0003_IDX_MAX] = {&rw0003_driver_[RW0003_IDX_ON_X],
                                                             &rw0003_driver_[RW0003_IDX_ON_Y],
                                                             &rw0003_driver_[RW0003_IDX_ON_Z]};
static float DI_RW0003_rw_speed_rad_s_[RW0003_IDX_MAX] = {0.0f, 0.0f, 0.0f};
static uint8_t DI_RW0003_is_initialized_[RW0003_IDX_MAX];  //!< 0 = not initialized, 1 = initialized
static RW0003_IDX DI_RW0003_idx_counter_ = (RW0003_IDX)(0);  //!< DI_RW0003_update_が呼ばれたときに観測するRWを指定するカウンタ．

static void DI_RW0003_init_(void);
static void DI_RW0003_update_(void);

/**
 * @brief RW IDをインクリメントする
 */
static void DI_RW0003_increment_idx_counter_(void);

/**
 * @brief  AOCS MANAGERに方向行列をセットする
 * @return AOCS_MANAGER_ERRORに準拠
 */
static AOCS_MANAGER_ERROR DI_RW0003_set_direction_matrix_to_aocs_manager_(void);


AppInfo DI_RW0003_update(void)
{
  return AI_create_app_info("update_DI_RW0003", DI_RW0003_init_, DI_RW0003_update_);
}


static void DI_RW0003_init_(void)
{
  RW0003_init(&rw0003_driver_[RW0003_IDX_ON_X], PORT_CH_I2C_RWS, I2C_DEVICE_ADDR_RW_X);
  RW0003_init(&rw0003_driver_[RW0003_IDX_ON_Y], PORT_CH_I2C_RWS, I2C_DEVICE_ADDR_RW_Y);
  RW0003_init(&rw0003_driver_[RW0003_IDX_ON_Z], PORT_CH_I2C_RWS, I2C_DEVICE_ADDR_RW_Z);

  // 回転方向設定: Ref https://docs.google.com/presentation/d/10LCdETZEpM2TPSs5-GUYgjpkl0SGcNnRgydSOlRsS_I/edit#slide=id.ge080f54f61_0_111
  float direction_x[] = {-1.0f, 0.0f, 0.0f};
  RW0003_set_rotation_direction_b(&rw0003_driver_[RW0003_IDX_ON_X], direction_x);
  float direction_y[] = {0.0f, 1.0f, 0.0f};
  RW0003_set_rotation_direction_b(&rw0003_driver_[RW0003_IDX_ON_Y], direction_y);
  float direction_z[] = {0.0f, 0.0f, -1.0f};
  RW0003_set_rotation_direction_b(&rw0003_driver_[RW0003_IDX_ON_Z], direction_z);
  AOCS_MANAGER_ERROR ret_manager = DI_RW0003_set_direction_matrix_to_aocs_manager_();
  if (ret_manager != AOCS_MANAGER_ERROR_OK) Printf("RW direction setting Failed ! %d \n", ret_manager);

  for (int i = 0; i < RW0003_IDX_MAX; i++)
  {
    DI_RW0003_is_initialized_[i] = 0;
  }
}

// TODO_L RWは軸ごとにアプリを分けてもよい気がしているが、とりあえずは他のものと足並みをそろえるため一つのアプリで書く
static void DI_RW0003_update_(void)
{
  // TODO_L ここもう少しスマートに書けるようにしたい気もする
  if (power_switch_control->switch_state_unreg[APP_PSC_UNREG_IDX_RWX] == APP_PSC_STATE_OFF)
  {
    DI_RW0003_is_initialized_[RW0003_IDX_ON_X] = 0;
    DI_RW0003_rw_speed_rad_s_[RW0003_IDX_ON_X] = 0.0f;
  }
  if (power_switch_control->switch_state_unreg[APP_PSC_UNREG_IDX_RWY] == APP_PSC_STATE_OFF)
  {
    DI_RW0003_is_initialized_[RW0003_IDX_ON_Y] = 0;
    DI_RW0003_rw_speed_rad_s_[RW0003_IDX_ON_Y] = 0.0f;
  }
  if (power_switch_control->switch_state_unreg[APP_PSC_UNREG_IDX_RWZ] == APP_PSC_STATE_OFF)
  {
    DI_RW0003_is_initialized_[RW0003_IDX_ON_Z] = 0;
    DI_RW0003_rw_speed_rad_s_[RW0003_IDX_ON_Z] = 0.0f;
  }

  if (DI_RW0003_is_initialized_[DI_RW0003_idx_counter_])
  {
    DS_CMD_ERR_CODE ret;
    EL_GROUP el_group_tlm_error = (EL_GROUP)(EL_GROUP_TLM_ERROR_RW0003X + DI_RW0003_idx_counter_);
    EL_GROUP el_group_error = (EL_GROUP)(EL_GROUP_ERROR_RW0003X + DI_RW0003_idx_counter_);

    ret = RW0003_observe_speed(&rw0003_driver_[DI_RW0003_idx_counter_]);
    if (ret != DS_CMD_OK)
    {
      EL_record_event(el_group_tlm_error, (uint32_t)DI_RW0003_idx_counter_, EL_ERROR_LEVEL_HIGH, (uint32_t)ret);
    }
    else
    {
      DI_RW0003_rw_speed_rad_s_[DI_RW0003_idx_counter_] = rw0003_driver_[DI_RW0003_idx_counter_].info.speed_rad_s;
    }

    ret = RW0003_observe_temperature(&rw0003_driver_[DI_RW0003_idx_counter_]);
    if (ret != DS_CMD_OK)
    {
      EL_record_event(el_group_tlm_error, (uint32_t)DI_RW0003_idx_counter_, EL_ERROR_LEVEL_HIGH, (uint32_t)ret);
    }

    RW0003_REC_CRC_STATE state = rw0003_driver_[DI_RW0003_idx_counter_].info.crc_state;
    if (state != RW0003_REC_CRC_STATE_OK)
    {
      EL_record_event(el_group_error, (uint32_t)DI_RW0003_idx_counter_, EL_ERROR_LEVEL_HIGH, (uint32_t)RW0003_EL_NOTE_CRC_ERROR);
    }
  }

  AOCS_MANAGER_set_rw_angular_velocity_rad_s(DI_RW0003_rw_speed_rad_s_); // RW電源がOFFならそのRWの角速度は0とみなす
  DI_RW0003_increment_idx_counter_();
}

AOCS_ERROR DI_RW0003_set_torque_Nm(const RW0003_IDX idx, const float torque_Nm)
{
  if (idx >= RW0003_IDX_MAX) return AOCS_ERROR_RANGE_OVER;
  if (DI_RW0003_is_initialized_[idx] != 1) return AOCS_ERROR_OTHERS;

  // 入力制限はRW0003関数の方でされている
  DS_CMD_ERR_CODE ret = RW0003_drive_torque(&rw0003_driver_[idx], torque_Nm);
  if (ret != DS_CMD_OK) return AOCS_ERROR_OTHERS;

  return AOCS_ERROR_OK;
}

AOCS_ERROR DI_RW0003_set_speed_rad_s(const RW0003_IDX idx, const float speed_rad_s)
{
  if (idx >= RW0003_IDX_MAX) return AOCS_ERROR_RANGE_OVER;
  if (DI_RW0003_is_initialized_[idx] != 1) return AOCS_ERROR_OTHERS;

  // 入力制限はRW0003関数の方でされている
  DS_CMD_ERR_CODE ret = RW0003_drive_speed(&rw0003_driver_[idx], speed_rad_s);
  if (ret != DS_CMD_OK) return AOCS_ERROR_OTHERS;

  return AOCS_ERROR_OK;
}

static void DI_RW0003_increment_idx_counter_(void)
{
  DI_RW0003_idx_counter_ = (RW0003_IDX)(DI_RW0003_idx_counter_ + 1);

  if (DI_RW0003_idx_counter_ >= RW0003_IDX_MAX)
  {
    DI_RW0003_idx_counter_ = (RW0003_IDX)(0);
  }
}

static AOCS_MANAGER_ERROR DI_RW0003_set_direction_matrix_to_aocs_manager_(void)
{
  float rw_rotation_direction_matrix[AOCS_MANAGER_NUM_OF_RW][PHYSICAL_CONST_THREE_DIM];
  for (int idx = 0; idx < AOCS_MANAGER_NUM_OF_RW; idx++)
  {
    VECTOR3_copy(rw_rotation_direction_matrix[idx], rw0003_driver_[idx].info.rotation_direction_b);
  }
  AOCS_MANAGER_ERROR ret = AOCS_MANAGER_set_rw_rotation_direction_matrix(rw_rotation_direction_matrix);
  return ret;
}


// コマンド関数
CCP_EXEC_STS Cmd_DI_RW0003_INIT(const CommonCmdPacket* packet)
{
  const uint8_t* param = CCP_get_param_head(packet);
  RW0003_IDX idx;
  DS_CMD_ERR_CODE ret;

  idx = (RW0003_IDX)param[0];
  if (idx >= RW0003_IDX_MAX) return CCP_EXEC_ILLEGAL_PARAMETER;

  ret = RW0003_start_app(&rw0003_driver_[idx]);
  if (ret == DS_CMD_OK)
  {
    DI_RW0003_is_initialized_[idx] = 1;
  }

  return DS_conv_cmd_err_to_ccp_exec_sts(ret);
}

CCP_EXEC_STS Cmd_DI_RW0003_SET_ROTATION_DIRECTION_VECTOR(const CommonCmdPacket* packet)
{
  uint8_t arg_position = 0;

  RW0003_IDX idx = (RW0003_IDX)CCP_get_param_from_packet(packet, arg_position, uint8_t);
  arg_position++;
  if (idx >= RW0003_IDX_MAX) return CCP_EXEC_ILLEGAL_PARAMETER;

  float rotation_direction_b[PHYSICAL_CONST_THREE_DIM];
  for (int body_axis = 0; body_axis < PHYSICAL_CONST_THREE_DIM; body_axis++)
  {
    rotation_direction_b[body_axis] = (float)CCP_get_param_from_packet(packet, arg_position, float);
    arg_position++;
  }
  C2A_MATH_ERROR ret_math = RW0003_set_rotation_direction_b(&rw0003_driver_[idx], rotation_direction_b);
  if (ret_math != C2A_MATH_ERROR_OK) return CCP_EXEC_ILLEGAL_PARAMETER;

  AOCS_MANAGER_ERROR ret_manager = DI_RW0003_set_direction_matrix_to_aocs_manager_();
  if (ret_manager != AOCS_MANAGER_ERROR_OK) return CCP_EXEC_ILLEGAL_CONTEXT;

  return CCP_EXEC_SUCCESS;
}

CCP_EXEC_STS Cmd_DI_RW0003_SET_IDLE(const CommonCmdPacket* packet)
{
  const uint8_t* param = CCP_get_param_head(packet);
  RW0003_IDX idx;
  DS_CMD_ERR_CODE ret;

  idx = (RW0003_IDX)param[0];
  if (idx >= RW0003_IDX_MAX) return CCP_EXEC_ILLEGAL_PARAMETER;

  if (DI_RW0003_is_initialized_[idx] != 1) return CCP_EXEC_ILLEGAL_CONTEXT;

  ret = RW0003_set_idle(&rw0003_driver_[idx]);

  return DS_conv_cmd_err_to_ccp_exec_sts(ret);
}

CCP_EXEC_STS Cmd_DI_RW0003_DRIVE_TORQUE(const CommonCmdPacket* packet)
{
  const uint8_t* param = CCP_get_param_head(packet);
  RW0003_IDX idx;
  DS_CMD_ERR_CODE ret;

  idx = (RW0003_IDX)param[0];
  if (idx >= RW0003_IDX_MAX) return CCP_EXEC_ILLEGAL_PARAMETER;

  if (DI_RW0003_is_initialized_[idx] != 1) return CCP_EXEC_ILLEGAL_CONTEXT;

  float torque_Nm;
  endian_memcpy(&torque_Nm, param + 1, sizeof(float));
  if (torque_Nm > RW0003_kMaxTorqueNm) return CCP_EXEC_ILLEGAL_PARAMETER;
  if (torque_Nm < -1.0f * RW0003_kMaxTorqueNm) return CCP_EXEC_ILLEGAL_PARAMETER;

  ret = RW0003_drive_torque(&rw0003_driver_[idx], torque_Nm);

  return DS_conv_cmd_err_to_ccp_exec_sts(ret);
}

CCP_EXEC_STS Cmd_DI_RW0003_DRIVE_SPEED(const CommonCmdPacket* packet)
{
  const uint8_t* param = CCP_get_param_head(packet);
  RW0003_IDX idx;
  DS_CMD_ERR_CODE ret;

  idx = (RW0003_IDX)param[0];
  if (idx >= RW0003_IDX_MAX) return CCP_EXEC_ILLEGAL_PARAMETER;

  if (DI_RW0003_is_initialized_[idx] != 1) return CCP_EXEC_ILLEGAL_CONTEXT;

  float speed_rad_s;
  endian_memcpy(&speed_rad_s, param + 1, sizeof(float));
  if (speed_rad_s > rw0003_driver_[idx].info.speed_limit1_rad_s) return CCP_EXEC_ILLEGAL_PARAMETER;
  if (speed_rad_s < -1.0f * rw0003_driver_[idx].info.speed_limit1_rad_s) return CCP_EXEC_ILLEGAL_PARAMETER;

  ret = RW0003_drive_speed(&rw0003_driver_[idx], speed_rad_s);

  return DS_conv_cmd_err_to_ccp_exec_sts(ret);
}

#pragma section
