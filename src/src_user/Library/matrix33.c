#pragma section REPRO
/**
 * @file
 * @brief  行列計算用の関数群の定義
 */

#include "matrix33.h"

#include <math.h>

#include "vector3.h"
#include "math_constants.h"

void MATRIX33_add(float out [PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM],
            const float in_a[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM],
            const float in_b[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM])
{
  int i;

  for (i = 0; i < PHYSICAL_CONST_THREE_DIM; i++)
  {
    VECTOR3_add(out[i], in_a[i], in_b[i]);
  }
}

void MATRIX33_subtract(float out[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM],
                 const float lhs[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM],
                 const float rhs[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM])
{
  int i;

  for (i = 0; i < PHYSICAL_CONST_THREE_DIM; i++)
  {
    VECTOR3_subtract(out[i], lhs[i], rhs[i]);
  }
}

void MATRIX33_multiply_scalar_matrix(float out[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM],
                               const float scalar,
                               const float in[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM])
{
  int row, column;

  for (row = 0; row < PHYSICAL_CONST_THREE_DIM; row++)
  {
    for (column = 0; column < PHYSICAL_CONST_THREE_DIM; column++)
    {
      out[row][column] = scalar * in[row][column];
    }
  }
}

void MATRIX33_multiply_matrix_vector(float vec_out[PHYSICAL_CONST_THREE_DIM],
                               const float mat[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM],
                               const float vec_in[PHYSICAL_CONST_THREE_DIM])
{
  int i, j;
  // *vec_inと*vec_outが同じときvec_inが書き換えられるのを防ぐ
  float vec_temp[PHYSICAL_CONST_THREE_DIM];

  for (i = 0; i < PHYSICAL_CONST_THREE_DIM; i++)
  {
    vec_temp[i] = 0.0f;
    for (j = 0; j < PHYSICAL_CONST_THREE_DIM; j++)
    {
      vec_temp[i] += mat[i][j] * vec_in[j];
    }
  }
  for (i = 0; i < PHYSICAL_CONST_THREE_DIM; i++)
  {
    vec_out[i] = vec_temp[i];
  }
}

void MATRIX33_multiply_matrix_vector_double(double vec_out[PHYSICAL_CONST_THREE_DIM],
                                      const double mat[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM],
                                      const double vec_in[PHYSICAL_CONST_THREE_DIM])
{
  int i, j;
  // *vec_inと*vec_outが同じときvec_inが書き換えられるのを防ぐ
  double vec_temp[PHYSICAL_CONST_THREE_DIM];

  for (i = 0; i < PHYSICAL_CONST_THREE_DIM; i++)
  {
    vec_temp[i] = 0.0f;
    for (j = 0; j < PHYSICAL_CONST_THREE_DIM; j++)
    {
      vec_temp[i] += mat[i][j] * vec_in[j];
    }
  }
  for (i = 0; i < PHYSICAL_CONST_THREE_DIM; i++)
  {
    vec_out[i] = vec_temp[i];
  }
}

float MATRIX33_multiply_matrix_vector_quadratic(const float mat[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM],
                                                const float vec[PHYSICAL_CONST_THREE_DIM])
{
  // b^t * A * b = (A^T * b)^T ・ bとして計算する
  float mat_transpose[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM]; //!< 行列Aの転置
  MATRIX33_transpose(mat_transpose, mat);
  float mat_transpose_vec[PHYSICAL_CONST_THREE_DIM]; //!< (A^T * b)
  MATRIX33_multiply_matrix_vector(mat_transpose_vec, mat_transpose, vec);

  float out = VECTOR3_inner_product(mat_transpose_vec, vec);
  return out;
}

void MATRIX33_multiply_matrix_matrix(float out[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM],
                               const float lhs[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM],
                               const float rhs[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM])
{
  int row, column, i;
  // lhsやrhsとoutとが同じときlhsやrhsが書き換えられるのを防ぐ
  float matrix_temp[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM];

  for (row = 0; row < PHYSICAL_CONST_THREE_DIM; row++)
  {
    for (column = 0; column < PHYSICAL_CONST_THREE_DIM; column++)
    {
      matrix_temp[row][column] = 0.0f;
      for (i = 0; i < PHYSICAL_CONST_THREE_DIM; i++)
      {
        matrix_temp[row][column] += lhs[row][i] * rhs[i][column];
      }
    }
  }
  for (row = 0; row < PHYSICAL_CONST_THREE_DIM; row++)
  {
    for (column = 0; column < PHYSICAL_CONST_THREE_DIM; column++)
    {
      out[row][column] = matrix_temp[row][column];
    }
  }
}

