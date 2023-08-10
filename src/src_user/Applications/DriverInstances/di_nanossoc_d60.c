#pragma section REPRO
/**
* @file   di_nanossoc_d60.c
* @brief  nanoSSOC-D60のDriver Instance
*/

#include "di_nanossoc_d60.h"

#include <src_core/Library/print.h>
#include <src_core/System/EventManager/event_logger.h>
#include <src_core/TlmCmd/common_cmd_packet_util.h>
#include <src_user/Settings/port_config.h>
#include <src_user/Settings/DriverSuper/driver_buffer_define.h>
#include <src_user/Applications/UserDefined/Power/power_switch_control.h>
#include <src_user/IfWrapper/GPIO.h>

// Satellite Parameters
#include <src_user/Settings/SatelliteParameters/nanossoc_d60_parameters.h>

static void DI_NANOSSOC_D60_init_(void);
static void DI_NANOSSOC_D60_update_(void);
static uint8_t DI_NANOSSOC_D60_conv_idx_to_i2c_address_(uint8_t idx);
/**
 * @brief  DI_NANOSSOC_D60_idx_counterアップデート関数
 *         DI_NANOSSOC_D60_update_で呼ばれて，カウンタをインクリメントし，次にアップデートするサンセンサの個体を指定する
 * @param  void
 * @return void
 */
static void DI_NANOSSOC_D60_update_idx_counter_(void);

static NANOSSOC_D60_Driver nanossoc_d60_driver_[NANOSSOC_D60_IDX_MAX];
const  NANOSSOC_D60_Driver* const nanossoc_d60_driver[NANOSSOC_D60_IDX_MAX]
                                                     = {&nanossoc_d60_driver_[NANOSSOC_D60_IDX_ON_PY],
                                                        &nanossoc_d60_driver_[NANOSSOC_D60_IDX_ON_MY],
                                                        &nanossoc_d60_driver_[NANOSSOC_D60_IDX_ON_PZ],
                                                        &nanossoc_d60_driver_[NANOSSOC_D60_IDX_ON_MZ],
                                                        &nanossoc_d60_driver_[NANOSSOC_D60_IDX_ON_PX],
                                                        &nanossoc_d60_driver_[NANOSSOC_D60_IDX_ON_MX]};
// バッファ
static DS_StreamRecBuffer DI_NANOSSOC_D60_rx_buffer_[NANOSSOC_D60_PARAMETERS_NUMBER_OF_MOUNTED_SENSOR];
static uint8_t DI_NANOSSOC_D60_rx_buffer_allocation_[NANOSSOC_D60_PARAMETERS_NUMBER_OF_MOUNTED_SENSOR][DS_STREAM_REC_BUFFER_SIZE_SYNCHRONOUS_SMALL];

static NANOSSOC_D60_IDX NANOSSOC_D60_idx_counter_ = NANOSSOC_D60_IDX_ON_PY; // DI_NANOSSOC_D60_update_が呼ばれたときにアップデートするサンセンサを指定するカウンタ．


AppInfo DI_NANOSSOC_D60_update(void)
{
  return AI_create_app_info("update_DI_NANOSSOC_D60", DI_NANOSSOC_D60_init_, DI_NANOSSOC_D60_update_);
}

