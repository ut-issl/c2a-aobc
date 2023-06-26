#pragma section REPRO
/**
* @file   di_rm3100.c
* @brief  RM3100のDriver Instance
*/

#include "di_rm3100.h"

#include <src_core/Library/print.h>
#include <src_core/Library/endian.h>
#include <src_core/System/EventManager/event_logger.h>
#include <src_core/TlmCmd/common_cmd_packet_util.h>
#include "../../Settings/port_config.h"
#include "../../Settings/DriverSuper/driver_buffer_define.h"
#include "../UserDefined/Power/power_switch_control.h"
#include "../../Library/vector3.h"

// Satellite Parameters
#include "../../Settings/SatelliteParameters/rm3100_parameters.h"

static void DI_RM3100_init_(void);
static void DI_RM3100_update_(void);

static RM3100_Driver rm3100_driver_[RM3100_IDX_MAX];
const  RM3100_Driver* const rm3100_driver[RM3100_IDX_MAX] = {&rm3100_driver_[RM3100_IDX_ON_AOBC],
                                                             &rm3100_driver_[RM3100_IDX_EXTERNAL]};
// バッファ
static DS_StreamRecBuffer DI_RM3100_rx_buffer_[RM3100_IDX_MAX];
static uint8_t DI_RM3100_rx_buffer_allocation_[RM3100_IDX_MAX][DS_STREAM_REC_BUFFER_SIZE_DEFAULT];

static uint8_t DI_RM3100_is_initialized_[RM3100_IDX_MAX] = { 0, 0 };  //!< 0 = not initialized, 1 = initialized

static float DI_RM3100_default_bias_aobc_compo_nT_[PHYSICAL_CONST_THREE_DIM];
static float DI_RM3100_default_bias_ext_compo_nT_[PHYSICAL_CONST_THREE_DIM];
static float DI_RM3100_kBiasDiffMax_nT_ = 20000.0f; //!< 磁気バイアスAddコマンドでトータルバイアスがデフォルト値から離れすぎないようにするためのしきい値


AppInfo DI_RM3100_update(void)
{
  return AI_create_app_info("update_DI_RM3100", DI_RM3100_init_, DI_RM3100_update_);
}


static void DI_RM3100_init_(void)
{
  uint8_t i;
  DS_ERR_CODE ret1;
  C2A_MATH_ERROR ret;

  // バッファの初期化
  for (i = 0; i < RM3100_IDX_MAX; ++i)
  {
    ret1 = DS_init_stream_rec_buffer(&DI_RM3100_rx_buffer_[i],
                                     DI_RM3100_rx_buffer_allocation_[i],
                                     sizeof(DI_RM3100_rx_buffer_allocation_[i]));
    if (ret1 != DS_ERR_CODE_OK)
    {
      Printf("RM3100 buffer #%d init Failed ! %d \n", i, ret1);
    }
  }

  // AOBC
  RM3100_init(&rm3100_driver_[RM3100_IDX_ON_AOBC],
              PORT_CH_I2C_SENSORS,
              I2C_DEVICE_ADDR_AOBC_RM,
              &DI_RM3100_rx_buffer_[RM3100_IDX_ON_AOBC]);
  Quaternion q_aobc_c2b;
  QUATERNION_make_from_euler_angles(&q_aobc_c2b,
                                    PHYSICAL_CONST_degree_to_radian(0.0f),
                                    PHYSICAL_CONST_degree_to_radian(90.0f),
                                    PHYSICAL_CONST_degree_to_radian(180.0f),
                                    EULER_ANGLE_ROTATION_ORDER_321);
  ret = RM3100_set_frame_transform_c2b(&rm3100_driver_[RM3100_IDX_ON_AOBC], q_aobc_c2b);
  if (ret != C2A_MATH_ERROR_OK)
  {
    Printf("RM3100: q_aobc_c2b set error.\n");  // 初期化時のエラーはデバッグ表示して知らせるだけ
  }

  VECTOR3_copy(DI_RM3100_default_bias_aobc_compo_nT_, RM3100_PARAMETERS_mag_aobc_bias_compo_nT);
  ret = RM3100_set_mag_bias_compo_nT(&rm3100_driver_[RM3100_IDX_ON_AOBC], DI_RM3100_default_bias_aobc_compo_nT_);
  if (ret != C2A_MATH_ERROR_OK)
  {
    Printf("RM3100: mag_aobc_bias set error.\n");
  }

  // External
  RM3100_init(&rm3100_driver_[RM3100_IDX_EXTERNAL],
              PORT_CH_I2C_SENSORS,
              I2C_DEVICE_ADDR_EXT_RM,
              &DI_RM3100_rx_buffer_[RM3100_IDX_EXTERNAL]);
  ret = RM3100_set_frame_transform_c2b(&rm3100_driver_[RM3100_IDX_EXTERNAL], RM3100_PARAMETERS_mag_ext_quaternion_c2b);
  if (ret != C2A_MATH_ERROR_OK)
  {
    Printf("RM3100: q_ext_c2b set error.\n");  // 初期化時のエラーはデバッグ表示して知らせるだけ
  }

  VECTOR3_copy(DI_RM3100_default_bias_ext_compo_nT_, RM3100_PARAMETERS_mag_ext_bias_compo_nT);
  ret = RM3100_set_mag_bias_compo_nT(&rm3100_driver_[RM3100_IDX_EXTERNAL], DI_RM3100_default_bias_ext_compo_nT_);
  if (ret != C2A_MATH_ERROR_OK)
  {
    Printf("RM3100: mag_ext_bias set error.\n");
  }

  // 共通
  for (int i = 0; i < RM3100_IDX_MAX; i++)
  {
    DI_RM3100_is_initialized_[i] = 0;
  }
}