void MATRIX33_multiply_matrix_matrix_double(double out[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM],
                                      const double lhs[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM],
                                      const double rhs[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM])
{
  int row, column, i;
  // lhsやrhsとoutとが同じときlhsやrhsが書き換えられるのを防ぐ
  double matrix_temp[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM];

  for (row = 0; row < PHYSICAL_CONST_THREE_DIM; row++)
  {
    for (column = 0; column < PHYSICAL_CONST_THREE_DIM; column++)
    {
      matrix_temp[row][column] = 0.0f;
      for (i = 0; i < PHYSICAL_CONST_THREE_DIM; i++)
      {
        matrix_temp[row][column] += lhs[row][i] * rhs[i][column];
      }
    }
  }
  for (row = 0; row < PHYSICAL_CONST_THREE_DIM; row++)
  {
    for (column = 0; column < PHYSICAL_CONST_THREE_DIM; column++)
    {
      out[row][column] = matrix_temp[row][column];
    }
  }
}

float MATRIX33_trace(const float mat[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM])
{
  int i;
  float trace = 0.0f;

  for (i = 0; i < PHYSICAL_CONST_THREE_DIM; i++)
  {
    trace += mat[i][i];
  }

  return trace;
}

void  MATRIX33_transpose(float out[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM],
                   const float in[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM])
{
  // inとoutとが同じときinが書き換えられるのを防ぐ
  float temp[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM];

  for (int i = 0; i < PHYSICAL_CONST_THREE_DIM; i++)
  {
    for (int j = 0; j < PHYSICAL_CONST_THREE_DIM; j++)
    {
      temp[i][j] = in[j][i];
    }
  }
  MATRIX33_copy(out, temp);
}

void  MATRIX33_transpose_double(double out[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM],
                          const double in[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM])
{
  // inとoutとが同じときinが書き換えられるのを防ぐ
  double temp[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM];

  for (int i = 0; i < PHYSICAL_CONST_THREE_DIM; i++)
  {
    for (int j = 0; j < PHYSICAL_CONST_THREE_DIM; j++)
    {
      temp[i][j] = in[j][i];
    }
  }
  MATRIX33_copy_double(out, temp);
}

float MATRIX33_determinant(const float mat[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM])
{
  float det = mat[0][0] * mat[1][1] * mat[2][2] +
              mat[1][0] * mat[2][1] * mat[0][2] +
              mat[2][0] * mat[0][1] * mat[1][2] -
              mat[0][0] * mat[2][1] * mat[1][2] -
              mat[2][0] * mat[1][1] * mat[0][2] -
              mat[1][0] * mat[0][1] * mat[2][2];

  return det;
}

void MATRIX33_adjugate(float out[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM],
                 const float in[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM])
{
  // inとoutとが同じときinが書き換えられるのを防ぐ
  float temp[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM];

  temp[0][0] = (in[1][1] * in[2][2] - in[1][2] * in[2][1]);
  temp[0][1] = (in[0][2] * in[2][1] - in[0][1] * in[2][2]);
  temp[0][2] = (in[0][1] * in[1][2] - in[0][2] * in[1][1]);

  temp[1][0] = (in[1][2] * in[2][0] - in[1][0] * in[2][2]);
  temp[1][1] = (in[0][0] * in[2][2] - in[0][2] * in[2][0]);
  temp[1][2] = (in[0][2] * in[1][0] - in[0][0] * in[1][2]);

  temp[2][0] = (in[1][0] * in[2][1] - in[1][1] * in[2][0]);
  temp[2][1] = (in[0][1] * in[2][0] - in[0][0] * in[2][1]);
  temp[2][2] = (in[0][0] * in[1][1] - in[0][1] * in[1][0]);

  MATRIX33_copy(out, temp);
}

C2A_MATH_ERROR MATRIX33_inverse(float inv[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM],
                          const float mat[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM])
{
  float det = MATRIX33_determinant(mat);

  if (fabsf(det) < MATH_CONST_PROHIBIT_DIVISION_VALUE)
  {
    MATRIX33_adjugate(inv, mat);

    return C2A_MATH_ERROR_SINGULAR;
  }
  else
  {
    float adj[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM]; //!< 余因子行列
    MATRIX33_adjugate(adj, mat);
    MATRIX33_multiply_scalar_matrix(inv, 1.0f / det, adj);

    return C2A_MATH_ERROR_OK;
  }
}

