/**
* @file   aocs_error.h
* @brief  AOCS系各種制御・決定アプリで使われるエラーコードの定義
* @note   setterなどで使うことを想定
*/

#ifndef AOCS_ERROR_H_
#define AOCS_ERROR_H_

/**
 * @enum   AOCS_ERROR
 * @brief  AOCS系各種制御・決定アプリで使われるエラーコードの定義
 * @note   uint8_tを想定
 */
typedef enum
{
  AOCS_ERROR_OK = 0,        //!< 数値エラーなし
  AOCS_ERROR_SINGULAR,      //!< callされた関数の処理内で極端に大きい or 小さい値が発生している
  AOCS_ERROR_RANGE_OVER,    //!< 値が指定された値域を超えている
  AOCS_ERROR_NOT_CONVERGED, //!< 収束すべき値が収束しなかった
  AOCS_ERROR_OTHERS         //!< その他
} AOCS_ERROR;

#endif
