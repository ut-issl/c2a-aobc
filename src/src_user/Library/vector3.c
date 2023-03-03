#pragma section REPRO
/**
 * @file
 * @brief  ベクトル計算用の関数群の定義
 */

#include "vector3.h"

#include <math.h>

#include "math_constants.h"

void VECTOR3_initialize(float out[PHYSICAL_CONST_THREE_DIM],
                  const float value)
{
  int i;
  for (i = 0; i < PHYSICAL_CONST_THREE_DIM; i++)
  {
    out[i] = value;
  }
}

void VECTOR3_initialize_double(double out[PHYSICAL_CONST_THREE_DIM],
                         const double value)
{
  int i;
  for (i = 0; i < PHYSICAL_CONST_THREE_DIM; i++)
  {
    out[i] = value;
  }
}

float VECTOR3_inner_product(const float in_a[PHYSICAL_CONST_THREE_DIM],
                            const float in_b[PHYSICAL_CONST_THREE_DIM])
{
  int i;
  float ans = 0.0f;

  for (i = 0; i < PHYSICAL_CONST_THREE_DIM; i++)
  {
    ans += in_a[i] * in_b[i];
  }

  return ans;
}

void VECTOR3_outer_product(float out[PHYSICAL_CONST_THREE_DIM],
                     const float lhs[PHYSICAL_CONST_THREE_DIM],
                     const float rhs[PHYSICAL_CONST_THREE_DIM])
{
  // lhsやrhsとoutとが同じときlhsやrhsが書き換えられるのを防ぐ
  float temp[PHYSICAL_CONST_THREE_DIM];

  temp[0] = lhs[1] * rhs[2] - lhs[2] * rhs[1];
  temp[1] = lhs[2] * rhs[0] - lhs[0] * rhs[2];
  temp[2] = lhs[0] * rhs[1] - lhs[1] * rhs[0];
  VECTOR3_copy(out, temp);
}

void VECTOR3_outer_product_double(double out[PHYSICAL_CONST_THREE_DIM],
                            const double lhs[PHYSICAL_CONST_THREE_DIM],
                            const double rhs[PHYSICAL_CONST_THREE_DIM])
{
  // lhsやrhsとoutとが同じときlhsやrhsが書き換えられるのを防ぐ
  double temp[PHYSICAL_CONST_THREE_DIM];

  temp[0] = lhs[1] * rhs[2] - lhs[2] * rhs[1];
  temp[1] = lhs[2] * rhs[0] - lhs[0] * rhs[2];
  temp[2] = lhs[0] * rhs[1] - lhs[1] * rhs[0];
  VECTOR3_copy_double(out, temp);
}

void VECTOR3_hadamard_product(float out[PHYSICAL_CONST_THREE_DIM],
                              const float lhs[PHYSICAL_CONST_THREE_DIM],
                              const float rhs[PHYSICAL_CONST_THREE_DIM])
{
  int i;

  for ( i = 0; i < PHYSICAL_CONST_THREE_DIM; i++)
  {
    out[i] = lhs[i] * rhs[i];
  }
}

void VECTOR3_scalar_product(float out[PHYSICAL_CONST_THREE_DIM],
                      const float scalar,
                      const float in[PHYSICAL_CONST_THREE_DIM])
{
  int i;

  for (i = 0; i < PHYSICAL_CONST_THREE_DIM; i++)
  {
    out[i] = scalar * in[i];
  }
}

void VECTOR3_add(float out[PHYSICAL_CONST_THREE_DIM],
           const float in_a[PHYSICAL_CONST_THREE_DIM],
           const float in_b[PHYSICAL_CONST_THREE_DIM])
{
  int i;

  for (i = 0; i < PHYSICAL_CONST_THREE_DIM; i++)
  {
    out[i] = in_a[i] + in_b[i];
  }
}

void VECTOR3_add_double(double out[PHYSICAL_CONST_THREE_DIM],
                  const double in_a[PHYSICAL_CONST_THREE_DIM],
                  const double in_b[PHYSICAL_CONST_THREE_DIM])
{
  int i;

  for (i = 0; i < PHYSICAL_CONST_THREE_DIM; i++)
  {
    out[i] = in_a[i] + in_b[i];
  }
}

