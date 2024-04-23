#pragma section REPRO
/**
* @file   di_oem7600.c
* @brief  OEM7600のDriver Instance
*/

#include "di_oem7600.h"
#include <src_core/Library/print.h>
#include <src_core/System/EventManager/event_logger.h>
#include <src_core/TlmCmd/common_cmd_packet_util.h>
#include <src_user/Settings/port_config.h>
#include <src_user/Settings/DriverSuper/driver_buffer_define.h>
#include <src_user/Applications/UserDefined/Power/power_switch_control.h>
#include <src_user/Applications/UserDefined/AOCS/aocs_manager.h>

#define DS_STREAM_REC_BUFFER_SIZE_OEM7600 (DS_IF_RX_BUFFER_SIZE_OEM7600 * 2)  //!< DS_StreamRecBuffer のバッファサイズ（非同期通信なので2倍している）

static void DI_OEM7600_init_(void);
static void DI_OEM7600_update_(void);

static OEM7600_Driver oem7600_driver_[OEM7600_IDX_MAX];
const  OEM7600_Driver* const oem7600_driver[OEM7600_IDX_MAX] = { &oem7600_driver_[OEM7600_IDX_IN_UNIT] };

// バッファ
// 面倒くさいので要素数一つと仮定してバッファを確保する
static DS_StreamRecBuffer DI_OEM7600_rx_buffer_;
static uint8_t DI_OEM7600_rx_buffer_allocation_[DS_STREAM_REC_BUFFER_SIZE_OEM7600];


AppInfo DI_OEM7600_update(void)
{
  return AI_create_app_info("update_DI_OEM7600", DI_OEM7600_init_, DI_OEM7600_update_);
}


static void DI_OEM7600_init_(void)
{
  DS_ERR_CODE ret1;
  DS_INIT_ERR_CODE ret2;

  ret1 = DS_init_stream_rec_buffer(&DI_OEM7600_rx_buffer_,
                                   DI_OEM7600_rx_buffer_allocation_,
                                   sizeof(DI_OEM7600_rx_buffer_allocation_));
  if (ret1 != DS_ERR_CODE_OK)
  {
    Printf("OEM7600 buffer init Failed ! %d \n", ret1);
  }

  // 面倒くさいので要素数一つと仮定して初期化する
  ret2 = OEM7600_init(&oem7600_driver_[OEM7600_IDX_IN_UNIT],
                      PORT_CH_UART_OEM,
                      PORT_CH_GPIO_IN_OEM_PPS,
                      PORT_CH_GPIO_OUT_OEM_RST,
                      &DI_OEM7600_rx_buffer_);
  if (ret2 != DS_INIT_OK)
  {
    Printf("OEM7600 init Failed ! %d \n", ret2);
  }
}

static void DI_OEM7600_update_(void)
{
  int oem_id = 0;

  // TODO_L: 複数のOEM7600と複数の電源IDを対応させる
  if (power_switch_control->switch_state_unreg[APP_PSC_UNREG_IDX_OEM7600] == APP_PSC_STATE_OFF)
  {
    // 電源OFF直前の可視衛星数最終値が4以上の状態で残りつづけ，他所で参照されることを避ける
    for (oem_id = 0; oem_id < OEM7600_IDX_MAX; oem_id++)
    {
      oem7600_driver_[oem_id].info.num_of_visible_sats = 0;
    }
    return;
  }

  for (oem_id = 0; oem_id < OEM7600_IDX_MAX; oem_id++)
  {
    DS_REC_ERR_CODE error_code_uart = OEM7600_rec(&oem7600_driver_[oem_id]);

    if (oem7600_driver_->driver.super.config.info.rec_status_.ret_from_if_rx == 0) return;

    if (error_code_uart != DS_REC_OK)
    {
      EL_record_event(EL_GROUP_TLM_ERROR_OEM7600, (uint32_t)oem_id, EL_ERROR_LEVEL_HIGH, (uint32_t)error_code_uart);
    }

    OEM7600_CRC_STATE state = oem7600_driver_->info.crc_state;
    if (state != OEM7600_CRC_STATE_OK)
    {
      EL_record_event(EL_GROUP_ERROR_OEM7600, (uint32_t)oem_id, EL_ERROR_LEVEL_HIGH, (uint32_t)OEM7600_EL_NOTE_CRC_ERROR);
    }

  }

  return;
}


CCP_CmdRet Cmd_DI_OEM7600_INIT(const CommonCmdPacket* packet)
{
  DS_CMD_ERR_CODE ret = DS_CMD_ILLEGAL_CONTEXT;

  // TODO_L: 現段階では初期化コマンド不要だが，今後必要に応じて中身を詰める

  return DS_conv_cmd_err_to_ccp_cmd_ret(ret);
}


CCP_CmdRet Cmd_DI_OEM7600_ONOFF_ANTENNA_POWER(const CommonCmdPacket* packet)
{
  DS_CMD_ERR_CODE ret;
  const uint8_t* param = CCP_get_param_head(packet);
  const uint8_t kPowerOn  = 1;
  const uint8_t kPowerOff = 0;
  OEM7600_IDX oem7600_id;
  uint8_t power_onoff;


  oem7600_id = (OEM7600_IDX)param[0];
  if (oem7600_id >= OEM7600_IDX_MAX) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);

  power_onoff = param[1];
  if ((power_onoff !=  kPowerOn) && (power_onoff != kPowerOff)) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);

  ret = OEM7600_onoff_antenna_power(&oem7600_driver_[oem7600_id], power_onoff);

  return DS_conv_cmd_err_to_ccp_cmd_ret(ret);
}


