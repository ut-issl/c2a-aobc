#pragma section REPRO
/**
* @file   di_nanossoc_d60.c
* @brief  nanoSSOC-D60のDriver Instance
*/

#include "di_nanossoc_d60.h"

#include <src_core/Library/print.h>
#include <src_core/System/EventManager/event_logger.h>
#include "../../Settings/port_config.h"
#include "../UserDefined/Power/power_switch_control.h"
#include "../../IfWrapper/GPIO.h"

static NANOSSOC_D60_Driver nanossoc_d60_driver_[NANOSSOC_D60_IDX_MAX];
const  NANOSSOC_D60_Driver* const nanossoc_d60_driver[NANOSSOC_D60_IDX_MAX]
                                                     = {&nanossoc_d60_driver_[NANOSSOC_D60_IDX_ON_PY],
                                                        &nanossoc_d60_driver_[NANOSSOC_D60_IDX_ON_MY],
                                                        &nanossoc_d60_driver_[NANOSSOC_D60_IDX_ON_PZ],
                                                        &nanossoc_d60_driver_[NANOSSOC_D60_IDX_ON_MZ]};

static void DI_NANOSSOC_D60_init_(void);
static void DI_NANOSSOC_D60_update_(void);

static NANOSSOC_D60_IDX NANOSSOC_D60_idx_counter_ = NANOSSOC_D60_IDX_ON_PY; // DI_NANOSSOC_D60_update_が呼ばれたときにアップデートするサンセンサを指定するカウンタ．

/**
 * @brief  DI_NANOSSOC_D60_idx_counterアップデート関数
 *         DI_NANOSSOC_D60_update_で呼ばれて，カウンタをインクリメントし，次にアップデートするサンセンサの個体を指定する
 * @param  void
 * @return void
 */
static void DI_NANOSSOC_D60_update_idx_counter_(void);


AppInfo DI_NANOSSOC_D60_update(void)
{
  return AI_create_app_info("update_DI_NANOSSOC_D60", DI_NANOSSOC_D60_init_, DI_NANOSSOC_D60_update_);
}


static void DI_NANOSSOC_D60_init_(void)
{
  int ret;

  ret = NANOSSOC_D60_init(&nanossoc_d60_driver_[NANOSSOC_D60_IDX_ON_PY], PORT_CH_I2C_SENSORS, I2C_DEVICE_ADDR_SS_PY);
  if (ret != 0) Printf("NanoSSOC-D60 PY init Failed ! %d \n", ret);
  ret = NANOSSOC_D60_init(&nanossoc_d60_driver_[NANOSSOC_D60_IDX_ON_MY], PORT_CH_I2C_SENSORS, I2C_DEVICE_ADDR_SS_MY);
  if (ret != 0) Printf("NanoSSOC-D60 MY init Failed ! %d \n", ret);
  ret = NANOSSOC_D60_init(&nanossoc_d60_driver_[NANOSSOC_D60_IDX_ON_PZ], PORT_CH_I2C_SENSORS, I2C_DEVICE_ADDR_SS_PZ);
  if (ret != 0) Printf("NanoSSOC-D60 PZ init Failed ! %d \n", ret);
  ret = NANOSSOC_D60_init(&nanossoc_d60_driver_[NANOSSOC_D60_IDX_ON_MZ], PORT_CH_I2C_SENSORS, I2C_DEVICE_ADDR_SS_MZ);
  if (ret != 0) Printf("NanoSSOC-D60 MZ init Failed ! %d \n", ret);

  C2A_MATH_ERROR ret_math;
  Quaternion q_py_c2b;
  q_py_c2b = QUATERNION_make_x_rot(PHYSICAL_CONST_degree_to_radian(90.0f));
  ret_math = NANOSSOC_D60_set_frame_transform_c2b(&nanossoc_d60_driver_[NANOSSOC_D60_IDX_ON_PY], q_py_c2b);
  if (ret_math != C2A_MATH_ERROR_OK)
  {
    Printf("NanoSSOC-D60: q_py_c2b set error.\n");  // 初期化時のエラーはデバッグ表示して知らせるだけ
  }

  Quaternion q_my_c2b;
  QUATERNION_make_from_euler_angles(&q_my_c2b,
                                    PHYSICAL_CONST_degree_to_radian(90.0f),
                                    PHYSICAL_CONST_degree_to_radian(0.0f),
                                    PHYSICAL_CONST_degree_to_radian(180.0f),
                                    EULER_ANGLE_ROTATION_ORDER_123);
  ret_math = NANOSSOC_D60_set_frame_transform_c2b(&nanossoc_d60_driver_[NANOSSOC_D60_IDX_ON_MY], q_my_c2b);
  if (ret_math != C2A_MATH_ERROR_OK)
  {
    Printf("NanoSSOC-D60: q_py_c2b set error.\n");  // 初期化時のエラーはデバッグ表示して知らせるだけ
  }

  Quaternion q_pz_c2b;
  q_pz_c2b = QUATERNION_make_z_rot(PHYSICAL_CONST_degree_to_radian(90.0f));
  ret_math = NANOSSOC_D60_set_frame_transform_c2b(&nanossoc_d60_driver_[NANOSSOC_D60_IDX_ON_PZ], q_pz_c2b);
  if (ret_math != C2A_MATH_ERROR_OK)
  {
    Printf("NanoSSOC-D60: q_py_c2b set error.\n");  // 初期化時のエラーはデバッグ表示して知らせるだけ
  }

  Quaternion q_mz_c2b;
  QUATERNION_make_from_euler_angles(&q_mz_c2b,
                                    PHYSICAL_CONST_degree_to_radian(-90.0f),
                                    PHYSICAL_CONST_degree_to_radian(0.0f),
                                    PHYSICAL_CONST_degree_to_radian(180.0f),
                                    EULER_ANGLE_ROTATION_ORDER_321);
  ret_math = NANOSSOC_D60_set_frame_transform_c2b(&nanossoc_d60_driver_[NANOSSOC_D60_IDX_ON_MZ], q_mz_c2b);
  if (ret_math != C2A_MATH_ERROR_OK)
  {
    Printf("NanoSSOC-D60: q_py_c2b set error.\n");  // 初期化時のエラーはデバッグ表示して知らせるだけ
  }

}