static void DI_RM3100_update_(void)
{
  if (power_switch_control->switch_state_5v[APP_PSC_5V_IDX_RM3100] == APP_PSC_STATE_OFF)
  {
    for (int i = 0; i < RM3100_IDX_MAX; i++)
    {
      DI_RM3100_is_initialized_[i] = 0;
    }
    return;
  }

  for (int idx = 0; idx < RM3100_IDX_MAX; idx++)
  {
    if (DI_RM3100_is_initialized_[idx])
    {
      DS_CMD_ERR_CODE ret = RM3100_observe_mag(&rm3100_driver_[idx]);
      if (ret != DS_CMD_OK)
      {
        EL_record_event(EL_GROUP_TLM_ERROR_RM3100, (uint32_t)idx, EL_ERROR_LEVEL_HIGH, (uint32_t)ret);
      }
    }
  }
}


CCP_CmdRet Cmd_DI_RM3100_INIT(const CommonCmdPacket* packet)
{
  const uint8_t* param = CCP_get_param_head(packet);
  const uint8_t mode = RM3100_kCmmMode;
  RM3100_IDX idx;
  DS_CMD_ERR_CODE ret;

  idx = (RM3100_IDX)param[0];
  if (idx >= RM3100_IDX_MAX) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);

  ret = RM3100_set_mode(&rm3100_driver_[idx], mode);
  if (ret == DS_CMD_OK)
  {
    DI_RM3100_is_initialized_[idx] = 1;
  }

  return DS_conv_cmd_err_to_ccp_cmd_ret(ret);
}


CCP_CmdRet Cmd_DI_RM3100_SET_FRAME_TRANSFORMATION_QUATERNION_C2B(const CommonCmdPacket* packet)
{
  const uint8_t* param = CCP_get_param_head(packet);

  RM3100_IDX idx;
  idx = (RM3100_IDX)param[0];
  if (idx >= RM3100_IDX_MAX) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);

  float q_array_c2b[PHYSICAL_CONST_QUATERNION_DIM];
  for (int axis = 0; axis < PHYSICAL_CONST_QUATERNION_DIM; axis++)
  {
    ENDIAN_memcpy(&q_array_c2b[axis], param + 1 + axis * sizeof(float), sizeof(float));
  }

  Quaternion quaternion_c2b;
  C2A_MATH_ERROR ret;
  ret = QUATERNION_make_from_array(&quaternion_c2b, q_array_c2b, QUATERNION_SCALAR_POSITION_LAST);
  if (ret != C2A_MATH_ERROR_OK) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);

  RM3100_set_frame_transform_c2b(&rm3100_driver_[idx], quaternion_c2b);

  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

