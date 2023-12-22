#pragma section REPRO
/**
* @file   mtq_seiren.c
* @brief  MTQ_SEIRENのDriver
*/

#include "mtq_seiren.h"

#include <src_user/Library/vector3.h>

GPIO_ERR_CODE MTQ_SEIREN_init(MTQ_SEIREN_Driver* mtq_seiren_driver, uint8_t ch_gpio_positive, uint8_t ch_gpio_negative, float max_mag_moment)
{
  GPIO_ERR_CODE ret;

  mtq_seiren_driver->driver.ch_gpio_positive = ch_gpio_positive;
  mtq_seiren_driver->driver.ch_gpio_negative = ch_gpio_negative;

  // この2行でOUTPUTにできなかった場合、output_turn_offの内部でエラーが出るので、ここではアサーションが必要ない
  GPIO_set_direction(ch_gpio_positive, GPIO_OUTPUT);
  GPIO_set_direction(ch_gpio_negative, GPIO_OUTPUT);

  ret = MTQ_SEIREN_output(mtq_seiren_driver, MTQ_SEIREN_NO_OUTPUT);

  mtq_seiren_driver->driver.max_mag_moment = max_mag_moment;

  mtq_seiren_driver->info.magnetic_moment_direction_b[0] = 1.0f;
  mtq_seiren_driver->info.magnetic_moment_direction_b[1] = 0.0f;
  mtq_seiren_driver->info.magnetic_moment_direction_b[2] = 0.0f;

  return ret;
}


GPIO_ERR_CODE MTQ_SEIREN_output(const MTQ_SEIREN_Driver* mtq_seiren_driver, MTQ_SEIREN_POLARITY polarity)
{
  GPIO_ERR_CODE ret1, ret2;

  if (polarity == MTQ_SEIREN_POLARITY_POSITIVE)
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
  else if (polarity == MTQ_SEIREN_POLARITY_NEGATIVE)
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
  else if (polarity == MTQ_SEIREN_NO_OUTPUT)
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
  else
  { // NOT REACHED
    return DS_DRIVER_ERR_CODE_ILLEGAL_PARAMETER;
  }
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

#pragma section
