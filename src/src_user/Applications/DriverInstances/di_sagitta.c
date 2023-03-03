#pragma section REPRO
/**
* @file   di_sagitta.c
* @brief  SAGITTAのDriver Instance
*/

#include "di_sagitta.h"

#include <src_core/Library/print.h>
#include <src_core/Library/endian_memcpy.h>
#include <src_core/System/EventManager/event_logger.h>
#include <src_core/TlmCmd/common_cmd_packet_util.h>

#include "../../Settings/port_config.h"
#include "../UserDefined/Power/power_switch_control.h"

static SAGITTA_Driver sagitta_driver_[SAGITTA_IDX_MAX];
const  SAGITTA_Driver* const sagitta_driver[SAGITTA_IDX_MAX] = {&sagitta_driver_[SAGITTA_IDX_IN_UNIT]};

static void DI_SAGITTA_init_(void);
static void DI_SAGITTA_update_(void);

static uint8_t DI_SAGITTA_is_booted_[SAGITTA_IDX_MAX];  //!< 0 = not booted, 1 = booted

AppInfo DI_SAGITTA_update(void)
{
  return AI_create_app_info("update_DI_SAGITTA", DI_SAGITTA_init_, DI_SAGITTA_update_);
}


static void DI_SAGITTA_init_(void)
{

  int ret = 0;
  ret = SAGITTA_init(&(sagitta_driver_[SAGITTA_IDX_IN_UNIT]), PORT_CH_UART_SAGITTA);

  if (ret != 0)
  {
    Printf("SAGITTA init Failed ! %d", ret);
  }

  for (int i = 0; i < SAGITTA_IDX_MAX; i++)
  {
    DI_SAGITTA_is_booted_[i] = 0;
  }

  C2A_MATH_ERROR ret_math;
  Quaternion q_c2b;
  QUATERNION_make_from_euler_angles(&q_c2b,
                                    PHYSICAL_CONST_degree_to_radian(75.0f),
                                    PHYSICAL_CONST_degree_to_radian(90.0f),
                                    PHYSICAL_CONST_degree_to_radian(0.0f),
                                    EULER_ANGLE_ROTATION_ORDER_321);
  ret_math = SAGITTA_set_frame_transform_c2b(&sagitta_driver_[SAGITTA_IDX_IN_UNIT], q_c2b);
  if (ret_math != C2A_MATH_ERROR_OK)
  {
    Printf("SAGITTA: q_c2b set error.\n");  // 初期化時のエラーはデバッグ表示して知らせるだけ
  }

  return;
}


static void DI_SAGITTA_update_(void)
{
  if (power_switch_control->switch_state_unreg[APP_PSC_UNREG_IDX_SAGITTA] == APP_PSC_STATE_OFF)
  {
    DI_SAGITTA_is_booted_[SAGITTA_IDX_IN_UNIT] = 0;

    // 電源OFF直前の可視状態で残りつづけ，他所で参照されることを避ける
    for (uint8_t stt_id = 0; stt_id < SAGITTA_IDX_MAX; stt_id++)
    {
      sagitta_driver_[stt_id].info.is_valid_quaternion = 0;
    }
  }
  else
  {
    DS_REC_ERR_CODE ret = SAGITTA_rec(&sagitta_driver_[SAGITTA_IDX_IN_UNIT]);

    if (sagitta_driver_->driver.super.config.rec_status_.ret_from_if_rx == 0) return;

    if (ret != DS_REC_OK)
    {
      EL_record_event(EL_GROUP_TLM_ERROR_SAGITTA, (uint32_t)SAGITTA_IDX_IN_UNIT, EL_ERROR_LEVEL_HIGH, (uint32_t)ret);
    }

    SAGITTA_XXHASH_STATE state = sagitta_driver_->info.xxhash_state;
    if (state != SAGITTA_XXHASH_STATE_OK)
    {
      EL_record_event(EL_GROUP_ERROR_SAGITTA, (uint32_t)SAGITTA_IDX_IN_UNIT, EL_ERROR_LEVEL_HIGH, (uint32_t)SAGITTA_EL_NOTE_XXHASH_ERROR);
    }
  }
  return;
}


CCP_EXEC_STS Cmd_DI_SAGITTA_BOOT(const CommonCmdPacket* packet)
{
  DS_CMD_ERR_CODE ret;
  const uint8_t* param = CCP_get_param_head(packet);

  if (DI_SAGITTA_is_booted_[SAGITTA_IDX_IN_UNIT])  return CCP_EXEC_ILLEGAL_CONTEXT;

  ret = SAGITTA_boot(&(sagitta_driver_[SAGITTA_IDX_IN_UNIT]));

  if (ret != DS_CMD_OK) return DS_conv_cmd_err_to_ccp_exec_sts(ret);

  DI_SAGITTA_is_booted_[SAGITTA_IDX_IN_UNIT] = 1;

  return DS_conv_cmd_err_to_ccp_exec_sts(ret);
}

