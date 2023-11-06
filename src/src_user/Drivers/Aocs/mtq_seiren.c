#pragma section REPRO
/**
* @file   mtq_seiren.c
* @brief  MTQ_SEIRENのDriver
*/

#include "mtq_seiren.h"

#include  <src_core/System/TimeManager/time_manager.h>
#include <src_user/Library/vector3.h>

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

  mtq_seiren_driver->driver.polarity                       = MTQ_SEIREN_NO_OUTPUT;
  mtq_seiren_driver->driver.max_mag_moment                 = max_mag_moment;

  mtq_seiren_driver->info.magnetic_moment_direction_b[0] = 1.0f;
  mtq_seiren_driver->info.magnetic_moment_direction_b[1] = 0.0f;
  mtq_seiren_driver->info.magnetic_moment_direction_b[2] = 0.0f;

  return ret;
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
