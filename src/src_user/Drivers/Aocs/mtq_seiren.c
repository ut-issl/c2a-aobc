#pragma section REPRO
/**
* @file   mtq_seiren.c
* @brief  MTQ_SEIRENのDriver
*/

#include "mtq_seiren.h"

#include  <src_core/System/TimeManager/time_manager.h>
#include <src_user/Library/vector3.h>


static uint32_t MTQ_SEIREN_pwm_period_ms_ = 1000; // TODO_L: 今はコマンドで自由に変更が効くようになっているが、const化するかどうか検討が必要
const uint32_t* MTQ_SEIREN_pwm_period_ms = &MTQ_SEIREN_pwm_period_ms_;

const static uint8_t MTQ_SEIREN_kUnsignedDutyMax = 100;

static void MTQ_SEIREN_update_internal_pwm_timer_(MTQ_SEIREN_Driver* mtq_seiren_driver);
static GPIO_ERR_CODE MTQ_SEIREN_output_turn_on_(MTQ_SEIREN_Driver* mtq_seiren_driver);
static GPIO_ERR_CODE MTQ_SEIREN_output_turn_off_(MTQ_SEIREN_Driver* mtq_seiren_driver);

GPIO_ERR_CODE MTQ_SEIREN_init(MTQ_SEIREN_Driver* mtq_seiren_driver, uint8_t ch_gpio_positive, uint8_t ch_gpio_negative, float max_mag_moment)
{
  GPIO_ERR_CODE ret;

  mtq_seiren_driver->driver.ch_gpio_positive = ch_gpio_positive;
  mtq_seiren_driver->driver.ch_gpio_negative = ch_gpio_negative;

  // この2行でOUTPUTにできなかった場合、output_turn_offの内部でエラーが出るので、ここではアサーションが必要ない
  GPIO_set_direction(ch_gpio_positive, GPIO_OUTPUT);
  GPIO_set_direction(ch_gpio_negative, GPIO_OUTPUT);

  ret = (GPIO_ERR_CODE)MTQ_SEIREN_output_turn_off_(mtq_seiren_driver);

  mtq_seiren_driver->driver.pwm_info.unsigned_duty_percent = 0;
  mtq_seiren_driver->driver.polarity                       = MTQ_SEIREN_NO_OUTPUT;
  mtq_seiren_driver->driver.max_mag_moment                 = max_mag_moment;
  mtq_seiren_driver->info.pwm_signed_duty_percent          = 0;
  mtq_seiren_driver->driver.pwm_info.last_set_time         = TMGR_get_master_clock();
  mtq_seiren_driver->driver.pwm_info.internal_timer_ms     = 950;
  mtq_seiren_driver->driver.polarity_buffer                = MTQ_SEIREN_NO_OUTPUT;
  mtq_seiren_driver->driver.unsigned_duty_percent_buffer   = 0;

  mtq_seiren_driver->info.magnetic_moment_direction_b[0] = 1.0f;
  mtq_seiren_driver->info.magnetic_moment_direction_b[1] = 0.0f;
  mtq_seiren_driver->info.magnetic_moment_direction_b[2] = 0.0f;

  return ret;
}

DS_CMD_ERR_CODE MTQ_SEIREN_set_pwm_signed_duty(MTQ_SEIREN_Driver* mtq_seiren_driver, int8_t pwm_signed_duty_percent)
{
  mtq_seiren_driver->info.pwm_signed_duty_percent = pwm_signed_duty_percent;

  // PWM制御で使いやすいように、符号付きdutyを極性と符号なしdutyに分離して格納する
  // PWMの途中で設定が書き換わることを避けるため，ここではCMDバッファ値のみ書換，PWMタイマのゼロにてpwm_infoに反映する
  if (pwm_signed_duty_percent > 0)
  {
    mtq_seiren_driver->driver.polarity_buffer = MTQ_SEIREN_POLARITY_POSITIVE;
    mtq_seiren_driver->driver.unsigned_duty_percent_buffer = (uint8_t)pwm_signed_duty_percent;
  }
  else if (pwm_signed_duty_percent < 0)
  {
    mtq_seiren_driver->driver.polarity_buffer = MTQ_SEIREN_POLARITY_NEGATIVE;
    mtq_seiren_driver->driver.unsigned_duty_percent_buffer = (uint8_t)(-1.0f * pwm_signed_duty_percent);
  }
  else
  {
    mtq_seiren_driver->driver.polarity_buffer = MTQ_SEIREN_NO_OUTPUT;
    mtq_seiren_driver->driver.unsigned_duty_percent_buffer = 0;
  }

  return DS_CMD_OK;
}