CCP_EXEC_STS Cmd_DI_SAGITTA_SET_UNIX_TIME_US(const CommonCmdPacket* packet)
{
  DS_CMD_ERR_CODE ret;
  const uint8_t* param = CCP_get_param_head(packet);
  uint32_t unix_time_us_upper = 0;
  uint32_t unix_time_us_lower = 0;

  endian_memcpy(&unix_time_us_upper, &param[0], (uint8_t)sizeof(uint32_t));
  endian_memcpy(&unix_time_us_lower, &param[4], (uint8_t)sizeof(uint32_t));
  ret = SAGITTA_set_unix_time_us(&(sagitta_driver_[SAGITTA_IDX_IN_UNIT]), unix_time_us_upper, unix_time_us_lower);

  return DS_conv_cmd_err_to_ccp_exec_sts(ret);
}

CCP_EXEC_STS Cmd_DI_SAGITTA_SET_PARAMETER(const CommonCmdPacket* packet)
{
  if (!DI_SAGITTA_is_booted_[SAGITTA_IDX_IN_UNIT])  return CCP_EXEC_ILLEGAL_CONTEXT;

  uint8_t param_id = CCP_get_param_from_packet(packet, 0, uint8_t);

  DS_CMD_ERR_CODE ret;
  switch (param_id)
  {
  case SAGITTA_PARAMETER_ID_LOG_LEVEL:
    ret = SAGITTA_set_log_level(&(sagitta_driver_[SAGITTA_IDX_IN_UNIT]));
    break;
  case SAGITTA_PARAMETER_ID_LIMITS:
    ret = SAGITTA_set_limits(&(sagitta_driver_[SAGITTA_IDX_IN_UNIT]));
    break;
  case SAGITTA_PARAMETER_ID_MOUNTING:
    ret = SAGITTA_set_mounting(&(sagitta_driver_[SAGITTA_IDX_IN_UNIT]));
    break;
  case SAGITTA_PARAMETER_ID_CAMERA:
    ret = SAGITTA_set_camera(&(sagitta_driver_[SAGITTA_IDX_IN_UNIT]));
    break;
  case SAGITTA_PARAMETER_ID_IMAGE_PROCESSOR:
    ret = SAGITTA_set_image_processor(&(sagitta_driver_[SAGITTA_IDX_IN_UNIT]));
    break;
  case SAGITTA_PARAMETER_ID_CENTROIDING:
    ret = SAGITTA_set_centroiding(&(sagitta_driver_[SAGITTA_IDX_IN_UNIT]));
    break;
  case SAGITTA_PARAMETER_ID_LISA:
    ret = SAGITTA_set_lisa(&(sagitta_driver_[SAGITTA_IDX_IN_UNIT]));
    break;
  case SAGITTA_PARAMETER_ID_MATCHING:
    ret = SAGITTA_set_matching(&(sagitta_driver_[SAGITTA_IDX_IN_UNIT]));
    break;
  case SAGITTA_PARAMETER_ID_TRACKING:
    ret = SAGITTA_set_tracking(&(sagitta_driver_[SAGITTA_IDX_IN_UNIT]));
    break;
  case SAGITTA_PARAMETER_ID_VALIDATION:
    ret = SAGITTA_set_validation(&(sagitta_driver_[SAGITTA_IDX_IN_UNIT]));
    break;
  case SAGITTA_PARAMETER_ID_ALGO:
    ret = SAGITTA_set_algo(&(sagitta_driver_[SAGITTA_IDX_IN_UNIT]));
    break;
  case SAGITTA_PARAMETER_ID_SUBSCRIPTION:
    ret = SAGITTA_set_subscription(&(sagitta_driver_[SAGITTA_IDX_IN_UNIT]));
    break;
  default:
    return CCP_EXEC_ILLEGAL_CONTEXT;
  }

  return DS_conv_cmd_err_to_ccp_exec_sts(ret);
}