static void DI_NANOSSOC_D60_init_(void)
{
  uint8_t i;
  DS_ERR_CODE ret1;
  DS_INIT_ERR_CODE ret2;
  uint8_t addr;

  for (i = 0; i < NANOSSOC_D60_PARAMETERS_NUMBER_OF_MOUNTED_SENSOR; ++i)
  {
    ret1 = DS_init_stream_rec_buffer(&DI_NANOSSOC_D60_rx_buffer_[i],
                                     DI_NANOSSOC_D60_rx_buffer_allocation_[i],
                                     sizeof(DI_NANOSSOC_D60_rx_buffer_allocation_[i]));
    if (ret1 != DS_ERR_CODE_OK)
    {
      Printf("NANOSSOC_D60 buffer #%d init Failed ! %d \n", i, ret1);
    }

    addr = DI_NANOSSOC_D60_conv_idx_to_i2c_address_(i);
    ret2 = NANOSSOC_D60_init(&nanossoc_d60_driver_[i], PORT_CH_I2C_SENSORS, addr, &DI_NANOSSOC_D60_rx_buffer_[i]);
    if (ret2 != DS_INIT_OK)
    {
      Printf("NANOSSOC_D60 #%d init Failed ! %d \n", i, ret2);
    }
  }

  C2A_MATH_ERROR ret_math;
  ret_math = NANOSSOC_D60_set_frame_transform_c2b(&nanossoc_d60_driver_[NANOSSOC_D60_IDX_ON_PY], NANOSSOC_D60_PARAMETERS_py_quaternion_c2b);
  if (ret_math != C2A_MATH_ERROR_OK)
  {
    Printf("NanoSSOC-D60: q_py_c2b set error.\n");  // 初期化時のエラーはデバッグ表示して知らせるだけ
  }

  ret_math = NANOSSOC_D60_set_frame_transform_c2b(&nanossoc_d60_driver_[NANOSSOC_D60_IDX_ON_MY], NANOSSOC_D60_PARAMETERS_my_quaternion_c2b);
  if (ret_math != C2A_MATH_ERROR_OK)
  {
    Printf("NanoSSOC-D60: q_py_c2b set error.\n");  // 初期化時のエラーはデバッグ表示して知らせるだけ
  }

  ret_math = NANOSSOC_D60_set_frame_transform_c2b(&nanossoc_d60_driver_[NANOSSOC_D60_IDX_ON_PZ], NANOSSOC_D60_PARAMETERS_pz_quaternion_c2b);
  if (ret_math != C2A_MATH_ERROR_OK)
  {
    Printf("NanoSSOC-D60: q_py_c2b set error.\n");  // 初期化時のエラーはデバッグ表示して知らせるだけ
  }

  ret_math = NANOSSOC_D60_set_frame_transform_c2b(&nanossoc_d60_driver_[NANOSSOC_D60_IDX_ON_MZ], NANOSSOC_D60_PARAMETERS_mz_quaternion_c2b);
  if (ret_math != C2A_MATH_ERROR_OK)
  {
    Printf("NanoSSOC-D60: q_py_c2b set error.\n");  // 初期化時のエラーはデバッグ表示して知らせるだけ
  }

  ret_math = NANOSSOC_D60_set_frame_transform_c2b(&nanossoc_d60_driver_[NANOSSOC_D60_IDX_ON_PX], NANOSSOC_D60_PARAMETERS_px_quaternion_c2b);
  if (ret_math != C2A_MATH_ERROR_OK)
  {
    Printf("NanoSSOC-D60: q_px_c2b set error.\n");  // 初期化時のエラーはデバッグ表示して知らせるだけ
  }

  ret_math = NANOSSOC_D60_set_frame_transform_c2b(&nanossoc_d60_driver_[NANOSSOC_D60_IDX_ON_MX], NANOSSOC_D60_PARAMETERS_mx_quaternion_c2b);
  if (ret_math != C2A_MATH_ERROR_OK)
  {
    Printf("NanoSSOC-D60: q_zx_c2b set error.\n");  // 初期化時のエラーはデバッグ表示して知らせるだけ
  }
}

