/**
* @file   non_volatile_memory_manager.h
* @brief  不揮発メモリ管理をするアプリ
*/
#ifndef NON_VOLATILE_MEMORY_MANAGER_H_
#define NON_VOLATILE_MEMORY_MANAGER_H_

#include <src_core/System/ApplicationManager/app_info.h>
#include <src_core/TlmCmd/common_cmd_packet.h>

// 下記は使うH/Wによって修正する
#include <src_user/Applications/DriverInstances/di_fm25v10.h>

#define APP_NVM_MANAGER_STOP_ADDRESS (DI_FM25V10_STOP_GLOBAL_ADDRESS)  //!< 不揮発メモリ管理アプリで扱う最大アドレス
#define APP_NVM_MANAGER_MAX_LENGTH  (DI_FM25V10_MAX_LENGTH)            //!< 不揮発メモリ管理アプリで扱う最大アドレス

// 下記はH/Wに依存しない
#define APP_NVM_MANAGER_MAX_CMD_DATA_SIZE (8) //!< 書き込み/読み出しコマンドで指定する最大サイズ [byte] (double型まで想定)

/**
 * @enum  APP_NVM_MANAGER_ERROR
 * @brief エラー用enum
 * @note  型はuint8_tを想定
 */
typedef enum
{
  APP_NVM_MANAGER_ERROR_OK = 0,     //!< エラーなし
  APP_NVM_MANAGER_ERROR_NG_ADDRESS, //!< 先頭アドレス指定エラー
  APP_NVM_MANAGER_ERROR_NG_LENGTH,  //!< データ長関連エラー
  APP_NVM_MANAGER_ERROR_NG_READ,    //!< 読み出しエラー
  APP_NVM_MANAGER_ERROR_NG_WRITE,   //!< 書き込みエラー
  APP_NVM_MANAGER_ERROR_NG_OTHERS,  //!< その他未分類のエラー
} APP_NVM_MANAGER_ERROR;

/**
 * @struct APP_NVM_MANAGER_InfoForTlm
 * @brief  不揮発メモリ全体管理のテレメ表示用の基礎構造体
 */
typedef struct
{
  uint32_t start_address;                           //!< 先頭アドレス
  uint8_t  data_size_byte;                          //!< データサイズ
  uint8_t  data[APP_NVM_MANAGER_MAX_CMD_DATA_SIZE]; //!< データ
} APP_NVM_MANAGER_InfoForTlm;

/**
 * @struct NonVolatileMemoryManager
 * @brief  不揮発メモリ管理アプリの地上コマンドに対する外部公開情報用構造体
 */
typedef struct
{
  APP_NVM_MANAGER_ERROR error_status;     //!< エラーステータス
  APP_NVM_MANAGER_InfoForTlm write_info;  //!< 書き込み情報
  APP_NVM_MANAGER_InfoForTlm read_info;   //!< 読み出し情報
} NonVolatileMemoryManager;

extern const NonVolatileMemoryManager* const non_volatile_memory_manager;

AppInfo APP_NVM_MANAGER_create_app(void);

CCP_CmdRet Cmd_APP_NVM_MANAGER_WRITE_BYTES(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_APP_NVM_MANAGER_READ_BYTES(const CommonCmdPacket* packet);

// 外部公開関数
/**
 * @brief データ書き込み関数
 * @param[in] start_address : 先頭アドレス
 * @param[in] length        : 書き込みデータ長
 * @param[in] write_data    : 書き込みデータ
 * @return                  : APP_NVM_MANAGER_ERRORに準拠
 */
APP_NVM_MANAGER_ERROR APP_NVM_MANAGER_write_bytes(const uint32_t start_address, const uint32_t length, const uint8_t* write_data);
/**
 * @brief データ読み出し関数
 * @param[out] write_data    : 読み出しデータ
 * @param[in]  start_address : 先頭アドレス
 * @param[in]  length        : 読み出しデータ長
 * @return                   : APP_NVM_MANAGER_ERRORに準拠
 */
APP_NVM_MANAGER_ERROR APP_NVM_MANAGER_read_bytes(uint8_t* read_data, const uint32_t start_address, const uint32_t length);

#endif