static void DI_NANOSSOC_D60_update_(void)
{
  if (power_switch_control->switch_state_5v[APP_PSC_5V_IDX_NANOSSOC_D60] == APP_PSC_STATE_OFF)
  {
    // 電源OFF直前の可視状態で残りつづけ，他所で参照されることを避ける
    for (uint8_t sensor_id = 0; sensor_id < NANOSSOC_D60_IDX_MAX; sensor_id++)
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

static void DI_NANOSSOC_D60_update_idx_counter_(void)
{
  NANOSSOC_D60_idx_counter_ = (NANOSSOC_D60_IDX)(NANOSSOC_D60_idx_counter_ + 1);

  if (NANOSSOC_D60_idx_counter_ >= NANOSSOC_D60_IDX_MAX)
  {
    NANOSSOC_D60_idx_counter_ = NANOSSOC_D60_IDX_ON_PY;
  }
}

CCP_EXEC_STS Cmd_DI_NANOSSOC_D60_SET_FRAME_TRANSFORMATION_QUATERNION_C2B(const CommonCmdPacket* packet)
{
  const uint8_t* param = CCP_get_param_head(packet);

  NANOSSOC_D60_IDX idx;
  idx = (NANOSSOC_D60_IDX)param[0];
  if (idx >= NANOSSOC_D60_IDX_MAX) return CCP_EXEC_ILLEGAL_PARAMETER;

  float q_array_c2b[PHYSICAL_CONST_QUATERNION_DIM];
  for (int axis = 0; axis < PHYSICAL_CONST_QUATERNION_DIM; axis++)
  {
    endian_memcpy(&q_array_c2b[axis], param + 1 + axis * sizeof(float), sizeof(float));
  }

  Quaternion quaternion_c2b;
  C2A_MATH_ERROR ret;
  ret = QUATERNION_make_from_array(&quaternion_c2b, q_array_c2b, QUATERNION_SCALAR_POSITION_LAST);
  if (ret != C2A_MATH_ERROR_OK) return CCP_EXEC_ILLEGAL_PARAMETER;

  NANOSSOC_D60_set_frame_transform_c2b(&nanossoc_d60_driver_[idx], quaternion_c2b);

  return CCP_EXEC_SUCCESS;
}

#pragma section
