#pragma section REPRO
/**
* @file   power_switch_control.c
* @brief  電源操作アプリケーション
*/

#include "power_switch_control.h"

#include <src_core/TlmCmd/common_cmd_packet_util.h>
#include <src_core/Library/print.h>
#include <src_user/IfWrapper/GPIO.h>
#include <src_user/Settings/port_config.h>

/**
 * @enum   APP_PSC_OUTPUT_LOGIC
 * @brief  負論理か正論理かを表すenum
 * @note   uint8_tを想定
 */
typedef enum
{
  APP_PSC_OUTPUT_LOGIC_NEGATIVE = 0,
  APP_PSC_OUTPUT_LOGIC_POSITIVE = 1,
} APP_PSC_OUTPUT_LOGIC;

/**
 * @struct PowerSwitchControlPrivate
 * @brief 電源操作に関するPrivate構造体
 */
typedef struct
{
  APP_PSC_OUTPUT_LOGIC output_logic;
  uint8_t port_list;
} APP_PSC_PrivateInfo;


static APP_PSC_PrivateInfo APP_PSC_private_info_5v_[APP_PSC_5V_IDX_MAX];
static APP_PSC_PrivateInfo APP_PSC_private_info_unreg_[APP_PSC_UNREG_IDX_MAX];

static PowerSwitchControl        power_switch_control_;
const  PowerSwitchControl* const power_switch_control = &power_switch_control_;

/**
 * @brief 電源操作するprivate関数(5V系と非安定系それぞれ)
 * @param[in] port_id APP_PSC_5V_IDXもしくはAPP_PSC_UNREG_IDXで指定
 * @param[in] output 出力をHigh = ONかLow = OFFかで指定する
 * @return 0：正常、-1異常 TODO_L: enumにする？
 */
static int APP_PSC_control_5v_port_(APP_PSC_5V_IDX port_id, APP_PSC_STATE output);
static int APP_PSC_control_unreg_port_(APP_PSC_UNREG_IDX port_id, APP_PSC_STATE output);

/**
 * @brief TLM用の圧縮版を操作する関数(5V系と非安定系それぞれ)
 * @param[in] port_id APP_PSC_5V_IDXもしくはAPP_PSC_UNREG_IDXで指定
 * @param[in] output 出力をHigh = ONかLow = OFFかで指定する
 * @return 無し
 */
void APP_PSC_switch_state_tlm_5v_(APP_PSC_5V_IDX port_id, APP_PSC_STATE output);
void APP_PSC_switch_state_tlm_unreg_(APP_PSC_UNREG_IDX port_id, APP_PSC_STATE output);

/**
 * @brief 電源ON/OFF状態をGPIO状態に変換する関数
 * @param[in] power_state 電源ON/OFF状態
 * @param[in] info        ポート情報
 * @return                GPIOがHIGHかLOWか
 */
static GPIO_HL APP_PSC_convert_power_state_to_gpio_state_(APP_PSC_STATE power_state, APP_PSC_PrivateInfo info);

static void APP_PSC_init_(void);
static void APP_PSC_exec_(void);

AppInfo APP_PSC_create_app(void)
{
  return AI_create_app_info("PowerSwitchControl", APP_PSC_init_, APP_PSC_exec_);
}

