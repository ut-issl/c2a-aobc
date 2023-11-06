/**
* @file   mtq_seiren.h
* @brief  MTQ_SEIREN磁気トルカのドライバ
*/

#ifndef MTQ_SEIREN_H_
#define MTQ_SEIREN_H_

#include <src_core/Drivers/Super/driver_super.h>
#include <src_user/IfWrapper/GPIO.h>
#include <src_user/Library/physical_constants.h>
#include <src_user/Library/c2a_math.h>

/**
 * @struct MTQ_SEIREN_POLARITY
 * @brief  MTQ極性
 * @note   int8_tを想定
 */
typedef enum
{
  MTQ_SEIREN_POLARITY_POSITIVE = 1,  //!< 正方向に出力
  MTQ_SEIREN_POLARITY_NEGATIVE = -1, //!< 負方向に出力
  MTQ_SEIREN_NO_OUTPUT = 0           //!< 出力ゼロ
} MTQ_SEIREN_POLARITY;

/**
 * @struct MTQ_SEIREN_Info
 * @brief MTQのテレメトリ情報
 */
typedef struct
{
  float magnetic_moment_direction_b[PHYSICAL_CONST_THREE_DIM]; //!< 機体座標系での磁気モーメント方向単位ベクトル
} MTQ_SEIREN_Info;

/**
 * @struct MTQ_SEIREN_Driver
 * @brief  MTQ_SEIREN_Driver構造体
 */
typedef struct
{
  struct
  {
    uint8_t ch_gpio_positive;
    uint8_t ch_gpio_negative;
    float max_mag_moment;                 //!< 出力可能な最大磁気モーメント
  } driver;
  MTQ_SEIREN_Info info;
} MTQ_SEIREN_Driver;

// 基本関数

/**
 * @brief  MTQ_SEIREN初期化
 *
 *         MTQ_SEIREN_Driver構造体のポインタを渡すことでポートを初期化し，MTQ_SEIREN_Driverの各変数も初期化する
 * @param  *mtq_seiren         : 初期化するMTQ_SEIREN_Driver構造体へのポインタ
 * @param  ch_gpio_positive    : MTQ_SEIRENが接続されているGPIOポート．MTQ出力の正の極性側．
 * @param  ch_gpio_negative    : MTQ_SEIRENが接続されているGPIOポート．MTQ出力の負の極性側．
 * @return GPIO_ERR_CODEに準じる
 */
GPIO_ERR_CODE MTQ_SEIREN_init(MTQ_SEIREN_Driver* mtq_seiren_driver, uint8_t ch_gpio_positive, uint8_t ch_gpio_negative, float max_mag_moment);

/**
 * @brief  MTQ_SEIREN 磁気モーメント出力
 * @param  MTQ_SEIREN に極性を指定し、磁気モーメントを出力する。出力方向はMTQ_SEIREN_POLARITYの定義に従う。
 * @param  *mtq_seiren_driver    : 出力させるMTQ_SEIREN_Driver構造体へのポインタ
 * @param  polarity              : 出力する極性
*/
GPIO_ERR_CODE MTQ_SEIREN_output(MTQ_SEIREN_Driver* mtq_seiren_driver, MTQ_SEIREN_POLARITY polarity);

/**
 * @brief  磁気モーメント方向ベクトル設定関数
 * @param[in] *mtq_seiren_driver          : MTQ_SEIREN_Driver構造体へのポインタ
 * @param[in] magnetic_moment_direction_b : 機体固定座標系での磁気モーメント方向ベクトル
 * @return C2A_MATH_ERRORに準じる
 */
C2A_MATH_ERROR MTQ_SEIREN_set_magnetic_moment_direction_b(MTQ_SEIREN_Driver* mtq_seiren_driver,
                                                          const float magnetic_moment_direction_b[PHYSICAL_CONST_THREE_DIM]);

#endif