DS_CMD_ERR_CODE MTQ_SEIREN_set_pwm_period_ms(uint32_t pwm_period_ms)
{
  MTQ_SEIREN_pwm_period_ms_ = pwm_period_ms;
  return DS_CMD_OK;
}

DS_CMD_ERR_CODE MTQ_SEIREN_output_pwm(MTQ_SEIREN_Driver* mtq_seiren_driver)
{
  GPIO_ERR_CODE ret;

  MTQ_SEIREN_update_internal_pwm_timer_(mtq_seiren_driver);

  // PWMの途中で設定が書き換わることを避けるため，PWMタイマのゼロでのみCMD値を反映する
  if (mtq_seiren_driver->driver.pwm_info.internal_timer_ms == 0)
  {
    mtq_seiren_driver->driver.pwm_info.unsigned_duty_percent =
    mtq_seiren_driver->driver.unsigned_duty_percent_buffer;

    mtq_seiren_driver->driver.polarity = mtq_seiren_driver->driver.polarity_buffer;
  }

  uint32_t pwm_pulse_width_ms = (uint32_t)(MTQ_SEIREN_pwm_period_ms_ *
                                           mtq_seiren_driver->driver.pwm_info.unsigned_duty_percent /
                                           MTQ_SEIREN_kUnsignedDutyMax);

  if (mtq_seiren_driver->driver.pwm_info.internal_timer_ms < pwm_pulse_width_ms)
  {
    ret = MTQ_SEIREN_output_turn_on_(mtq_seiren_driver);
    if (ret != GPIO_OK) return DS_CMD_DRIVER_SUPER_ERR;
  }
  else
  {
    ret = MTQ_SEIREN_output_turn_off_(mtq_seiren_driver);
    if (ret != GPIO_OK) return DS_CMD_DRIVER_SUPER_ERR;
  }

  return DS_CMD_OK;
}

C2A_MATH_ERROR MTQ_SEIREN_set_magnetic_moment_direction_b(MTQ_SEIREN_Driver* mtq_seiren_driver,
                                                          const float magnetic_moment_direction_b[PHYSICAL_CONST_THREE_DIM])
{
  C2A_MATH_ERROR is_normalized = VECTOR3_is_normalized(magnetic_moment_direction_b);
  if (is_normalized != C2A_MATH_ERROR_OK) return is_normalized;
  VECTOR3_copy(mtq_seiren_driver->info.magnetic_moment_direction_b,
              magnetic_moment_direction_b);
  return C2A_MATH_ERROR_OK;
}


/**
 * @brief  MTQ_SEIREN PWMタイマアップデート
 *
 *         MTQ_SEIREN_Driver構造体の内部のPWMタイマを進める．PWMの1周期を超えたら，タイマをリセットする．
 * @param  *mtq_seiren   : MTQ_SEIREN_Driver構造体へのポインタ
 */
