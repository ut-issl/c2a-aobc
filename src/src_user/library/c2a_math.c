#pragma section REPRO
/**
* @file   c2a_math.c
* @brief  各種数学処理用関数群の定義
* @note   基本，ここで提供するのはスカラー処理とし，ベクトル等の配列への対応はvector3.h/.c等に実装
* @note   基本的には入力はfloat想定．double入力の場合は関数末尾に_doubleとついているものを使用．
*/

#include "c2a_math.h"

#include <math.h>
#include "math_constants.h"

// 特定値域へのクリップ，NaN判定などのユーティリティ数値操作系処理

C2A_MATH_ERROR C2A_MATH_check_nan_inf(const float input)
{
  if (input != input)     return C2A_MATH_ERROR_NAN;
  if (isinf(input) != 0)  return C2A_MATH_ERROR_NAN;

  return C2A_MATH_ERROR_OK;
}


C2A_MATH_ERROR C2A_MATH_check_nan_inf_double(const double input)
{
  if (input != input)     return C2A_MATH_ERROR_NAN;
  if (isinf(input) != 0)  return C2A_MATH_ERROR_NAN;

  return C2A_MATH_ERROR_OK;
}


float C2A_MATH_clip_value(const float input, const float upper_limit, const float lower_limit)
{
  if (input > upper_limit) return upper_limit;
  if (input < lower_limit) return lower_limit;

  return input;
}


float C2A_MATH_clip_value_abs(const float input, const float abs_limit)
{
  float upper_limit =   abs_limit;
  float lower_limit = - abs_limit;

  return C2A_MATH_clip_value(input, upper_limit, lower_limit);
}


float C2A_MATH_clip_value_reject_nan(const float input, const float upper_limit,
                                     const float lower_limit, const float substitutional_ans)
{
  if (C2A_MATH_check_nan_inf(input) != C2A_MATH_ERROR_OK) return substitutional_ans;

  if (input > upper_limit) return upper_limit;
  if (input < lower_limit) return lower_limit;

  return input;
}


double C2A_MATH_clip_value_double(const double input, const double upper_limit,
                                  const double lower_limit, const double substitutional_ans)
{
  if (C2A_MATH_check_nan_inf_double(input) != C2A_MATH_ERROR_OK) return substitutional_ans;

  if (input > upper_limit) return upper_limit;
  if (input < lower_limit) return lower_limit;

  return input;
}


float C2A_MATH_normalize_zero_2pi(const float input_angle_rad)
{
  float output_angle_rad = input_angle_rad;

  if ((output_angle_rad > MATH_CONST_2PI) || (output_angle_rad < - MATH_CONST_2PI))
  {
    output_angle_rad = output_angle_rad -
                       floorf(output_angle_rad / MATH_CONST_2PI) * MATH_CONST_2PI;
  }

  if (output_angle_rad < 0.0f)
  {
    output_angle_rad = output_angle_rad + MATH_CONST_2PI;
  }

  return output_angle_rad;
}

double C2A_MATH_normalize_zero_2pi_double(const double input_angle_rad)
{
  double output_angle_rad = input_angle_rad;

  if ((output_angle_rad > MATH_CONST_2PI_DOUBLE) || (output_angle_rad < - MATH_CONST_2PI_DOUBLE))
  {
    output_angle_rad = output_angle_rad -
                       floor(output_angle_rad / MATH_CONST_2PI_DOUBLE) * MATH_CONST_2PI_DOUBLE;
  }

  if (output_angle_rad < 0.0)
  {
    output_angle_rad = output_angle_rad + MATH_CONST_2PI_DOUBLE;
  }

  return output_angle_rad;
}


C2A_MATH_ERROR C2A_MATH_check_range_violation(const float input,
                                              const float upper_limit,
                                              const float lower_limit)
{
  if (input > upper_limit) return C2A_MATH_ERROR_RANGE_OVER_HIGH;
  if (input < lower_limit) return C2A_MATH_ERROR_RANGE_OVER_LOW;

  return C2A_MATH_ERROR_OK;
}


C2A_MATH_ERROR C2A_MATH_check_abs_range_violation(const float input, const float abs_limit)
{
  if (C2A_MATH_clip_value_abs(input, abs_limit + MATH_CONST_MACHINE_EPSILON) != input)
  {
    return C2A_MATH_ERROR_RANGE_OVER;
  }

  return C2A_MATH_ERROR_OK;
}

C2A_MATH_ERROR C2A_MATH_calculate_time_derivative(float out,
                                                  const float after,
                                                  const float before,
                                                  const float t_after,
                                                  const float t_before)
{
  float diff = after - before;
  float t_diff = t_after - t_before;

  if (t_diff < 0.0f)
  {
    return C2A_MATH_ERROR_RANGE_OVER;
  }
  else if (t_diff < MATH_CONST_PROHIBIT_DIVISION_VALUE)
  {
    return C2A_MATH_ERROR_SINGULAR;
  }

  out = diff / t_diff;

  return C2A_MATH_ERROR_OK;
}

// math.hの拡張関数

float C2A_MATH_acos_rad(const float input_cos)
{
  const float kCosLimitRange =  1.0f; //!< limit of absolute value of cosine

  float limited_input = C2A_MATH_clip_value_abs(input_cos, kCosLimitRange);

  return acosf(limited_input);
}

float C2A_MATH_sqrtf(const float input)
{
  if (input <= 0.0f)
  {
    return 0.0f;
  }
  return sqrtf(input);
}

#pragma section
