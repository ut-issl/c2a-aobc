/**
* @file   di_oem7600.h
* @brief  OEM7600のDriver Instance
*/
#ifndef DI_OEM7600_H_
#define DI_OEM7600_H_

#include <src_user/Drivers/Aocs/oem7600.h>
#include <src_core/System/ApplicationManager/app_info.h>

/**
 * @enum  OEM7600_IDX
 * @brief 搭載されているOEM7600の管理番号 (今は1つのみ)
 * @note  uint8_tを想定
 * @note  要素数が変わった場合、cファイル側の初期化部分の修正が必要になる点に注意する
 */
typedef enum
{
  OEM7600_IDX_IN_UNIT = 0,
  OEM7600_IDX_MAX,
} OEM7600_IDX;

/**
 * @enum  OEM7600_EL_NOTE
 * @brief OEM7600に関連するELの詳細
 * @note  型はuint8_tを想定
 */
typedef enum
{
  OEM7600_EL_NOTE_CRC_ERROR = 0,
  OEM7600_EL_NOTE_NOISE_REMOVED,
  OEM7600_EL_NOTE_MAX,
} OEM7600_EL_NOTE;

extern const OEM7600_Driver* const oem7600_driver[OEM7600_IDX_MAX];

/**
 * @brief  OEM7600定期実行アプリ生成関数
 * @param  void
 * @return AppInfo
 */
AppInfo DI_OEM7600_update(void);


CCP_CmdRet Cmd_DI_OEM7600_INIT(const CommonCmdPacket* packet);  // TODO_L: 現段階では初期化コマンド不要だが，今後必要に応じて中身を詰める

CCP_CmdRet Cmd_DI_OEM7600_ONOFF_ANTENNA_POWER(const CommonCmdPacket* packet);

CCP_CmdRet Cmd_DI_OEM7600_SOFTWARE_RESET(const CommonCmdPacket* packet);

CCP_CmdRet Cmd_DI_OEM7600_SET_RESET_GPIO_HIGHLOW(const CommonCmdPacket* packet);

CCP_CmdRet Cmd_DI_OEM7600_SET_TLM_CONTENTS(const CommonCmdPacket* packet);

CCP_CmdRet Cmd_DI_OEM7600_SAVE_TLM_SETTING(const CommonCmdPacket* packet);

CCP_CmdRet Cmd_DI_OEM7600_SET_UART_BAUDRATE(const CommonCmdPacket* packet);

#endif
