/**
* @file   di_nanossoc_d60.h
* @brief  nanoSSOC-D60のDriver Instance
*/
#ifndef DI_NANOSSOC_D60_H_
#define DI_NANOSSOC_D60_H_

#include "../../Drivers/Aocs/nanossoc_d60.h"
#include <src_core/System/ApplicationManager/app_info.h>

/**
 * @enum  NANOSSOC_D60_IDX
 * @brief 複数搭載されているNANOSSOC_D60の番号管理
 * @note  型はuint8_tを想定
 * @note  要素数が変わった場合、cファイル側の初期化部分の修正が必要になる点に注意する
 */
typedef enum
{
  NANOSSOC_D60_IDX_ON_PY = 0,
  NANOSSOC_D60_IDX_ON_MY,
  NANOSSOC_D60_IDX_ON_PZ,
  NANOSSOC_D60_IDX_ON_MZ,
  NANOSSOC_D60_IDX_MAX,
} NANOSSOC_D60_IDX;

/**
 * @enum  NANOSSOC_D60_EL_NOTE
 * @brief NANOSSOC_D60に関連するELの詳細
 * @note  型はuint8_tを想定
 */
typedef enum
{
  NANOSSOC_D60_EL_NOTE_CHECKSUM_ERROR = 0,
  NANOSSOC_D60_EL_NOTE_NOISE_REMOVED,
  NANOSSOC_D60_EL_NOTE_MAX,
} NANOSSOC_D60_EL_NOTE;

extern const NANOSSOC_D60_Driver* const nanossoc_d60_driver[NANOSSOC_D60_IDX_MAX];

/**
 * @brief  NANOSSOC_D60定期実行アプリ生成関数
 * @param  void
 * @return AppInfo
 * @note   サンセンサのI2C通信の間隔は1msほどあける必要があるが，delay関数は使えないので，1回にupdateするサンセンサは1個のみである．
 * @note   したがって，TLではNanoSSOC-D60を4回登録する必要がある．
 */
AppInfo DI_NANOSSOC_D60_update(void);

CCP_CmdRet Cmd_DI_NANOSSOC_D60_SET_FRAME_TRANSFORMATION_QUATERNION_C2B(const CommonCmdPacket* packet);

#endif
