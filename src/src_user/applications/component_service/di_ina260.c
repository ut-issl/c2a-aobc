#pragma section REPRO
/**
* @file   di_ina260.c
* @brief  INA260のDriver Instance
*/

#include "di_ina260.h"

#include <src_core/Library/print.h>
#include <src_core/Library/endian.h>
#include <src_core/TlmCmd/common_cmd_packet_util.h>
#include <src_core/System/EventManager/event_logger.h>
#include <src_user/Settings/port_config.h>
#include <src_user/Settings/DriverSuper/driver_buffer_define.h>
#include <src_user/Applications/UserDefined/Power/power_switch_control.h>


static void DI_INA260_init_(void);
static void DI_INA260_update_(void);
static uint8_t DI_INA260_conv_idx_to_i2c_address_(uint8_t idx);
static void DI_INA260_increment_idx_counter_(void);

static INA260_Driver ina260_driver_[INA260_IDX_MAX];
const  INA260_Driver* const ina260_driver[INA260_IDX_MAX] = {&ina260_driver_[INA260_IDX_PIC],
                                                             &ina260_driver_[INA260_IDX_STIM210],
                                                             &ina260_driver_[INA260_IDX_SAGITTA],
                                                             &ina260_driver_[INA260_IDX_OEM7600],
                                                             &ina260_driver_[INA260_IDX_RM3100],
                                                             &ina260_driver_[INA260_IDX_NANOSSOC_D60],
                                                             &ina260_driver_[INA260_IDX_MTQ],
                                                             &ina260_driver_[INA260_IDX_RW0003_X],
                                                             &ina260_driver_[INA260_IDX_RW0003_Y],
                                                             &ina260_driver_[INA260_IDX_RW0003_Z]};
// バッファ
static DS_StreamRecBuffer DI_INA260_rx_buffer_[INA260_IDX_MAX];
static uint8_t DI_INA260_rx_buffer_allocation_[INA260_IDX_MAX][DS_STREAM_REC_BUFFER_SIZE_SYNCHRONOUS_SMALL];

static uint8_t    DI_INA260_is_initialized_[INA260_IDX_MAX]; //!< 0 = not initialized, 1 = initialized
static INA260_IDX DI_INA260_idx_counter_ = (INA260_IDX)(0);  //!< DI_INA260_update_が呼ばれたときに観測する電流センサを指定するカウンタ．
const  INA260_IDX* const DI_INA260_idx_counter = &DI_INA260_idx_counter_;


AppInfo DI_INA260_update(void)
{
  return AI_create_app_info("update_DI_INA260", DI_INA260_init_, DI_INA260_update_);
}

static void DI_INA260_init_(void)
{
  uint8_t i;
  DS_ERR_CODE ret1;
  DS_INIT_ERR_CODE ret2;
  uint8_t addr;

  for (i = 0; i < INA260_IDX_MAX; ++i)
  {
    ret1 = DS_init_stream_rec_buffer(&DI_INA260_rx_buffer_[i],
                                     DI_INA260_rx_buffer_allocation_[i],
                                     sizeof(DI_INA260_rx_buffer_allocation_[i]));
    if (ret1 != DS_ERR_CODE_OK)
    {
      Printf("INA260 buffer #%d init Failed ! %d \n", i, ret1);
    }

    addr = DI_INA260_conv_idx_to_i2c_address_(i);
    ret2 = INA260_init(&ina260_driver_[i], PORT_CH_I2C_INAS, addr, &DI_INA260_rx_buffer_[i]);
    if (ret2 != DS_INIT_OK)
    {
      Printf("INA260 #%d init Failed ! %d \n", i, ret2);
    }

    DI_INA260_is_initialized_[i] = 0;
  }
}

static void DI_INA260_update_(void)
{
  if (power_switch_control->switch_state_5v[APP_PSC_5V_IDX_INA260] == APP_PSC_STATE_OFF)
  {
    for (int i = 0; i < INA260_IDX_MAX; i++)
    {
      DI_INA260_is_initialized_[i] = 0;
    }
    return;
  }

  if (DI_INA260_is_initialized_[DI_INA260_idx_counter_])
  {
    DS_CMD_ERR_CODE ret;
    ret = INA260_observe_current(&ina260_driver_[DI_INA260_idx_counter_]);
    if (ret != DS_CMD_OK)
    {
      EL_record_event(EL_GROUP_TLM_ERROR_INA260, (uint32_t)DI_INA260_idx_counter_, EL_ERROR_LEVEL_HIGH, (uint32_t)ret);
    }
    ret = INA260_observe_voltage(&ina260_driver_[DI_INA260_idx_counter_]);
    if (ret != DS_CMD_OK)
    {
      EL_record_event(EL_GROUP_TLM_ERROR_INA260, (uint32_t)DI_INA260_idx_counter_, EL_ERROR_LEVEL_HIGH, (uint32_t)ret);
    }
  }

  DI_INA260_increment_idx_counter_();
}