CCP_CmdRet Cmd_DI_OEM7600_SOFTWARE_RESET(const CommonCmdPacket* packet)
{
  DS_CMD_ERR_CODE ret;
  const uint8_t* param = CCP_get_param_head(packet);
  const uint8_t kTimeForResetMaxSec = 60;  //!< リセットまでの猶予期間 [sec] 指定値上限値 TODO_L:値域は適宜見直しても良いが，現状値で問題はない
  const uint8_t kTimeForResetMinSec = 1;   //!< リセットまでの猶予期間 [sec] 指定値下限値 TODO_L:値域は適宜見直しても良いが，現状値で問題はない
  OEM7600_IDX oem7600_id;
  uint8_t time_for_reset_sec;           //!< リセットまでの猶予期間指定値 [sec]


  oem7600_id = (OEM7600_IDX)param[0];
  if (oem7600_id >= OEM7600_IDX_MAX) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);

  time_for_reset_sec = param[1];
  if ((time_for_reset_sec < kTimeForResetMinSec) || (time_for_reset_sec > kTimeForResetMaxSec))
  {
    return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
  }

  ret = OEM7600_software_reset(&oem7600_driver_[oem7600_id], time_for_reset_sec);

  return DS_conv_cmd_err_to_ccp_cmd_ret(ret);
}


CCP_CmdRet Cmd_DI_OEM7600_SET_RESET_GPIO_HIGHLOW(const CommonCmdPacket* packet)
{
  DS_CMD_ERR_CODE ret;
  const uint8_t* param = CCP_get_param_head(packet);
  OEM7600_IDX oem7600_id;
  GPIO_HL high_low;


  oem7600_id = (OEM7600_IDX)param[0];
  if (oem7600_id >= OEM7600_IDX_MAX) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);

  high_low = (GPIO_HL)param[1];
  if ((high_low != GPIO_HIGH) && (high_low != GPIO_LOW)) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);

  ret = OEM7600_set_reset_gpio_high_low(&oem7600_driver_[oem7600_id], high_low);

  return DS_conv_cmd_err_to_ccp_cmd_ret(ret);
}


CCP_CmdRet Cmd_DI_OEM7600_SET_TLM_CONTENTS(const CommonCmdPacket* packet)
{
  DS_CMD_ERR_CODE ret;
  const uint8_t* param = CCP_get_param_head(packet);
  const uint8_t kTlmOutIntervalMaxSeconds = 60; //!< テレメ出力間隔設定値の最大値, TODO_L: 値域は適宜見直しても良いが，このままでも良い
  uint32_t oem7600_tlm_id;                      //!< 出力設定したいTLMのID (OEM7600_TLM_ID列挙子メンバ以外の数字はCMD送信関数側で除外する)
  uint8_t tlm_out_interval_seconds;             //!< 出力頻度 (0で停止)
  OEM7600_IDX oem7600_id;


  oem7600_id = (OEM7600_IDX)param[0];
  if (oem7600_id >= OEM7600_IDX_MAX) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);

  tlm_out_interval_seconds = param[1];
  if (tlm_out_interval_seconds > kTlmOutIntervalMaxSeconds) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);

  ENDIAN_memcpy(&oem7600_tlm_id, &(param[2]), sizeof(oem7600_tlm_id));

  ret = OEM7600_set_tlm_contents(&oem7600_driver_[oem7600_id], oem7600_tlm_id, tlm_out_interval_seconds);

  return DS_conv_cmd_err_to_ccp_cmd_ret(ret);
}


CCP_CmdRet Cmd_DI_OEM7600_SAVE_TLM_SETTING(const CommonCmdPacket* packet)
{
  DS_CMD_ERR_CODE ret;
  const uint8_t* param = CCP_get_param_head(packet);
  OEM7600_IDX oem7600_id;


  oem7600_id = (OEM7600_IDX)param[0];
  if (oem7600_id >= OEM7600_IDX_MAX) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);

  ret = OEM7600_save_tlm_setting(&oem7600_driver_[oem7600_id]);

  return DS_conv_cmd_err_to_ccp_cmd_ret(ret);
}

CCP_CmdRet Cmd_DI_OEM7600_SET_UART_BAUDRATE(const CommonCmdPacket* packet)
{
  const uint32_t kMaxBaudRate = 115200;
  const uint32_t kMinBaudRate = 2;

  DS_CMD_ERR_CODE ret;

  OEM7600_IDX oem7600_id = (OEM7600_IDX)CCP_get_param_from_packet(packet, 0, uint8_t);
  if (oem7600_id >= OEM7600_IDX_MAX) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);

  uint32_t baudrate_cmd = CCP_get_param_from_packet(packet, 1, uint32_t);
  if (baudrate_cmd > kMaxBaudRate) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
  if (baudrate_cmd < kMinBaudRate) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);

  OEM7600_UART_BAUDRATE_SET_DEVICE_ID device_id = (OEM7600_UART_BAUDRATE_SET_DEVICE_ID)CCP_get_param_from_packet(packet, 2, uint8_t);
  if (device_id >= OEM7600_UART_BAUDRATE_SET_DEVICE_ID_MAX) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);

  ret = OEM7600_set_uart_baudrate(&oem7600_driver_[oem7600_id], baudrate_cmd, device_id, &DI_OEM7600_rx_buffer_);

  return DS_conv_cmd_err_to_ccp_cmd_ret(ret);
}

#pragma section
