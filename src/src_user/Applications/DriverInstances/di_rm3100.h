/**
* @file   di_rm3100.h
* @brief  RM3100のDriver Instance
*/
#ifndef DI_RM3100_H_
#define DI_RM3100_H_

#include "../../Drivers/Aocs/rm3100.h"
#include <src_core/System/ApplicationManager/app_info.h>

/**
 * @enum  RM3100_IDX
 * @brief 複数搭載されているRM3100の番号管理
 * @note  型はint8_tを想定
 * @note  要素数が変わった場合、cファイル側の初期化部分の修正が必要になる点に注意する
 */
typedef enum
{
  RM3100_IDX_ON_AOBC  = 0,
  RM3100_IDX_EXTERNAL,
  RM3100_IDX_MAX,
} RM3100_IDX;

/**
 * @enum  RM3100_EL_NOTE
 * @brief RM3100に関連するELの詳細
 * @note  型はuint8_tを想定
 */
typedef enum
{
  RM3100_EL_NOTE_NOISE_REMOVED = 0,
  RM3100_EL_NOTE_MAX,
} RM3100_EL_NOTE;

extern const RM3100_Driver* const rm3100_driver[RM3100_IDX_MAX];

/**
 * @brief  RM3100定期実行アプリ生成関数
 * @param  void
 * @return AppInfo
 */
AppInfo DI_RM3100_update(void);

CCP_EXEC_STS Cmd_DI_RM3100_INIT(const CommonCmdPacket* packet);
CCP_EXEC_STS Cmd_DI_RM3100_SET_FRAME_TRANSFORMATION_QUATERNION_C2B(const CommonCmdPacket* packet);
CCP_EXEC_STS Cmd_DI_RM3100_SET_MAG_BIAS_COMPO_NT(const CommonCmdPacket* packet);

#endif
