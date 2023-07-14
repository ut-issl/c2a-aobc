/**
* @file
* @brief  FM25V10のDriver Instance
*/
#ifndef DI_FM25V10_H_
#define DI_FM25V10_H_

#include <src_user/Drivers/Cdh/fm25v10.h>
#include <src_core/System/ApplicationManager/app_info.h>

/**
 * @enum  FM25V10_IDX
 * @brief 複数搭載されているFM25V10の番号管理
 * @note  型はuint8_tを想定
 * @note  要素数が変わった場合、cファイル側の初期化部分の修正が必要になる点に注意する
 */
typedef enum
{
  FM25V10_IDX_1 = 0,
  FM25V10_IDX_2,
  FM25V10_IDX_3,
  FM25V10_IDX_4,
  FM25V10_IDX_MAX,
} FM25V10_IDX;

/**
 * @enum  FM25V10_ERROR
 * @brief エラー用enum
 * @note  型はuint8_tを想定
 */
typedef enum
{
  DI_FM25V10_ERROR_OK = 0,      //!< エラーなし
  DI_FM25V10_ERROR_NG_ADDRESS,  //!< 先頭アドレス関連エラー
  DI_FM25V10_ERROR_NG_LENGTH,   //!< データ長関連エラー
  DI_FM25V10_ERROR_NG_READ,     //!< 読み出しエラー
  DI_FM25V10_ERROR_NG_WRITE,    //!< 書き込みエラー
} DI_FM25V10_ERROR;

#define DI_FM25V10_STOP_GLOBAL_ADDRESS (FM25V10_STOP_ADDRESS * FM25V10_IDX_MAX) //!< 複数のFM25V10をまとめて一つのアドレスで管理するときのSTOPアドレス(排他範囲)
#define DI_FM25V10_MAX_LENGTH          (FM25V10_MAX_LENGTH)                     //!< アクセス可能な最大データ長

extern const FM25V10_Driver* const fm25v10_driver[FM25V10_IDX_MAX];

/**
 * @brief  FM25V10定期実行アプリ生成関数
 * @param  void
 * @return AppInfo
 */
AppInfo DI_FM25V10_update(void);

// コマンド関数
CCP_CmdRet Cmd_DI_FM25V10_WRITE_BYTE(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_DI_FM25V10_READ_BYTE (const CommonCmdPacket* packet);
CCP_CmdRet Cmd_DI_FM25V10_MANAGE_STATUS(const CommonCmdPacket* packet);

// 外部公開関数
/**
 * @brief  複数のFM25V10をまとめて管理してデータの書き込みを行う関数
 * @param[in]  global_start_address : 複数のFRAMをまとめたときの先頭アドレス
 * @param[in]  length               : 書き込みデータ長
 * @param[in]  write_data           : 書き込みデータ
 * @return                          : FM25V10_ERRORに準拠
 */
DI_FM25V10_ERROR DI_FM25V10_global_write_bytes(const uint32_t global_start_address, const uint32_t length, const uint8_t* write_data);

/**
 * @brief  複数のFM25V10をまとめて管理してデータの読み出しを行う関数
 * @param[out] read_data            : 読み出しデータ
 * @param[in]  global_start_address : 複数のFRAMをまとめたアドレス
 * @param[in]  length               : 読み出しデータ長
 * @return                          : FM25V10_ERRORに準拠
 */
DI_FM25V10_ERROR DI_FM25V10_global_read_bytes(uint8_t* read_data, const uint32_t global_start_address, const uint32_t length);

#endif