CCP_EXEC_STS Cmd_DI_SAGITTA_CHANGE_PARAMETER(const CommonCmdPacket* packet)
{
  uint8_t param_id = CCP_get_param_from_packet(packet, 0, uint8_t); // SAGITTA_PARAMETER_ID
  uint8_t param_idx = CCP_get_param_from_packet(packet, 1, uint8_t); // 各SAGITTA_PARAMETER_ID内でのparameter index
  float value = CCP_get_param_from_packet(packet, 2, float);

  DS_CMD_ERR_CODE ret;
  switch (param_id)
  {
  case SAGITTA_PARAMETER_ID_LOG_LEVEL:
    ret = SAGITTA_change_log_level(&(sagitta_driver_[SAGITTA_IDX_IN_UNIT]), param_idx, value);
    break;
  case SAGITTA_PARAMETER_ID_LIMITS:
    ret = SAGITTA_change_limits(&(sagitta_driver_[SAGITTA_IDX_IN_UNIT]), param_idx, value);
    break;
  case SAGITTA_PARAMETER_ID_MOUNTING:
    ret = SAGITTA_change_mounting(&(sagitta_driver_[SAGITTA_IDX_IN_UNIT]), param_idx, value);
    break;
  case SAGITTA_PARAMETER_ID_CAMERA:
    ret = SAGITTA_change_camera(&(sagitta_driver_[SAGITTA_IDX_IN_UNIT]), param_idx, value);
    break;
  case SAGITTA_PARAMETER_ID_IMAGE_PROCESSOR:
    ret = SAGITTA_change_image_processor(&(sagitta_driver_[SAGITTA_IDX_IN_UNIT]), param_idx, value);
    break;
  case SAGITTA_PARAMETER_ID_CENTROIDING:
    ret = SAGITTA_change_centroiding(&(sagitta_driver_[SAGITTA_IDX_IN_UNIT]), param_idx, value);
    break;
  case SAGITTA_PARAMETER_ID_LISA:
    ret = SAGITTA_change_lisa(&(sagitta_driver_[SAGITTA_IDX_IN_UNIT]), param_idx, value);
    break;
  case SAGITTA_PARAMETER_ID_MATCHING:
    ret = SAGITTA_change_matching(&(sagitta_driver_[SAGITTA_IDX_IN_UNIT]), param_idx, value);
    break;
  case SAGITTA_PARAMETER_ID_TRACKING:
    ret = SAGITTA_change_tracking(&(sagitta_driver_[SAGITTA_IDX_IN_UNIT]), param_idx, value);
    break;
  case SAGITTA_PARAMETER_ID_VALIDATION:
    ret = SAGITTA_change_validation(&(sagitta_driver_[SAGITTA_IDX_IN_UNIT]), param_idx, value);
    break;
  case SAGITTA_PARAMETER_ID_ALGO:
    ret = SAGITTA_change_algo(&(sagitta_driver_[SAGITTA_IDX_IN_UNIT]), param_idx, value);
    break;
  case SAGITTA_PARAMETER_ID_SUBSCRIPTION:
    ret = SAGITTA_change_subscription(&(sagitta_driver_[SAGITTA_IDX_IN_UNIT]), param_idx, value);
    break;
  default:
    return CCP_EXEC_ILLEGAL_CONTEXT;
  }

  return DS_conv_cmd_err_to_ccp_exec_sts(ret);
}

CCP_EXEC_STS Cmd_DI_SAGITTA_READ_PARAMETER(const CommonCmdPacket* packet)
{
  if (!DI_SAGITTA_is_booted_[SAGITTA_IDX_IN_UNIT])  return CCP_EXEC_ILLEGAL_CONTEXT;

  DS_CMD_ERR_CODE ret;
  uint8_t param_id = CCP_get_param_from_packet(packet, 0, uint8_t);

  ret = SAGITTA_read_parameter(&(sagitta_driver_[SAGITTA_IDX_IN_UNIT]), param_id);

  return DS_conv_cmd_err_to_ccp_exec_sts(ret);
}

CCP_EXEC_STS Cmd_DI_SAGITTA_SET_FRAME_TRANSFORMATION_QUATERNION_C2B(const CommonCmdPacket* packet)
{
  const uint8_t* param = CCP_get_param_head(packet);

  SAGITTA_IDX idx;
  idx = (SAGITTA_IDX)param[0];
  if (idx >= SAGITTA_IDX_MAX) return CCP_EXEC_ILLEGAL_PARAMETER;

  float q_array_c2b[PHYSICAL_CONST_QUATERNION_DIM];
  for (int i = 0; i < PHYSICAL_CONST_QUATERNION_DIM; i++)
  {
    endian_memcpy(&q_array_c2b[i], param + 1 + i * sizeof(float), sizeof(float));
  }

  Quaternion quaternion_c2b;
  C2A_MATH_ERROR ret;
  ret = QUATERNION_make_from_array(&quaternion_c2b, q_array_c2b, QUATERNION_SCALAR_POSITION_LAST);
  if (ret != C2A_MATH_ERROR_OK) return CCP_EXEC_ILLEGAL_PARAMETER;

  SAGITTA_set_frame_transform_c2b(&sagitta_driver_[idx], quaternion_c2b);

  return CCP_EXEC_SUCCESS;
}

#pragma section
