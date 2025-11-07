#ifndef GPIO_H_
#define GPIO_H_

#include <stdint.h>

/**
 * @brief GPIOの方向を定義するためのenum
 * @note  1以外は入力として運用するのが安全なので、そう使ってほしい
 * @note  uint8_tを想定
 */
typedef enum
{
  GPIO_OUTPUT = 1,   //!< 出力
  GPIO_INPUT  = 0,   //!< 入力
} GPIO_DIRECTION;

/**
 * @brief GPIOのHigh/Lowを定義するためのenum
 * @note  High/Lowの定義はPICポートでの論理に従う
 * @note  uint8_tを想定
 */
typedef enum
{
  GPIO_HIGH = 1,   //!< High
  GPIO_LOW  = 0,   //!< Low
} GPIO_HL;

/**
 * @brief GPIOのHigh/Lowを定義するためのenum
 * @note  High/Lowの定義はPICポートでの論理に従う
 * @note  int8_tを想定
 */
typedef enum
{
  GPIO_UNKNOWN_ERR   = -14, //!< 原因不明
  GPIO_LOGIC_ERR     = -3,  //!< 論理指定異常(HIGHでもLOWでも無い)
  GPIO_DIRECTION_ERR = -2,  //!< 方向指定異常
  GPIO_PORT_ERR      = -1,  //!< ポート異常
  GPIO_OK            = 0,   //!< OKは0を踏襲
} GPIO_ERR_CODE;

/**
 * @brief  GPIOの入出力方向を決める関数。
 * @note   内部で各H/Wに依存した方向設定関数を読み出し、入出力を設定する
 *         初期化関数として使われることを想定している。
 * @param  port : 初期化するポート番号
 * @param  direction : 入力か出力かを選択する
 * @return GPIO_ERR_CODEに準拠する
 */
int GPIO_set_direction(const uint8_t port, const GPIO_DIRECTION direction);

/**
 * @brief  GPIO出力ポートの0,1操作
 * @note   内部で各H/Wに依存した出力関数を読み出し、出力設定をする
 * @param  port : 制御するポート番号
 * @param  output : HIGHかLOWかを選択する
 * @return GPIO_ERR_CODEに準拠する
 */
int GPIO_set_output(const uint8_t port, const GPIO_HL output);

/**
 * @brief  GPIO入力ポートの0,1読み出し
 * @note   内部でChipKitの入力読み出し関数を読み出し、入力値を返している
 * @param  port  : 制御するポート番号
 * @return 負    : GPIO_ERR_CODEに準拠する
 * @return 負以外 : GPIO_HLに従ったH/Lを返す
 */
int GPIO_read_input(const uint8_t port);


#endif