static void MTQ_SEIREN_update_internal_pwm_timer_(MTQ_SEIREN_Driver* mtq_seiren_driver)
{
  ObcTime current_obc_time = TMGR_get_master_clock();
  uint32_t step_time_ms = OBCT_diff_in_msec(&(mtq_seiren_driver->driver.pwm_info.last_set_time), &current_obc_time);
  mtq_seiren_driver->driver.pwm_info.last_set_time = current_obc_time;
  // TODO_L : step_time_msが必ず正であることを保証する。アプリの実行周期と違いすぎる場合のエラー処理を行う。

  mtq_seiren_driver->driver.pwm_info.internal_timer_ms += step_time_ms;

  if (mtq_seiren_driver->driver.pwm_info.internal_timer_ms >= MTQ_SEIREN_pwm_period_ms_)
  {
    mtq_seiren_driver->driver.pwm_info.internal_timer_ms = 0;
  }
}

/**
 * @brief  MTQ_SEIREN 出力on
 *
 *         MTQ_SEIRENの極性に応じて，GPIOピンの出力設定を行う
 * @param  void
 */
static GPIO_ERR_CODE MTQ_SEIREN_output_turn_on_(MTQ_SEIREN_Driver* mtq_seiren_driver)
{
  GPIO_ERR_CODE ret1, ret2;

  if (mtq_seiren_driver->driver.polarity == MTQ_SEIREN_POLARITY_POSITIVE)
  {
    ret1 = (GPIO_ERR_CODE)GPIO_set_output(mtq_seiren_driver->driver.ch_gpio_positive, GPIO_HIGH);
    ret2 = (GPIO_ERR_CODE)GPIO_set_output(mtq_seiren_driver->driver.ch_gpio_negative, GPIO_LOW);
    if (ret1 == GPIO_OK && ret2 == GPIO_OK)
    {
      return GPIO_OK;
    }
    else if (ret1 != GPIO_OK)
    {
      return ret1;
    }
    else
    {
      return ret2;
    }
  }
  else if (mtq_seiren_driver->driver.polarity == MTQ_SEIREN_POLARITY_NEGATIVE)
  {
    ret1 = (GPIO_ERR_CODE)GPIO_set_output(mtq_seiren_driver->driver.ch_gpio_positive, GPIO_LOW);
    ret2 = (GPIO_ERR_CODE)GPIO_set_output(mtq_seiren_driver->driver.ch_gpio_negative, GPIO_HIGH);
    if (ret1 == GPIO_OK && ret2 == GPIO_OK)
    {
      return GPIO_OK;
    }
    else if (ret1 != GPIO_OK)
    {
      return ret1;
    }
    else
    {
      return ret2;
    }
  }
  else
  {
    ret1 = (GPIO_ERR_CODE)GPIO_set_output(mtq_seiren_driver->driver.ch_gpio_positive, GPIO_LOW);
    ret2 = (GPIO_ERR_CODE)GPIO_set_output(mtq_seiren_driver->driver.ch_gpio_negative, GPIO_LOW);
    if (ret1 == GPIO_OK && ret2 == GPIO_OK)
    {
      return GPIO_OK;
    }
    else if (ret1 != GPIO_OK)
    {
      return ret1;
    }
    else
    {
      return ret2;
    }
  }
}

/**
 * @brief  MTQ_SEIREN 出力off
 *
 *         MTQ_SEIRENの極性によらず，GPIOの出力をLOWにする
 * @param  void
 */
static GPIO_ERR_CODE MTQ_SEIREN_output_turn_off_(MTQ_SEIREN_Driver* mtq_seiren_driver)
{
  GPIO_ERR_CODE ret1, ret2;
  ret1 = (GPIO_ERR_CODE)GPIO_set_output(mtq_seiren_driver->driver.ch_gpio_positive, GPIO_LOW);
  ret2 = (GPIO_ERR_CODE)GPIO_set_output(mtq_seiren_driver->driver.ch_gpio_negative, GPIO_LOW);

  // とにかくpositiveピンとnegativeピンをLOWにさせたいので，negativeのLOW化でエラーが出ていても，positiveのLOW化はトライする．
  if (ret1 == GPIO_OK && ret2 == GPIO_OK)
  {
    return GPIO_OK;
  }
  else if (ret1 != GPIO_OK)
  {
    return ret1;
  }
  else
  {
    return ret2;
  }
}

#pragma section
