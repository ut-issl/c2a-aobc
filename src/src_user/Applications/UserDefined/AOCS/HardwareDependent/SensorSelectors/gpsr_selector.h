/**
* @file   gpsr_selector.h
* @brief  複数あるGPSR出力を組み合わせて位置・時刻を求めるアプリ
* @note   現時点では一つしか搭載されていないので、その一つを使う
*/
#ifndef GPSR_SELECTOR_H_
#define GPSR_SELECTOR_H_

#include <src_core/System/ApplicationManager/app_info.h>
#include "../../../../../Library/stdint.h"

/**
 * @enum   APP_GPSR_SELECTOR_STATE
 * @brief  どのGPSRを選んでいるかなどの状態を表す
 * @note   uint8_tを想定
 */
typedef enum
{
  APP_GPSR_SELECTOR_STATE_OEM_IN_UNIT = 0,   //!< AOCSユニット内のOEMを使う
} APP_GPSR_SELECTOR_STATE;

/**
 * @struct GpsrSelector
 * @brief GPSR選択に関する外部公開情報用構造体
 */
typedef struct
{
  APP_GPSR_SELECTOR_STATE state;
} GpsrSelector;

extern const GpsrSelector* const gpsr_selector;

AppInfo APP_GPSR_SELECTOR_create_app(void);

#endif