void MATRIX33_make_diagonal(float matrix[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM],
                    const float seed_vector[PHYSICAL_CONST_THREE_DIM])
{
  for (int i = 0; i < PHYSICAL_CONST_THREE_DIM; i++)
  {
    for (int j = 0; j < PHYSICAL_CONST_THREE_DIM; j++)
    {
      matrix[i][j] = 0.0f;
    }
    matrix[i][i] = seed_vector[i];
  }
}

void MATRIX33_make_diagonal_double(double matrix[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM],
                             const double seed_vector[PHYSICAL_CONST_THREE_DIM])
{
  for (int i = 0; i < PHYSICAL_CONST_THREE_DIM; i++)
  {
    for (int j = 0; j < PHYSICAL_CONST_THREE_DIM; j++)
    {
      matrix[i][j] = 0.0f;
    }
    matrix[i][i] = seed_vector[i];
  }
}

void MATRIX33_make_unit(float matrix[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM])
{
  const float kOnesVector[] = { 1.0f, 1.0f, 1.0f };
  MATRIX33_make_diagonal(matrix, kOnesVector);
}

void MATRIX33_make_unit_double(double matrix[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM])
{
  const double kOnesVector[] = { 1.0, 1.0, 1.0 };
  MATRIX33_make_diagonal_double(matrix, kOnesVector);
}

void MATRIX33_make_skew(float matrix[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM],
                  const float seed_vector[PHYSICAL_CONST_THREE_DIM])
{
  matrix[0][1] = - seed_vector[2];
  matrix[0][2] =   seed_vector[1];
  matrix[1][2] = - seed_vector[0];

  matrix[1][0] = - matrix[0][1];
  matrix[2][0] = - matrix[0][2];
  matrix[2][1] = - matrix[1][2];

  for (int i = 0; i < PHYSICAL_CONST_THREE_DIM; i++)
  {
    matrix[i][i] = 0.0f;
  }
}

void MATRIX33_make_rotation(float mat[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM],
                            const float theta, const MATRIX33_ROTATION_AXIS axis)
{
  MATRIX33_make_unit(mat);
  if (axis == MATRIX33_ROTATION_AXIS_X)
  {
    mat[1][1] =   cosf(theta);
    mat[1][2] =   sinf(theta);
    mat[2][1] = - sinf(theta);
    mat[2][2] =   cosf(theta);
  }
  else if (axis == MATRIX33_ROTATION_AXIS_Y)
  {
    mat[0][0] =   cosf(theta);
    mat[0][2] = - sinf(theta);
    mat[2][0] =   sinf(theta);
    mat[2][2] =   cosf(theta);
  }
  else if (axis == MATRIX33_ROTATION_AXIS_Z)
  {
    mat[0][0] =   cosf(theta);
    mat[0][1] =   sinf(theta);
    mat[1][0] = - sinf(theta);
    mat[1][1] =   cosf(theta);
  }
}

void MATRIX33_make_rotation_double(double mat[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM],
                             const double theta, const MATRIX33_ROTATION_AXIS axis)
{
  MATRIX33_make_unit_double(mat);
  if (axis == MATRIX33_ROTATION_AXIS_X)
  {
    mat[1][1] =   cos(theta);
    mat[1][2] =   sin(theta);
    mat[2][1] = - sin(theta);
    mat[2][2] =   cos(theta);
  }
  else if (axis == MATRIX33_ROTATION_AXIS_Y)
  {
    mat[0][0] =   cos(theta);
    mat[0][2] = - sin(theta);
    mat[2][0] =   sin(theta);
    mat[2][2] =   cos(theta);
  }
  else if (axis == MATRIX33_ROTATION_AXIS_Z)
  {
    mat[0][0] =   cos(theta);
    mat[0][1] =   sin(theta);
    mat[1][0] = - sin(theta);
    mat[1][1] =   cos(theta);
  }
}

