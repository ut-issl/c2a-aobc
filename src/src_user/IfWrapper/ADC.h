/**
* @file   ADC.h
* @brief  ADCのIF．公開関数を提供.
* @note   現時点ではArduino/ChipKit環境前提の作りになっている
*/

#ifndef ADC_H_
#define ADC_H_

#include "../Library/stdint.h"

/**
* @brief  ADCの初期化
* @note   内部でH/W依存の初期パラメタ設定を実行する
* @param  init_param : 初期化設定（何かあるなら使う）
* @return 0     : 正常終了
* @return 0以外 : 異常終了
*/
int ADC_initialize(unsigned int init_param);

/**
 * @brief  ADCのデータ取得
 * @note   内部でH/W依存の読み出し関数を実行する
 * @param  ch : port_config.hに対応したADCチャンネル
 * @return : ADC計測値。なにかのエラー時は0を返す（ので，ずっと0だと何かがおかしいはず）
 */
int ADC_get_value(uint8_t ch);


#endif
