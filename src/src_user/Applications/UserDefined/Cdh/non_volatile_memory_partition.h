/**
* @file   non_volatile_partition.h
* @brief  不揮発メモリアクセス用のパーティションを定義する
*/
#ifndef NON_VOLATILE_MEMORY_PARTITION_H_
#define NON_VOLATILE_MEMORY_PARTITION_H_

#include <stdint.h>
#include "non_volatile_memory_manager.h"
#include <src_core/System/ApplicationManager/app_info.h>
#include <src_core/TlmCmd/common_cmd_packet.h>

/**
 * @enum  APP_NVM_PARTITION_ID
 * @brief 不揮発メモリのパーティションID定義
 * @note  型はuint8_tを想定
 */
typedef enum
{
  APP_NVM_PARTITION_ID_NORMAL_PARAMS = 0,   //!< 冗長化しない通常のパラメータ
  APP_NVM_PARTITION_ID_REDUNDANT_PARAMS_1,  //!< 三重冗長するパラメータ, 基本的には1のパーティションにアクセスして読み書きする
  APP_NVM_PARTITION_ID_DR_1,                //!< データレコーダ用1
  APP_NVM_PARTITION_ID_DR_2,                //!< データレコーダ用2
  APP_NVM_PARTITION_ID_DR_3,                //!< データレコーダ用3
  APP_NVM_PARTITION_ID_BCT,                 //!< Block Command Table用
  APP_NVM_PARTITION_ID_REDUNDANT_PARAMS_2,  //!< 三重冗長するパラメータ, 基本的にアクセスしない
  APP_NVM_PARTITION_ID_REDUNDANT_PARAMS_3,  //!< 三重冗長するパラメータ, 基本的にアクセスしない
  APP_NVM_PARTITION_ID_MAX,
} APP_NVM_PARTITION_ID;

/**
 * @struct APP_NVM_PARTITION_Elements
 * @brief  不揮発メモリパーティション管理で必要な要素
 */
typedef struct
{
  uint32_t start_address; //!< パーティションの先頭アドレス
  uint32_t size_byte;     //!< パーティションのサイズ [byte]
} APP_NVM_PARTITION_Elements;

/**
 * @struct APP_NVM_PARTITION_InfoForTlm
 * @brief  不揮発メモリパーティション管理のテレメ表示用の基礎構造体
 */
typedef struct
{
  APP_NVM_PARTITION_ID  partition_id;               //!< パーティションID
  uint32_t start_address;                           //!< 先頭アドレス
  uint8_t  data_size_byte;                          //!< データサイズ
  uint8_t  data[APP_NVM_MANAGER_MAX_CMD_DATA_SIZE]; //!< データ
} APP_NVM_PARTITION_InfoForTlm;

/**
 * @struct APP_NVM_Element
 * @brief  不揮発メモリパーティション分割用構造体
 */
typedef struct
{
  APP_NVM_PARTITION_Elements elements[APP_NVM_PARTITION_ID_MAX];  //!< 登録されたパーティションの基本情報リスト

  // 地上コマンド用テレメ
  APP_NVM_MANAGER_ERROR error_status;       //!< エラーステータス
  APP_NVM_PARTITION_InfoForTlm write_info;  //!< 書き込み情報
  APP_NVM_PARTITION_InfoForTlm read_info;   //!< 読み出し情報
} NonVolatileMemoryPartition;

extern const NonVolatileMemoryPartition* const non_volatile_memory_partition;

AppInfo APP_NVM_PARTITION_create_app(void);

CCP_CmdRet Cmd_APP_NVM_PARTITION_WRITE_BYTES(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_APP_NVM_PARTITION_READ_BYTES(const CommonCmdPacket* packet);

// 外部公開関数
/**
 * @brief  パーティション毎の不揮発メモリ書き込み
 * @param[in]  id            : パーティションID
 * @param[in]  start_address : パーティション内での書き込み先頭アドレス
 * @param[in]  size_byte     : 書き込むデータサイズ
 * @param[in]  write_data    : 書き込むデータ
 * @return                   : APP_NVM_MANAGER_ERRORに従う
 */
APP_NVM_MANAGER_ERROR APP_NVM_PARTITION_write_bytes(APP_NVM_PARTITION_ID id, uint32_t start_address, uint32_t size_byte, uint8_t* write_data);
/**
 * @brief  パーティション毎の不揮発メモリ読み出し
 * @param[out] read_data     : 読み出しデータ
 * @param[in]  id            : パーティションID
 * @param[in]  start_address : パーティション内での読み出し先頭アドレス
 * @param[in]  size_byte     : 読み出しデータサイズ
 * @return                   : APP_NVM_MANAGER_ERRORに従う
 */
APP_NVM_MANAGER_ERROR APP_NVM_PARTITION_read_bytes(uint8_t* read_data, APP_NVM_PARTITION_ID id, uint32_t start_address, uint32_t size_byte);

#endif