C2A_MATH_ERROR MATRIX33_make_dcm_from_two_vectors(float dcm_out[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM],
                                                  float first_vec_in[PHYSICAL_CONST_THREE_DIM],
                                                  float second_vec_in[PHYSICAL_CONST_THREE_DIM])
{
  float third_vec[PHYSICAL_CONST_THREE_DIM];
  float third_vec_norm;

  VECTOR3_normalize(first_vec_in, first_vec_in);
  VECTOR3_normalize(second_vec_in, second_vec_in);

  VECTOR3_outer_product(third_vec, first_vec_in, second_vec_in);
  // first_vecとsecond_vecが平行（third_vecのノルムが0）のとき，エラーを返す．
  third_vec_norm = VECTOR3_norm(third_vec);
  if (third_vec_norm < MATH_CONST_NORMAL_CHECK_THRESHOLD)
  {
    return C2A_MATH_ERROR_SINGULAR;
  }
  VECTOR3_normalize(third_vec, third_vec);

  VECTOR3_outer_product(second_vec_in, third_vec, first_vec_in);
  VECTOR3_normalize(second_vec_in, second_vec_in);

  for (int i = 0; i < PHYSICAL_CONST_THREE_DIM; i++)
  {
    dcm_out[i][0] = first_vec_in[i];
    dcm_out[i][1] = second_vec_in[i];
    dcm_out[i][2] = third_vec[i];
  }

  return C2A_MATH_ERROR_OK;
}

void MATRIX33_copy(float matrix_out[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM],
             const float matrix_in [PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM])
{
  for (int i = 0; i < PHYSICAL_CONST_THREE_DIM; i++)
  {
    VECTOR3_copy(matrix_out[i], matrix_in[i]);
  }
}

void MATRIX33_copy_double(double matrix_out[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM],
                    const double matrix_in [PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM])
{
  for (int i = 0; i < PHYSICAL_CONST_THREE_DIM; i++)
  {
    VECTOR3_copy_double(matrix_out[i], matrix_in[i]);
  }
}

C2A_MATH_ERROR MATRIX33_is_normalized(const float in[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM])
{
  float matrix_det = MATRIX33_determinant(in);

  C2A_MATH_ERROR equal_zero =
  C2A_MATH_check_abs_range_violation(matrix_det, MATH_CONST_PROHIBIT_DIVISION_VALUE);
  if (equal_zero == C2A_MATH_ERROR_OK)  // here, C2A_MATH_ERROR_OK means matrix_det is almost 0
  {
    return C2A_MATH_ERROR_SINGULAR;
  }

  C2A_MATH_ERROR check_normalized =
  C2A_MATH_check_abs_range_violation((1.0f - matrix_det), MATH_CONST_NORMAL_CHECK_THRESHOLD);
  if (check_normalized != C2A_MATH_ERROR_OK)
  {
    return C2A_MATH_ERROR_UNNORMALIZED;
  }

  return C2A_MATH_ERROR_OK;
}

C2A_MATH_ERROR MATRIX33_normalize(float matrix_out[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM],
                            const float matrix_in [PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM])
{
  C2A_MATH_ERROR error_check_result = MATRIX33_is_normalized(matrix_in);
  if (error_check_result == C2A_MATH_ERROR_SINGULAR)
  {
    MATRIX33_make_unit(matrix_out);
    return C2A_MATH_ERROR_SINGULAR;
  }

  if (error_check_result == C2A_MATH_ERROR_OK)
  {
    MATRIX33_copy(matrix_out, matrix_in);
    return C2A_MATH_ERROR_OK;
  }

  // safety of devide operation with matrix_det is already guaranteed
  float matrix_det = fabs(MATRIX33_determinant(matrix_in));
  // since det(k*matrix) = k^n*det(matrix), scale factor should be pow(1/n) of det(matrix)
  float inverse_scale_factor = powf(matrix_det, (1.0f /(float)(PHYSICAL_CONST_THREE_DIM)));
  MATRIX33_multiply_scalar_matrix(matrix_out, (1.0f / inverse_scale_factor), matrix_in);
  return C2A_MATH_ERROR_OK;
}

C2A_MATH_ERROR MATRIX33_is_symmetry(const float in[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM])
{
  int i;
  int row   [PHYSICAL_CONST_THREE_DIM] = {0, 0, 1};
  int column[PHYSICAL_CONST_THREE_DIM] = {1, 2, 2};
  const float kSymmetryThreshold = 1e-3f;
  for (i = 0; i < PHYSICAL_CONST_THREE_DIM; i++)
  {
    float diff = in[row[i]][column[i]] -
                 in[column[i]][row[i]];
    if (fabsf(diff) >= kSymmetryThreshold)
    {
      return C2A_MATH_ERROR_OTHERS;
    }
  }

  return C2A_MATH_ERROR_OK;
}

#pragma section
