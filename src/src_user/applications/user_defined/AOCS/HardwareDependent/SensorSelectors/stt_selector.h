/**
* @file   stt_selector.h
* @brief  複数あるSTT出力を組み合わせてQuaternionを求めるアプリ
* @note   現時点では一つしか搭載されていないので、その一つを使う
*/
#ifndef STT_SELECTOR_H_
#define STT_SELECTOR_H_

#include <stdint.h>
#include <src_core/System/ApplicationManager/app_info.h>

/**
 * @enum   APP_STT_SELECTOR_STATE
 * @brief  どのSTTを選んでいるかなどの状態を表す
 * @note   uint8_tを想定
 */
typedef enum
{
  APP_STT_SELECTOR_STATE_SAGITTA_IN_UNIT = 0,   //!< AOCSユニット内のSAGITTAを使う
} APP_STT_SELECTOR_STATE;

/**
 * @struct SttSelector
 * @brief STTセンサ選択に関する外部公開情報用構造体
 */
typedef struct
{
  APP_STT_SELECTOR_STATE state;
} SttSelector;

extern const SttSelector* const stt_selector;

AppInfo APP_STT_SELECTOR_create_app(void);

#endif