static void APP_PSC_init_(void)
{
  power_switch_control_.switch_state_5v_tlm.byte    = 0x00;
  power_switch_control_.switch_state_unreg_tlm.byte = 0x00;

  // Set port id
  // stable 5V power port list
  APP_PSC_private_info_5v_[APP_PSC_5V_IDX_INA260].port_list       = PORT_CH_GPIO_OUT_POWER_INA;
  APP_PSC_private_info_5v_[APP_PSC_5V_IDX_MPU9250].port_list      = PORT_CH_GPIO_OUT_POWER_MPU;
  APP_PSC_private_info_5v_[APP_PSC_5V_IDX_RM3100].port_list       = PORT_CH_GPIO_OUT_POWER_RM;
  APP_PSC_private_info_5v_[APP_PSC_5V_IDX_NANOSSOC_D60].port_list = PORT_CH_GPIO_OUT_POWER_SS;
  APP_PSC_private_info_5v_[APP_PSC_5V_IDX_MTQ_SEIREN].port_list   = PORT_CH_GPIO_OUT_POWER_MTQ;

  // unregulated power port list
  APP_PSC_private_info_unreg_[APP_PSC_UNREG_IDX_STIM210].port_list = PORT_CH_GPIO_OUT_POWER_STIM;
  APP_PSC_private_info_unreg_[APP_PSC_UNREG_IDX_SAGITTA].port_list = PORT_CH_GPIO_OUT_POWER_SAGITTA;
  APP_PSC_private_info_unreg_[APP_PSC_UNREG_IDX_OEM7600].port_list = PORT_CH_GPIO_OUT_POWER_OEM;
  APP_PSC_private_info_unreg_[APP_PSC_UNREG_IDX_RWX].port_list     = PORT_CH_GPIO_OUT_POWER_RWX;
  APP_PSC_private_info_unreg_[APP_PSC_UNREG_IDX_RWY].port_list     = PORT_CH_GPIO_OUT_POWER_RWY;
  APP_PSC_private_info_unreg_[APP_PSC_UNREG_IDX_RWZ].port_list     = PORT_CH_GPIO_OUT_POWER_RWZ;

  // Set output logic
  // stable 5V output logic
  for (uint8_t i = 0; i < APP_PSC_5V_IDX_MAX; i++)
  {
    APP_PSC_private_info_5v_[i].output_logic = APP_PSC_OUTPUT_LOGIC_POSITIVE;
  }
  APP_PSC_private_info_5v_[APP_PSC_5V_IDX_INA260].output_logic = APP_PSC_OUTPUT_LOGIC_NEGATIVE;

  // unregulated output logic
  for (uint8_t i = 0; i < APP_PSC_UNREG_IDX_MAX; i++)
  {
    APP_PSC_private_info_unreg_[i].output_logic = APP_PSC_OUTPUT_LOGIC_POSITIVE;
  }

  // Set direction and initial output
  for (uint8_t i = 0; i < APP_PSC_5V_IDX_MAX; i++)
  {
    GPIO_set_direction(APP_PSC_private_info_5v_[i].port_list, GPIO_OUTPUT);
    APP_PSC_control_5v_port_((APP_PSC_5V_IDX)i, APP_PSC_STATE_OFF);
  }
  for (uint8_t i = 0; i < APP_PSC_UNREG_IDX_MAX; i++)
  {
    GPIO_set_direction(APP_PSC_private_info_unreg_[i].port_list, GPIO_OUTPUT);
    APP_PSC_control_unreg_port_((APP_PSC_UNREG_IDX)i, APP_PSC_STATE_OFF);
  }

  return;
}

static void APP_PSC_exec_(void)
{
  return;
}

static GPIO_HL APP_PSC_convert_power_state_to_gpio_state_(APP_PSC_STATE power_state, APP_PSC_PrivateInfo info)
{
  if (info.output_logic == APP_PSC_OUTPUT_LOGIC_POSITIVE)
  {
    if (power_state == APP_PSC_STATE_OFF)
    {
      return GPIO_LOW;
    }
    else
    {
      return GPIO_HIGH;
    }
  }
  else
  {
    if (power_state == APP_PSC_STATE_OFF)
    {
      return GPIO_HIGH;
    }
    else
    {
      return GPIO_LOW;
    }
  }
}

static int APP_PSC_control_5v_port_(APP_PSC_5V_IDX port_id, APP_PSC_STATE output)
{
  if (port_id >= APP_PSC_5V_IDX_MAX) return -1;

  GPIO_HL gpio_output = APP_PSC_convert_power_state_to_gpio_state_(output, APP_PSC_private_info_5v_[port_id]);

  if (GPIO_set_output(APP_PSC_private_info_5v_[port_id].port_list, gpio_output) != GPIO_OK) return -1;

  power_switch_control_.switch_state_5v[port_id] = output;
  APP_PSC_switch_state_tlm_5v_(port_id, output);

  return 0;
}

