/**
 * @file  parameter_setting_macro.h
 * @brief 衛星固有パラメータの数値設定で使われるマクロをまとめたヘッダファイル．
 * @note  staticな衛星固有パラメータの初期化子は定数式か文字列リテラルである必要があり，ここに関数式を含めることができない．
 *        関数式を呼び出した場合，initializer element is not constantビルドエラーとなる．
 */

#ifndef PARAMETER_SETTING_MACRO_H_
#define PARAMETER_SETTING_MACRO_H_

#include <src_user/Library/math_constants.h>

/**
 * @brief     degからradに変換する関数マクロ
 * @param[in] deg : 角度[deg]
*/
#define PARAMETER_SETTING_MACRO_DEGREE_TO_RADIAN(degree) ((degree) * (MATH_CONST_PI) / 180.0f)

/**
 * @brief     RPMからrad/sに変換する関数マクロ
 * @param[in] rpm : 回転角速度[rpm]
*/
#define PARAMETER_SETTING_MACRO_RPM_TO_RADIAN_SEC(rpm) ((rpm) * MATH_CONST_PI / 30.0f)

#endif // PARAMETER_SETTING_MACRO_H_
