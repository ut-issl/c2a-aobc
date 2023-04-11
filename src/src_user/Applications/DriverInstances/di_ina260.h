/**
* @file   di_ina260.h
* @brief  INA260のDriver Instance
*/
#ifndef DI_INA260_H_
#define DI_INA260_H_

#include "../../Drivers/Power/ina260.h"
#include <src_core/System/ApplicationManager/app_info.h>

/**
 * @enum  INA260_IDX
 * @brief 複数搭載されているINA260の番号管理
 * @note  型はuint8_tを想定
 * @note  要素数が変わった場合、cファイル側の初期化部分の修正が必要になる点に注意する
 */
typedef enum
{
  INA260_IDX_PIC = 0,
  INA260_IDX_STIM210,
  INA260_IDX_SAGITTA,
  INA260_IDX_OEM7600,
  INA260_IDX_RM3100,
  INA260_IDX_NANOSSOC_D60,
  INA260_IDX_MTQ,
  INA260_IDX_RW0003_X,
  INA260_IDX_RW0003_Y,
  INA260_IDX_RW0003_Z,
  INA260_IDX_MAX,
} INA260_IDX;

extern const INA260_Driver* const ina260_driver[INA260_IDX_MAX];
extern const INA260_IDX* const DI_INA260_idx_counter;

/**
 * @brief  INA260定期実行アプリ生成関数
 * @param  void
 * @return AppInfo
 */
AppInfo DI_INA260_update(void);

CCP_CmdRet Cmd_DI_INA260_INIT(const CommonCmdPacket* packet);

CCP_CmdRet Cmd_DI_INA260_SET_OVER_CURRENT_PROTECTION(const CommonCmdPacket* packet);

#endif