static uint8_t DI_INA260_conv_idx_to_i2c_address_(uint8_t idx)
{
  switch (idx)
  {
  case INA260_IDX_PIC:
    return I2C_DEVICE_ADDR_INA_PIC;
  case INA260_IDX_STIM210:
    return I2C_DEVICE_ADDR_INA_STIM;
  case INA260_IDX_SAGITTA:
    return I2C_DEVICE_ADDR_INA_SAGITTA;
  case INA260_IDX_OEM7600:
    return I2C_DEVICE_ADDR_INA_OEM;
  case INA260_IDX_RM3100:
    return I2C_DEVICE_ADDR_INA_RM;
  case INA260_IDX_NANOSSOC_D60:
    return I2C_DEVICE_ADDR_INA_SS;
  case INA260_IDX_MTQ:
    return I2C_DEVICE_ADDR_INA_MTQ;
  case INA260_IDX_RW0003_X:
    return I2C_DEVICE_ADDR_INA_RWX;
  case INA260_IDX_RW0003_Y:
    return I2C_DEVICE_ADDR_INA_RWY;
  case INA260_IDX_RW0003_Z:
    return I2C_DEVICE_ADDR_INA_RWZ;
  default:
    // ここには来ないはず
    return 0;
  }
}

static void DI_INA260_increment_idx_counter_(void)
{
  DI_INA260_idx_counter_ = (INA260_IDX)(DI_INA260_idx_counter_ + 1);

  if (DI_INA260_idx_counter_ >= INA260_IDX_MAX)
  {
    DI_INA260_idx_counter_ = (INA260_IDX)(0);
  }
}


CCP_CmdRet Cmd_DI_INA260_INIT(const CommonCmdPacket* packet)
{
  const uint8_t* param = CCP_get_param_head(packet);
  INA260_IDX idx;
  DS_CMD_ERR_CODE ret;

  idx = (INA260_IDX)param[0];
  if (idx >= INA260_IDX_MAX) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);

  INA260_AVERAGING_MODE  averaging_mode = (INA260_AVERAGING_MODE)param[1];
  INA260_CONVERSION_TIME voltage_conversion_time = (INA260_CONVERSION_TIME)param[2];
  INA260_CONVERSION_TIME current_conversion_time = (INA260_CONVERSION_TIME)param[3];

  ret = INA260_set_mode(&ina260_driver_[idx], averaging_mode, voltage_conversion_time, current_conversion_time);
  if (ret == DS_CMD_OK)
  {
    DI_INA260_is_initialized_[idx] = 1;
  }

  return DS_conv_cmd_err_to_ccp_cmd_ret(ret);
}

CCP_CmdRet Cmd_DI_INA260_SET_OVER_CURRENT_PROTECTION(const CommonCmdPacket* packet)
{
  const uint8_t* param = CCP_get_param_head(packet);
  float over_current_threshold_mA;
  INA260_IDX idx;
  DS_CMD_ERR_CODE ret;

  idx = (INA260_IDX)param[0];
  if (idx >= INA260_IDX_MAX) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);

  ENDIAN_memcpy(&over_current_threshold_mA, param + 1, sizeof(float));
  // アサーションはset関数内で行っている

  ret = INA260_set_over_current_threshold(&ina260_driver_[idx], over_current_threshold_mA);
  if (ret != DS_CMD_OK) return DS_conv_cmd_err_to_ccp_cmd_ret(ret);

  ret = INA260_enable_over_current_protection(&ina260_driver_[idx]);

  // OCにかかっていた時にラッチを外すため一度読む
  ret = INA260_read_mask_register(&ina260_driver_[idx]);

  return DS_conv_cmd_err_to_ccp_cmd_ret(ret);
}

#pragma section