static void DI_NANOSSOC_D60_update_(void)
{
  if (power_switch_control->switch_state_5v[APP_PSC_5V_IDX_NANOSSOC_D60] == APP_PSC_STATE_OFF)
  {
    // 電源OFF直前の可視状態で残りつづけ，他所で参照されることを避ける
    for (uint8_t sensor_id = 0; sensor_id < NANOSSOC_D60_PARAMETERS_NUMBER_OF_MOUNTED_SENSOR; sensor_id++)
    {
      nanossoc_d60_driver_[sensor_id].info.sun_intensity_percent = 0.0f;
    }
    return;
  }

  DS_CMD_ERR_CODE ret = NANOSSOC_D60_observe(&nanossoc_d60_driver_[NANOSSOC_D60_idx_counter_]);
  if (ret != DS_CMD_OK)
  {
    EL_record_event(EL_GROUP_TLM_ERROR_NANOSSOC, (uint32_t)NANOSSOC_D60_idx_counter_, EL_ERROR_LEVEL_HIGH, (uint32_t)ret);
  }

  NANOSSOC_D60_CHECKSUM_STATE state = nanossoc_d60_driver_->info.checksum_state;
  if (state != NANOSSOC_D60_CHECKSUM_STATE_OK)
  {
    EL_record_event(EL_GROUP_ERROR_NANOSSOC, (uint32_t)NANOSSOC_D60_idx_counter_, EL_ERROR_LEVEL_HIGH, (uint32_t)NANOSSOC_D60_EL_NOTE_CHECKSUM_ERROR);
  }

  DI_NANOSSOC_D60_update_idx_counter_();
}

static uint8_t DI_NANOSSOC_D60_conv_idx_to_i2c_address_(uint8_t idx)
{
  switch (idx)
  {
  case NANOSSOC_D60_IDX_ON_PY:
    return I2C_DEVICE_ADDR_SS_PY;
  case NANOSSOC_D60_IDX_ON_MY:
    return I2C_DEVICE_ADDR_SS_MY;
  case NANOSSOC_D60_IDX_ON_PZ:
    return I2C_DEVICE_ADDR_SS_PZ;
  case NANOSSOC_D60_IDX_ON_MZ:
    return I2C_DEVICE_ADDR_SS_MZ;
  case NANOSSOC_D60_IDX_ON_PX:
    return I2C_DEVICE_ADDR_SS_PX;
  case NANOSSOC_D60_IDX_ON_MX:
    return I2C_DEVICE_ADDR_SS_MX;
  default:
    // ここには来ないはず
    return 0;
  }
}

static void DI_NANOSSOC_D60_update_idx_counter_(void)
{
  NANOSSOC_D60_idx_counter_ = (NANOSSOC_D60_IDX)(NANOSSOC_D60_idx_counter_ + 1);

  if (NANOSSOC_D60_idx_counter_ >= NANOSSOC_D60_PARAMETERS_NUMBER_OF_MOUNTED_SENSOR)
  {
    NANOSSOC_D60_idx_counter_ = NANOSSOC_D60_IDX_ON_PY;
  }
}

CCP_CmdRet Cmd_DI_NANOSSOC_D60_SET_FRAME_TRANSFORMATION_QUATERNION_C2B(const CommonCmdPacket* packet)
{
  const uint8_t* param = CCP_get_param_head(packet);

  NANOSSOC_D60_IDX idx;
  idx = (NANOSSOC_D60_IDX)param[0];
  if (idx >= NANOSSOC_D60_PARAMETERS_NUMBER_OF_MOUNTED_SENSOR) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);

  float q_array_c2b[PHYSICAL_CONST_QUATERNION_DIM];
  for (int axis = 0; axis < PHYSICAL_CONST_QUATERNION_DIM; axis++)
  {
    ENDIAN_memcpy(&q_array_c2b[axis], param + 1 + axis * sizeof(float), sizeof(float));
  }

  Quaternion quaternion_c2b;
  C2A_MATH_ERROR ret;
  ret = QUATERNION_make_from_array(&quaternion_c2b, q_array_c2b, QUATERNION_SCALAR_POSITION_LAST);
  if (ret != C2A_MATH_ERROR_OK) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);

  NANOSSOC_D60_set_frame_transform_c2b(&nanossoc_d60_driver_[idx], quaternion_c2b);

  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

#pragma section
