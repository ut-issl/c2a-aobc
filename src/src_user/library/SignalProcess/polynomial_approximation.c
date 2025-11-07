#pragma section REPRO
/**
* @file   polynomial_approximation.h
* @brief  多項式近似関連関数群ライブラリ
*/

#include "./polynomial_approximation.h"
#include <math.h>


int POLYNOMIAL_APPROX_initialize(PolynomialApprox* polynomial_approx,
                          const uint8_t num_coeff,
                          const float* coefficients,
                          const float range_low,
                          const float range_high)
{
  if (num_coeff >= POLYNOMIAL_APPROX_MAX_COEFF_NUM) return -1;
  polynomial_approx->num_coeff_ = num_coeff;

  if (range_low >= range_high) return -1;
  polynomial_approx->range_high_ = range_high;
  polynomial_approx->range_low_ = range_low;

  for (int i = 0; i < polynomial_approx->num_coeff_; i++)
  {
    polynomial_approx->coefficients_[i] = coefficients[i];
  }

  return 0;
}

int POLYNOMIAL_APPROX_calc_output(float* output, const PolynomialApprox polynomial_approx, const float input)
{
  if (input < polynomial_approx.range_low_)  return -1;
  if (input > polynomial_approx.range_high_) return -1;

  *output = 0.0f;
  float factorial_input = 1.0f;
  for (uint8_t i = 0; i < polynomial_approx.num_coeff_; i++)
  {
    *output += polynomial_approx.coefficients_[i] * factorial_input;
    factorial_input *= input;
  }

  return 0;
}