CCP_CmdRet Cmd_DI_RM3100_SET_MAG_BIAS_COMPO_NT(const CommonCmdPacket* packet)
{
  uint8_t arg_num = 0;
  RM3100_IDX idx = (RM3100_IDX)CCP_get_param_from_packet(packet, arg_num, uint8_t);
  arg_num++;
  if (idx >= RM3100_IDX_MAX) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);

  float mag_bias_compo_nT[PHYSICAL_CONST_THREE_DIM];
  for (int axis = 0; axis < PHYSICAL_CONST_THREE_DIM; axis++)
  {
    mag_bias_compo_nT[axis] = CCP_get_param_from_packet(packet, arg_num, float);
    arg_num++;
  }

  uint8_t add_flag = CCP_get_param_from_packet(packet, arg_num, uint8_t);
  arg_num++;
  if (add_flag >= 3) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);

  C2A_MATH_ERROR ret;
  if (add_flag == 0)
  {
    ret = RM3100_set_mag_bias_compo_nT(&rm3100_driver_[idx], mag_bias_compo_nT);
    // デフォルト値も更新
    if (ret == C2A_MATH_ERROR_OK)
    {
      if (idx == RM3100_IDX_ON_AOBC)
      {
        VECTOR3_copy(DI_RM3100_default_bias_aobc_compo_nT_, mag_bias_compo_nT);
      }
      else
      {
        VECTOR3_copy(DI_RM3100_default_bias_ext_compo_nT_, mag_bias_compo_nT);
      }
    }
  }
  else if (add_flag == 1)
  {
    ret = RM3100_add_mag_bias_compo_nT(&rm3100_driver_[idx], mag_bias_compo_nT);
    // デフォルト値から誤差が大きすぎたら弾く
    // TODO_L: 影響範囲を狭くするため汚いコードになっているので修正する
    float mag_bias_diff_compo_nT[PHYSICAL_CONST_THREE_DIM];
    float mag_bias_default_compo_nT[PHYSICAL_CONST_THREE_DIM];
    if (idx == RM3100_IDX_ON_AOBC)
    {
      VECTOR3_copy(mag_bias_default_compo_nT, DI_RM3100_default_bias_aobc_compo_nT_);
    }
    else
    {
      VECTOR3_copy(mag_bias_default_compo_nT, DI_RM3100_default_bias_ext_compo_nT_);
    }
    VECTOR3_subtract(mag_bias_diff_compo_nT, mag_bias_default_compo_nT, rm3100_driver_[idx].info.mag_bias_compo_nT);
    float mag_bias_diff_norm_nT = VECTOR3_norm(mag_bias_diff_compo_nT);
    if (mag_bias_diff_norm_nT > DI_RM3100_kBiasDiffMax_nT_)
    {
      // デフォルトから離れ過ぎたらデフォルト値に戻す
      RM3100_set_mag_bias_compo_nT(&rm3100_driver_[idx], mag_bias_default_compo_nT);
      return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_CONTEXT);
    }
  }
  else
  {
    // デフォルト値にクリア
    if (idx == RM3100_IDX_ON_AOBC)
    {
      ret = RM3100_set_mag_bias_compo_nT(&rm3100_driver_[idx], DI_RM3100_default_bias_aobc_compo_nT_);
    }
    else
    {
      ret = RM3100_set_mag_bias_compo_nT(&rm3100_driver_[idx], DI_RM3100_default_bias_ext_compo_nT_);
    }
  }

  if (ret != C2A_MATH_ERROR_OK) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);

  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

#pragma section
