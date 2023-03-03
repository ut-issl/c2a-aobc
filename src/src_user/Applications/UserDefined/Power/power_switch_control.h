/**
* @file   power_switch_control.h
* @brief  電源操作アプリケーション
*/
#ifndef POWER_SWITCH_CONTROL_H_
#define POWER_SWITCH_CONTROL_H_

#include <src_core/System/ApplicationManager/app_info.h>
#include <src_core/TlmCmd/common_cmd_packet.h>

/**
 * @enum   APP_PSC_STATE
 * @brief  電源スイッチ状態を表す
 * @note   uint8_tを想定
 */
typedef enum
{
  APP_PSC_STATE_OFF = 0,
  APP_PSC_STATE_ON = 1,
} APP_PSC_STATE;

/**
 * @enum   APP_PSC_5V_IDX
 * @brief  5V系のスイッチポートのインデックス定義
 * @note   TLM/CMDはこのインデックスで管理する
 * @note   uint8_tを想定
 */
typedef enum
{
  APP_PSC_5V_IDX_INA260 = 0,
  APP_PSC_5V_IDX_MPU9250,
  APP_PSC_5V_IDX_RM3100,
  APP_PSC_5V_IDX_NANOSSOC_D60,
  APP_PSC_5V_IDX_MTQ_SEIREN,
  APP_PSC_5V_IDX_MAX,
} APP_PSC_5V_IDX;

/**
 * @enum   APP_PSC_UNREG_IDX
 * @brief  非安定バス系のスイッチポートのインデックス定義
 * @note   TLM/CMDはこのインデックスで管理する
 * @note   uint8_tを想定
 */
typedef enum
{
  APP_PSC_UNREG_IDX_STIM210 = 0,
  APP_PSC_UNREG_IDX_SAGITTA,
  APP_PSC_UNREG_IDX_OEM7600,
  APP_PSC_UNREG_IDX_RWX,
  APP_PSC_UNREG_IDX_RWY,
  APP_PSC_UNREG_IDX_RWZ,
  APP_PSC_UNREG_IDX_MAX,
} APP_PSC_UNREG_IDX;

/**
 * @union APP_PSC_SwitchState5v
 * @brief テレメ用5V電源状態共用体
 */
typedef union
{
  struct
  {
    uint8_t ina260       : 1;
    uint8_t mpu9250      : 1;
    uint8_t rm3100       : 1;
    uint8_t nanossoc_d60 : 1;
    uint8_t mtq_seiren   : 1;
    uint8_t b6           : 1;
    uint8_t b7           : 1;
    uint8_t b8           : 1;
  } bit;
  uint8_t byte;
} APP_PSC_SwitchState5v;

/**
 * @union APP_PSC_SwitchStateUnreg
 * @brief テレメ用非安定バス電源状態共用体
 */
typedef union
{
  struct
  {
    uint8_t stim210 : 1;
    uint8_t sagitta : 1;
    uint8_t oem7600 : 1;
    uint8_t rw_x    : 1;
    uint8_t rw_y    : 1;
    uint8_t rw_z    : 1;
    uint8_t b7      : 1;
    uint8_t b8      : 1;
  } bit;
  uint8_t byte;
} APP_PSC_SwitchStateUnreg;

/**
 * @struct PowerSwitchControl
 * @brief 電源操作に関する外部公開情報用構造体
 */
typedef struct
{
  APP_PSC_STATE switch_state_5v[APP_PSC_5V_IDX_MAX];       //!< 5Vスイッチ状態 内部利用用
  APP_PSC_STATE switch_state_unreg[APP_PSC_UNREG_IDX_MAX]; //!< 非安定バススイッチ状態 内部利用用
  APP_PSC_SwitchState5v switch_state_5v_tlm;               //!< 5Vスイッチ状態 TLM用
  APP_PSC_SwitchStateUnreg  switch_state_unreg_tlm;        //!< 非安定バススイッチ状態 TLM用
} PowerSwitchControl;

extern const PowerSwitchControl* const power_switch_control;

AppInfo APP_PSC_create_app(void);

CCP_EXEC_STS Cmd_APP_PSC_CONTROL_5V_PORT(const CommonCmdPacket* packet);
CCP_EXEC_STS Cmd_APP_PSC_CONTROL_UNREG_PORT(const CommonCmdPacket* packet);

#endif