void VECTOR3_subtract(float out[PHYSICAL_CONST_THREE_DIM],
                const float lhs[PHYSICAL_CONST_THREE_DIM],
                const float rhs[PHYSICAL_CONST_THREE_DIM])
{
  int i;

  for (i = 0; i < PHYSICAL_CONST_THREE_DIM; i++)
  {
    out[i] = lhs[i] - rhs[i];
  }
}

void VECTOR3_subtract_double(double out[PHYSICAL_CONST_THREE_DIM],
                       const double lhs[PHYSICAL_CONST_THREE_DIM],
                       const double rhs[PHYSICAL_CONST_THREE_DIM])
{
  int i;

  for (i = 0; i < PHYSICAL_CONST_THREE_DIM; i++)
  {
    out[i] = lhs[i] - rhs[i];
  }
}

C2A_MATH_ERROR VECTOR3_time_derivative(float out[PHYSICAL_CONST_THREE_DIM],
                                       const float after[PHYSICAL_CONST_THREE_DIM],
                                       const float before[PHYSICAL_CONST_THREE_DIM],
                                       const float t_after,
                                       const float t_before)
{
  float diff[PHYSICAL_CONST_THREE_DIM];
  float t_diff;

  VECTOR3_subtract(diff, after, before);
  t_diff = t_after - t_before;

  if (t_diff < 0)
  {
    return C2A_MATH_ERROR_RANGE_OVER;
  }
  else if (t_diff < MATH_CONST_PROHIBIT_DIVISION_VALUE)
  {
    return C2A_MATH_ERROR_SINGULAR;
  }

  VECTOR3_scalar_product(out, 1.0f / t_diff, diff);

  return C2A_MATH_ERROR_OK;
}

void VECTOR3_copy(float out[PHYSICAL_CONST_THREE_DIM],
            const float in[PHYSICAL_CONST_THREE_DIM])
{
  int i;

  for (i = 0; i < PHYSICAL_CONST_THREE_DIM; i++)
  {
    out[i] = in[i];
  }
}

void VECTOR3_copy_double(double out[PHYSICAL_CONST_THREE_DIM],
                   const double in [PHYSICAL_CONST_THREE_DIM])
{
  int i;

  for (i = 0; i < PHYSICAL_CONST_THREE_DIM; i++)
  {
    out[i] = in[i];
  }
}

float VECTOR3_norm(const float vec[PHYSICAL_CONST_THREE_DIM])
{
  return sqrtf(VECTOR3_inner_product(vec, vec));
}

C2A_MATH_ERROR VECTOR3_normalize(float out[PHYSICAL_CONST_THREE_DIM],
                           const float in[PHYSICAL_CONST_THREE_DIM])
{
  C2A_MATH_ERROR  error_check = VECTOR3_is_normalized(in);
  if (error_check == C2A_MATH_ERROR_SINGULAR )
  {
    VECTOR3_copy(out, in);
    return error_check;
  }

  // for numerical precision, normalize input even if is_normalized says already normalized
  float inverse_norm = 1.0f / VECTOR3_norm(in);
  VECTOR3_scalar_product(out, inverse_norm, in);
  return C2A_MATH_ERROR_OK;
}

C2A_MATH_ERROR VECTOR3_is_normalized(const float in[PHYSICAL_CONST_THREE_DIM])
{
  float in_norm = VECTOR3_norm(in);

  C2A_MATH_ERROR is_norm_zero =
  C2A_MATH_check_abs_range_violation(in_norm, MATH_CONST_PROHIBIT_DIVISION_VALUE);
  if (is_norm_zero == C2A_MATH_ERROR_OK) return C2A_MATH_ERROR_SINGULAR; // input vector cannot be normalized

  C2A_MATH_ERROR is_normalize =
  C2A_MATH_check_abs_range_violation((1.0f - in_norm), MATH_CONST_NORMAL_CHECK_THRESHOLD);
  if (is_normalize != C2A_MATH_ERROR_OK) return C2A_MATH_ERROR_UNNORMALIZED;

  return C2A_MATH_ERROR_OK;
}

void VECTOR3_tensor_product(float matrix[][PHYSICAL_CONST_THREE_DIM],
                      const float v_in1[PHYSICAL_CONST_THREE_DIM],
                      const float v_in2[PHYSICAL_CONST_THREE_DIM])
{
  for (int i = 0; i < PHYSICAL_CONST_THREE_DIM; i++)
  {
    for (int j = 0; j < PHYSICAL_CONST_THREE_DIM; j++)
    {
      matrix[i][j] = v_in1[i] * v_in2[j];
    }
  }
}

#pragma section