static int APP_PSC_control_unreg_port_(APP_PSC_UNREG_IDX port_id, APP_PSC_STATE output)
{
  if (port_id >= APP_PSC_UNREG_IDX_MAX) return -1;

  GPIO_HL gpio_output = APP_PSC_convert_power_state_to_gpio_state_(output, APP_PSC_private_info_unreg_[port_id]);

  if (GPIO_set_output(APP_PSC_private_info_unreg_[port_id].port_list, gpio_output) != GPIO_OK) return -1;

  power_switch_control_.switch_state_unreg[port_id] = output;
  APP_PSC_switch_state_tlm_unreg_(port_id, output);

  return 0;
}

void APP_PSC_switch_state_tlm_5v_(APP_PSC_5V_IDX port_id, APP_PSC_STATE output)
{
  uint8_t out_u8 = (output == APP_PSC_STATE_ON) ? 1 : 0;
  switch (port_id)
  {
  case APP_PSC_5V_IDX_INA260:
    power_switch_control_.switch_state_5v_tlm.bit.ina260 = out_u8;
    break;
  case APP_PSC_5V_IDX_MPU9250:
    power_switch_control_.switch_state_5v_tlm.bit.mpu9250 = out_u8;
    break;
  case APP_PSC_5V_IDX_RM3100:
    power_switch_control_.switch_state_5v_tlm.bit.rm3100 = out_u8;
    break;
  case APP_PSC_5V_IDX_NANOSSOC_D60:
    power_switch_control_.switch_state_5v_tlm.bit.nanossoc_d60 = out_u8;
    break;
  case APP_PSC_5V_IDX_MTQ_SEIREN:
    power_switch_control_.switch_state_5v_tlm.bit.mtq_seiren = out_u8;
    break;
  default:
    break;
  }
  return;
}

void APP_PSC_switch_state_tlm_unreg_(APP_PSC_UNREG_IDX port_id, APP_PSC_STATE output)
{
  uint8_t out_u8 = (output == APP_PSC_STATE_ON) ? 1 : 0;
  switch (port_id)
  {
  case APP_PSC_UNREG_IDX_STIM210:
    power_switch_control_.switch_state_unreg_tlm.bit.stim210 = out_u8;
    break;
  case APP_PSC_UNREG_IDX_SAGITTA:
    power_switch_control_.switch_state_unreg_tlm.bit.sagitta = out_u8;
    break;
  case APP_PSC_UNREG_IDX_OEM7600:
    power_switch_control_.switch_state_unreg_tlm.bit.oem7600 = out_u8;
    break;
  case APP_PSC_UNREG_IDX_RWX:
    power_switch_control_.switch_state_unreg_tlm.bit.rw_x = out_u8;
    break;
  case APP_PSC_UNREG_IDX_RWY:
    power_switch_control_.switch_state_unreg_tlm.bit.rw_y = out_u8;
    break;
  case APP_PSC_UNREG_IDX_RWZ:
    power_switch_control_.switch_state_unreg_tlm.bit.rw_z = out_u8;
    break;
  default:
    break;
  }
  return;
}


CCP_CmdRet Cmd_APP_PSC_CONTROL_5V_PORT(const CommonCmdPacket* packet)
{
  const uint8_t* param = CCP_get_param_head(packet);
  APP_PSC_5V_IDX port_id;
  APP_PSC_STATE switch_flag;

  port_id = (APP_PSC_5V_IDX)param[0];
  if (port_id >= APP_PSC_5V_IDX_MAX) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);

  switch_flag = (APP_PSC_STATE)param[1];
  if (APP_PSC_control_5v_port_(port_id, switch_flag) != 0) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);  // GPIO_setのエラーもパラメータエラーに帰着する

  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

CCP_CmdRet Cmd_APP_PSC_CONTROL_UNREG_PORT(const CommonCmdPacket* packet)
{
  const uint8_t* param = CCP_get_param_head(packet);
  APP_PSC_UNREG_IDX port_id;
  APP_PSC_STATE switch_flag;

  port_id = (APP_PSC_UNREG_IDX)param[0];
  if (port_id >= APP_PSC_UNREG_IDX_MAX) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);  // GPIO_setのエラーもパラメータエラーに帰着する

  switch_flag = (APP_PSC_STATE)param[1];
  if (APP_PSC_control_unreg_port_(port_id, switch_flag) != 0) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);

  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

#pragma section
