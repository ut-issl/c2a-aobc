/**
* @file   mtq_seiren.h
* @brief  MTQ_SEIREN磁気トルカのドライバ
*/

#ifndef MTQ_SEIREN_H_
#define MTQ_SEIREN_H_

#include <src_core/Drivers/Super/driver_super.h>
#include <src_core/System/TimeManager/time_manager.h>
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
  MTQ_SEIREN_POLARITY_POSITIVE = 1,
  MTQ_SEIREN_POLARITY_NEGATIVE = -1,
  MTQ_SEIREN_NO_OUTPUT = 0
} MTQ_SEIREN_POLARITY;

/**
 * @struct MTQ_SEIREN_OUT_STATUS
 * @brief  MTQ駆動状態
 * @note   PWM周期が遅いプロジェクト専用のMTQ駆動状態モニタ
 * @note   int8_tを想定
 */
typedef enum
{
  MTQ_SEIREN_ACTIVE = 1,
  MTQ_SEIREN_IDLE   = 0
} MTQ_SEIREN_DRIVE_STATUS;

/**
 * @struct MTQ_SEIREN_Info
 * @brief MTQのテレメトリ情報
 */
typedef struct
{
  int8_t pwm_signed_duty_percent;                              //!< unsigned dutyとpolarityの積で表される，符号付きduty．
  uint32_t pwm_period_ms;                                      //!< PWMの1周期の長さ[ms]
  float magnetic_moment_direction_b[PHYSICAL_CONST_THREE_DIM]; //!< 機体座標系での磁気モーメント方向単位ベクトル
} MTQ_SEIREN_Info;

/**
 * @struct MTQ_SEIREN_Driver
 * @brief  MTQ_SEIREN_Driver構造体
 */
typedef struct
{
  // GPIOをどう扱うかは要議論
  struct
  {
    uint8_t ch_gpio_positive;
    uint8_t ch_gpio_negative;
    MTQ_SEIREN_POLARITY polarity;         //!< MTQ極性
    float max_mag_moment;                 //!< 出力可能な最大磁気モーメント
    MTQ_SEIREN_DRIVE_STATUS drive_status; //!< MTQ駆動状態 (PWM周期が遅いプロジェクト専用)
    uint8_t unsigned_duty_percent_buffer; //!< 上位アプリからCMD指示されたPWMの符号なしduty (PWMタイマがゼロの時にunsigned_duty_percentへ反映)
    MTQ_SEIREN_POLARITY polarity_buffer;  //!< 上位アプリからCMD指示されたMTQ駆動極性
    struct
    {
      uint8_t unsigned_duty_percent;  //!< PWMの符号なしduty
      ObcTime last_set_time;          //!< 前回実行時のOBC時刻
      uint32_t internal_timer_ms;     //!< PWMタイマカウンタ．pwm_period_ms_まで増加したら0に戻る
    } pwm_info;
  } driver;
  MTQ_SEIREN_Info info;
} MTQ_SEIREN_Driver;

// (現状では) PWM制御周期が遅く，消磁のためのOn/Off処理とこの値の大小に応じた処理分岐が必要な状況なので，外部参照可能にする
extern const uint32_t* MTQ_SEIREN_pwm_period_ms;

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
 * @brief  MTQ_SEIREN duty，極性設定
 *
 *         MTQ_SEIREN_Driver構造体のポインタを渡し，PWMのdutyを設定する
 * @param  *mtq_seiren        : 初期化するMTQ_SEIREN_Driver構造体へのポインタ
 * @param  signed_duty        : 設定する符号つきduty値[％]
 * @return DS_CMD_ERR_CODEに準じる
 */
DS_CMD_ERR_CODE MTQ_SEIREN_set_pwm_signed_duty(MTQ_SEIREN_Driver* mtq_seiren_driver, int8_t pwm_signed_duty_percent);

/**
 * @brief  MTQ_SEIREN PWM周期設定
 *
 *         MTQ_SEIREN_Driver構造体のポインタを渡し，PWMのdutyを設定する
 * @param  *mtq_seiren   : 初期化するMTQ_SEIREN_Driver構造体へのポインタ
 * @param  pwm_period_us : 設定するpwm周期
 * @return DS_CMD_ERR_CODEに準じる
 */
DS_CMD_ERR_CODE MTQ_SEIREN_set_pwm_period_ms(uint32_t pwm_period_ms);

/**
 * @brief  MTQ_SEIREN PWM出力
 *
 *         MTQ_SEIREN_Driver構造体のポインタを渡し，PWMのdutyを設定する
 * @param  *mtq_seiren   : MTQ_SEIREN_Driver構造体へのポインタ
 * @return DS_CMD_ERR_CODEに準じる
 */
DS_CMD_ERR_CODE MTQ_SEIREN_output_pwm(MTQ_SEIREN_Driver* mtq_seiren_driver);

/**
 * @brief  磁気モーメント方向ベクトル設定関数
 * @param[in] *mtq_seiren_driver          : MTQ_SEIREN_Driver構造体へのポインタ
 * @param[in] magnetic_moment_direction_b : 機体固定座標系での磁気モーメント方向ベクトル
 * @return C2A_MATH_ERRORに準じる
 */
C2A_MATH_ERROR MTQ_SEIREN_set_magnetic_moment_direction_b(MTQ_SEIREN_Driver* mtq_seiren_driver,
                                                          const float magnetic_moment_direction_b[PHYSICAL_CONST_THREE_DIM]);

#endif
