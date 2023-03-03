/**
 * @file
 * @brief  座標変換等に用いるEuler角関連の定数・関数群の定義
 */
#ifndef EULER_ANGLE_H_
#define EULER_ANGLE_H_


 /**
  * @enum   EULER_ANGLE_ROTATION_ORDER
  * @brief  Euler角で回転を指定する際の回転順序の指定
  * @note   uint8_tを想定
  */
typedef enum
{
  EULER_ANGLE_ROTATION_ORDER_123 = 0,
  EULER_ANGLE_ROTATION_ORDER_321 = 1,
  // TODO_L: 他定義を使う必要があれば適宜追加する
  EULER_ANGLE_ROTATION_ORDER_121 = 5  // just a example of i-j-i case
} EULER_ANGLE_ROTATION_ORDER;


// TODO_L: Euler角関連の回転処理とかが必要であれば関数追加

#endif
