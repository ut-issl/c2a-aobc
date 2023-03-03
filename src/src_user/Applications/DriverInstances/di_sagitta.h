/**
* @file   di_sagitta.h
* @brief  SAGITTAのDriver Instance
*/
#ifndef DI_SAGITTA_H_
#define DI_SAGITTA_H_

#include "../../Drivers/Aocs/sagitta.h"
#include <src_core/System/ApplicationManager/app_info.h>

/**
 * @enum  SAGITTA_IDX
 * @brief 搭載されているSAGITTAの管理番号 (今は1つのみ)
 * @note  uint8_tを想定
 * @note  要素数が変わった場合、cファイル側の初期化部分の修正が必要になる点に注意する
 */
typedef enum
{
  SAGITTA_IDX_IN_UNIT = 0,
  SAGITTA_IDX_MAX,
} SAGITTA_IDX;

/**
 * @enum  SAGITTA_EL_NOTE
 * @brief SAGITTAに関連するELの詳細
 * @note  型はuint8_tを想定
 */
typedef enum
{
  SAGITTA_EL_NOTE_XXHASH_ERROR = 0,
  SAGITTA_EL_NOTE_NOISE_REMOVED,
  SAGITTA_EL_NOTE_MAX,
} SAGITTA_EL_NOTE;

extern const SAGITTA_Driver* const sagitta_driver[SAGITTA_IDX_MAX];

/**
 * @brief  SAGITTA定期実行アプリ生成関数
 * @param  void
 * @return AppInfo
 */
AppInfo DI_SAGITTA_update(void);

// Action
CCP_EXEC_STS Cmd_DI_SAGITTA_BOOT(const CommonCmdPacket* packet);
CCP_EXEC_STS Cmd_DI_SAGITTA_SET_UNIX_TIME_US(const CommonCmdPacket* packet);
// Set parameter: Sagittaにparameter設定コマンドを送る
CCP_EXEC_STS Cmd_DI_SAGITTA_SET_PARAMETER(const CommonCmdPacket* packet);
// Change parameter: ドライバ内のparameterを変更する。Sagittaに送信するのはCmd_DI_SAGITTA_SET_PARAMETER。
// パラメータの値に対するアサーションはない。
// Cmd_DI_SAGITTA_READ_PARAMETERにより設定した値が正常であることを地上局で確認する。
CCP_EXEC_STS Cmd_DI_SAGITTA_CHANGE_PARAMETER(const CommonCmdPacket* packet);
// Read parameter
CCP_EXEC_STS Cmd_DI_SAGITTA_READ_PARAMETER(const CommonCmdPacket* packet);

CCP_EXEC_STS Cmd_DI_SAGITTA_SET_FRAME_TRANSFORMATION_QUATERNION_C2B(const CommonCmdPacket* packet);

#endif
