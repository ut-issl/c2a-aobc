/**
* @file
* @brief BCT のコピーを不揮発メモリに保存する
* @note ## コピー: AOBC -> 不揮発メモリ
*        - アプリ有効化中は, BC が bc_num_to_copy 個 / cycle ずつ不揮発メモリに自動コピーされる
*          - bc_num_to_copy = 1 の場合, 80 cycle = 8 秒で全ての BC をバックアップし終える
*          - つまり, BL コマンドで BC を編集した際も, 8 秒後には必ずバックアップされる
*        - 無効化されている BC は ready flag が下がり, リセット後に復元できなくなる
*          - 空き番の BC や意図的に使っていない BC がコピーされて復元されて, それが間違って使われることを防ぐため
* @note ## 復元: AOBC <- 不揮発メモリ
*        - Cmd_APP_NVM_BC_RESTORE_BC_FROM_NVM {bc_id} で一つずつ復元する
*        - ready flag が立っていない BC は復元できない. 逆に, flag が立っていれば確実にコピーが取れているはず.
*        - 復元された BC は activate された状態になるので, すぐに使える
* @note ## 使い方
*       0. 起動時はデフォルトでアプリ無効化されている
*       1. Cmd_APP_NVM_BC_SET_ENABLE で有効化すると BC が不揮発メモリに自動コピーされ始める
*       2. AOBC がリセットしてしまったら, 復元したい BC だけ Cmd_APP_NVM_BC_RESTORE_BC_FROM_NVM を用いて1つずつ復元する
*          復元前にアプリ有効化してしまうと, コピーが上書きされて消えてしまうので注意
*       3. 復元が完了したら Cmd_APP_NVM_BC_SET_ENABLE でアプリ有効化して自動コピーを再開する
* @note ## 注意点
*        - 基本的には bc_num_to_copy = 1 にすること. 2 以上になると 3-3 アノマリが出る
*          - 不揮発メモリの読み書きは実行時間を食う
*        - AOBC リセット後は, BC 復元が完了するまでアプリを有効化しないように注意！
*/
#ifndef NON_VOLATILE_MEMORY_BC_H_
#define NON_VOLATILE_MEMORY_BC_H_

#include "non_volatile_memory_partition.h"
#include <src_core/System/ApplicationManager/app_info.h>
#include <src_core/TlmCmd/block_command_table.h>


/**
 * @struct
 * @brief 不揮発BC管理アプリの構造体
 */
typedef struct
{
  uint8_t is_active;                           //!< アプリが有効かどうか
  uint8_t is_ready_to_restore[BCT_MAX_BLOCKS]; //!< 各 BC が復元できる状態かどうか
  bct_id_t bc_id_to_copy;                      //!< 次にコピーする BC の ID
  uint8_t bc_num_to_copy;                      /*!< 一度に何個の BC をコピーするか
                                                    2 以上にすると実行時間超過で 3-3 アノマリが出たので, 基本 1 にすること */
  // 以下二つはパーティッション内のアドレスであることに注意
  uint32_t address_for_ready_flags;            //!< is_ready_to_restore を保存している領域の開始アドレス
  uint32_t address_for_bc;                     //!< BCT を保存している領域の開始アドレス
} NonVolatileMemoryBc;

extern const NonVolatileMemoryBc* const nvm_bc;

AppInfo APP_NVM_BC_create_app(void);

CCP_CmdRet Cmd_APP_NVM_BC_SET_ENABLE(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_APP_NVM_BC_RESTORE_BC_FROM_NVM(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_APP_NVM_BC_OTHER_SETTINGS(const CommonCmdPacket* packet);

#endif
