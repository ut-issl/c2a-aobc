#pragma section REPRO
/**
* @file   quaternion.c
* @brief  quaternion関連の関数群の定義
* @note   全関数の末尾で規格化を行うため，ライブラリ内の演算結果は正規化が保たれる見込み
* @note   スカラー部の符号強制 (正に固定) は行わない (暫定仕様)
*/

#include "quaternion.h"

#include <math.h>

#include "math_constants.h"
#include "vector3.h"
#include "matrix33.h"

// QUATERNION生成関数

C2A_MATH_ERROR QUATERNION_make_from_array(Quaternion* q_out,
                                          const float array[PHYSICAL_CONST_QUATERNION_DIM],
                                          const QUATERNION_SCALAR_POSITION position)
{
  Quaternion q_out_before_check;
  C2A_MATH_ERROR position_err = C2A_MATH_ERROR_OK;

  position_err = QUATERNION_make_from_array_unnormalizing(&q_out_before_check, array, position);

  C2A_MATH_ERROR q_make_err = QUATERNION_normalize_with_error_check(q_out, q_out_before_check);
  if (q_make_err != C2A_MATH_ERROR_OK) return q_make_err;

  return position_err;
}

C2A_MATH_ERROR QUATERNION_make_from_array_unnormalizing(Quaternion* q_out,
                                                        const float array[PHYSICAL_CONST_QUATERNION_DIM],
                                                        const QUATERNION_SCALAR_POSITION position)
{
  C2A_MATH_ERROR position_err = C2A_MATH_ERROR_OK;
  QUATERNION_SCALAR_POSITION position_stored = position; //!< buffer to protect input
  if ((position != QUATERNION_SCALAR_POSITION_FIRST) &&
    (position != QUATERNION_SCALAR_POSITION_LAST))
  {
    position_stored = QUATERNION_SCALAR_POSITION_LAST;
    position_err = C2A_MATH_ERROR_ENUM_IRREGULAR;
  }

  int vector_part_begin = 0; //!< first position where vector part begins
  if (position_stored == QUATERNION_SCALAR_POSITION_FIRST) vector_part_begin = 1;

  q_out->scalar_part = array[position_stored];
  for (int i = vector_part_begin; i < (vector_part_begin + PHYSICAL_CONST_THREE_DIM); i++)
  {
    q_out->vector_part[i - vector_part_begin] = array[i];
  }

  return position_err;
}

C2A_MATH_ERROR QUATERNION_make_from_axis_angle(Quaternion* q_out,
                                               const float axis[PHYSICAL_CONST_THREE_DIM],
                                               const float angle_rad)
{
  float axis_normalized[PHYSICAL_CONST_THREE_DIM];

  C2A_MATH_ERROR is_normalized = VECTOR3_is_normalized(axis);
  if (is_normalized == C2A_MATH_ERROR_SINGULAR)
  {
    *q_out = QUATERNION_make_unit();
    return is_normalized;
  }

  // ignore warning since singular valune is already removed
  C2A_MATH_ERROR ignored_warning = VECTOR3_normalize(axis_normalized, axis);
  float angle_cliped = C2A_MATH_normalize_zero_2pi(angle_rad);

  VECTOR3_scalar_product(q_out->vector_part, sinf(0.5f * angle_cliped), axis_normalized);
  q_out->scalar_part = cosf(0.5f * angle_cliped);

  *q_out = QUATERNION_normalize(*q_out); // ignore assertion, since input is safe in this case

  return is_normalized;
}


C2A_MATH_ERROR QUATERNION_make_from_matrix(Quaternion* q_out,
                                           const float matrix[][PHYSICAL_CONST_THREE_DIM])
{
  float matrix_n[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM]; //!< normalized matrix

  C2A_MATH_ERROR normalize_check = MATRIX33_is_normalized(matrix);
  if (normalize_check == C2A_MATH_ERROR_SINGULAR)
  {
    *q_out = QUATERNION_make_unit();
    return normalize_check;
  }

  // ignore warning since singular valune is already removed
  C2A_MATH_ERROR ignored_warning = MATRIX33_normalize(matrix_n, matrix);
  Quaternion q_before_check;

  float trace = MATRIX33_trace(matrix_n);
  if (trace > 0.0f)
  {
    float q_scalar_seed = C2A_MATH_sqrtf(trace + 1.0f);
    q_before_check.scalar_part    = 0.5f * q_scalar_seed;
    q_before_check.vector_part[0] = (matrix_n[1][2] - matrix_n[2][1]) / (2.0f * q_scalar_seed);
    q_before_check.vector_part[1] = (matrix_n[2][0] - matrix_n[0][2]) / (2.0f * q_scalar_seed);
    q_before_check.vector_part[2] = (matrix_n[0][1] - matrix_n[1][0]) / (2.0f * q_scalar_seed);
  }
  else
  {
    float q_candidate[PHYSICAL_CONST_QUATERNION_DIM];
    q_candidate[0] = 0.5f * C2A_MATH_sqrtf(1.0f + matrix_n[0][0] - matrix_n[1][1] - matrix_n[2][2]);
    q_candidate[1] = 0.5f * C2A_MATH_sqrtf(1.0f - matrix_n[0][0] + matrix_n[1][1] - matrix_n[2][2]);
    q_candidate[2] = 0.5f * C2A_MATH_sqrtf(1.0f - matrix_n[0][0] - matrix_n[1][1] + matrix_n[2][2]);
    q_candidate[3] = 0.5f * C2A_MATH_sqrtf(1.0f + matrix_n[0][0] + matrix_n[1][1] + matrix_n[2][2]);

    uint8_t max_value_index = 0;
    float max_value_in_candidate = fabsf(q_candidate[0]);
    for (uint8_t i = 1; i < PHYSICAL_CONST_QUATERNION_DIM; i++)
    {
      if (fabsf(q_candidate[i]) > max_value_in_candidate)
      {
        max_value_index = i;
        max_value_in_candidate = fabsf(q_candidate[i]);
      }
    }

    if (max_value_in_candidate < MATH_CONST_PROHIBIT_DIVISION_VALUE)
    {
      // in this case, input is not a DCM
      *q_out = QUATERNION_make_unit();
      return C2A_MATH_ERROR_SINGULAR;
    }

    // zero devide is already removed
    float denominator = 4.0f * q_candidate[max_value_index];

    switch (max_value_index)
    {
    case 0:
      q_candidate[1] = (matrix_n[0][1] + matrix_n[1][0]) / denominator;
      q_candidate[2] = (matrix_n[0][2] + matrix_n[2][0]) / denominator;
      q_candidate[3] = (matrix_n[1][2] - matrix_n[2][1]) / denominator;
      break;
    case 1:
      q_candidate[0] = (matrix_n[0][1] + matrix_n[1][0]) / denominator;
      q_candidate[2] = (matrix_n[2][1] + matrix_n[1][2]) / denominator;
      q_candidate[3] = (matrix_n[2][0] - matrix_n[0][2]) / denominator;
      break;
    case 2:
      q_candidate[0] = (matrix_n[2][0] + matrix_n[0][2]) / denominator;
      q_candidate[1] = (matrix_n[2][1] + matrix_n[1][2]) / denominator;
      q_candidate[3] = (matrix_n[0][1] - matrix_n[1][0]) / denominator;
      break;
    case 3:
      q_candidate[0] = (matrix_n[1][2] - matrix_n[2][1]) / denominator;
      q_candidate[1] = (matrix_n[2][0] - matrix_n[0][2]) / denominator;
      q_candidate[2] = (matrix_n[0][1] - matrix_n[1][0]) / denominator;
      break;
    default:
      // not reach here
      break;
    }
    QUATERNION_make_from_array(&q_before_check, q_candidate, QUATERNION_SCALAR_POSITION_LAST);
  }

  C2A_MATH_ERROR error_check = QUATERNION_normalize_with_error_check(q_out, q_before_check);
  return error_check;
}


C2A_MATH_ERROR QUATERNION_make_from_euler_angles(Quaternion* q_out,
                                                 const float angle_1_rad,
                                                 const float angle_2_rad,
                                                 const float angle_3_rad,
                                                 const EULER_ANGLE_ROTATION_ORDER rotation_order)
{
  Quaternion q_rot_1, q_rot_2, q_rot_3;
  C2A_MATH_ERROR enum_check = C2A_MATH_ERROR_OK;

  switch (rotation_order)
  {
  case EULER_ANGLE_ROTATION_ORDER_123:
    q_rot_1 = QUATERNION_make_x_rot(angle_1_rad);
    q_rot_2 = QUATERNION_make_y_rot(angle_2_rad);
    q_rot_3 = QUATERNION_make_z_rot(angle_3_rad);
    break;
  case EULER_ANGLE_ROTATION_ORDER_321:
    q_rot_1 = QUATERNION_make_z_rot(angle_1_rad);
    q_rot_2 = QUATERNION_make_y_rot(angle_2_rad);
    q_rot_3 = QUATERNION_make_x_rot(angle_3_rad);
    break;
  case EULER_ANGLE_ROTATION_ORDER_121:  // just a example of i-j-i case
    q_rot_1 = QUATERNION_make_x_rot(angle_1_rad);
    q_rot_2 = QUATERNION_make_y_rot(angle_2_rad);
    q_rot_3 = QUATERNION_make_x_rot(angle_3_rad);
    break;
  default:
    // otherwise, replace the order with 1-2-3, which would be the most freqently used order
    q_rot_1 = QUATERNION_make_x_rot(angle_1_rad);
    q_rot_2 = QUATERNION_make_y_rot(angle_2_rad);
    q_rot_3 = QUATERNION_make_z_rot(angle_3_rad);
    enum_check = C2A_MATH_ERROR_ENUM_IRREGULAR;
    break;
  }

  Quaternion q_intermediate = QUATERNION_product(q_rot_1, q_rot_2);
  *q_out = QUATERNION_normalize(QUATERNION_product(q_intermediate, q_rot_3));

  return enum_check;
}

C2A_MATH_ERROR QUATERNION_make_from_vector_scalar(Quaternion* q_out,
                                                  const float vector_part[PHYSICAL_CONST_THREE_DIM],
                                                  const float scalar_part)
{
  Quaternion q_out_before_check;

  VECTOR3_copy(q_out_before_check.vector_part, vector_part);
  q_out_before_check.scalar_part = scalar_part;

  C2A_MATH_ERROR q_make_err = QUATERNION_normalize_with_error_check(q_out, q_out_before_check);
  return q_make_err;
}

Quaternion QUATERNION_make_x_rot(const float angle_around_x_rad)
{
  const float kAxisX[] = { 1.0f, 0.0f, 0.0f };
  Quaternion q_out;

  // since input for QUATERNION_make_from_axis_angle here is guaranteed, ignore err_check;
  C2A_MATH_ERROR err_check =
  QUATERNION_make_from_axis_angle(&q_out, kAxisX, angle_around_x_rad);
  return q_out;
}


Quaternion QUATERNION_make_y_rot(const float angle_around_y_rad)
{
  const float kAxisY[] = { 0.0f, 1.0f, 0.0f };
  Quaternion q_out;

  // since input for QUATERNION_make_from_axis_angle here is guaranteed, ignore err_check;
  C2A_MATH_ERROR err_check =
  QUATERNION_make_from_axis_angle(&q_out, kAxisY, angle_around_y_rad);
  return q_out;
}


Quaternion QUATERNION_make_z_rot(const float angle_around_z_rad)
{
  const float kAxisZ[] = { 0.0f, 0.0f, 1.0f };
  Quaternion q_out;

  // since input for QUATERNION_make_from_axis_angle here is guaranteed, ignore err_check;
  C2A_MATH_ERROR err_check =
  QUATERNION_make_from_axis_angle(&q_out, kAxisZ, angle_around_z_rad);
  return q_out;
}


Quaternion QUATERNION_make_unit(void)
{
  Quaternion q_out;
  for (int i = 0; i < PHYSICAL_CONST_THREE_DIM; i++)
  {
    q_out.vector_part[i] = 0.0f;
  }
  q_out.scalar_part = 1.0f;

  return q_out;
}


Quaternion QUATERNION_inverse(const Quaternion q_in)
{
  Quaternion q_out;
  q_out.scalar_part = q_in.scalar_part;
  VECTOR3_scalar_product(q_out.vector_part , - 1.0f, q_in.vector_part);

  return QUATERNION_normalize(q_out);
}


// Quaternionを別表現へ変換する関数

void QUATERNION_to_euler_axis_angle(float axis[PHYSICAL_CONST_THREE_DIM], float* angle_rad,
                                    const Quaternion q_in)
{
  float sin_half_angle = VECTOR3_norm(q_in.vector_part);

  C2A_MATH_ERROR is_sin_almost_0 =
  C2A_MATH_check_abs_range_violation(sin_half_angle, MATH_CONST_PROHIBIT_DIVISION_VALUE);
  if (is_sin_almost_0 == C2A_MATH_ERROR_OK)
  {
    *angle_rad = 0.0f;
    for (int i = 0; i < PHYSICAL_CONST_THREE_DIM; i++)
    {
      axis[i] = 0.0f;
    }
    return;
  }

  *angle_rad = 2.0f * atan2f(sin_half_angle, q_in.scalar_part);

  float axis_unnormalized[PHYSICAL_CONST_THREE_DIM];
  VECTOR3_scalar_product(axis_unnormalized, (1.0f / sin_half_angle), q_in.vector_part);
  // ignore error code since zero-devide is already handled
  C2A_MATH_ERROR err_check = VECTOR3_normalize(axis, axis_unnormalized);
}


void QUATERNION_to_rotation_matrix(float matrix[][PHYSICAL_CONST_THREE_DIM],
                                   const Quaternion q_in)
{
  // TODO_L: 実装は楽だが，計算時間がかかるかもしれないので，要確認．結果によっては従来と同じべた書きに変更

  //!< intermidiate matrix buffer for calculation
  float intermid_matrix[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM];
  MATRIX33_make_unit(intermid_matrix);

  //!< (q.scalar*q.scalar - dot(q_vec,q_vec))Eye(3)
  float diag_matrix[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM];
  float diag_component = q_in.scalar_part * q_in.scalar_part -
                         VECTOR3_inner_product(q_in.vector_part, q_in.vector_part);
  MATRIX33_multiply_scalar_matrix(diag_matrix, diag_component, intermid_matrix);

  //!< 2q_vec*(q_vec^t)
  float matrix_term_2nd[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM];
  VECTOR3_tensor_product(intermid_matrix, q_in.vector_part, q_in.vector_part);
  MATRIX33_multiply_scalar_matrix(matrix_term_2nd, 2.0f, intermid_matrix);

  //!< - 2q.scalar*skew(q_vec)
  float matrix_term_3rd[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM];
  MATRIX33_make_skew(intermid_matrix, q_in.vector_part);
  MATRIX33_multiply_scalar_matrix(matrix_term_3rd , - 2.0f * q_in.scalar_part, intermid_matrix);

  // (q.scalar*q.scalar - dot(q_vec,q_vec))Eye(3) + 2q_vec*(q_vec^t)
  MATRIX33_add(intermid_matrix, diag_matrix, matrix_term_2nd);

  // (q.scalar*q.scalar - dot(q_vec,q_vec))Eye(3) + 2q_vec*(q_vec^t) - 2q.scalar*skew(q_vec)
  float matrix_before_normalize[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM];
  MATRIX33_add(matrix_before_normalize, intermid_matrix, matrix_term_3rd);

  // just for keeping numerical precisions (ignore error code)
  C2A_MATH_ERROR dummy = MATRIX33_normalize(matrix, matrix_before_normalize);
}


void QUATERNION_to_array(float array_out[PHYSICAL_CONST_QUATERNION_DIM],
                         const Quaternion q_in,
                         const QUATERNION_SCALAR_POSITION position)
{
  QUATERNION_SCALAR_POSITION position_stored = position; //!< buffer to protect input
  if ((position != QUATERNION_SCALAR_POSITION_FIRST) &&
      (position != QUATERNION_SCALAR_POSITION_LAST))
  {
    position_stored = QUATERNION_SCALAR_POSITION_LAST;
  }

  array_out[position_stored] = q_in.scalar_part;

  int vector_part_begin = 0; //!< first position where vector part begins
  if (position_stored == QUATERNION_SCALAR_POSITION_FIRST)
  {
    vector_part_begin = 1;
  }

  for (int i = vector_part_begin; i < (vector_part_begin + PHYSICAL_CONST_THREE_DIM); i++)
  {
    array_out[i] = q_in.vector_part[i - vector_part_begin];
  }
}


// Quaternion演算関数

C2A_MATH_ERROR QUATERNION_is_normalized(const Quaternion q_in)
{
  float q_in_norm;
  C2A_MATH_ERROR ret;
  q_in_norm = 1.0f - QUATERNION_calc_norm(q_in);
  ret = C2A_MATH_check_abs_range_violation(q_in_norm, MATH_CONST_NORMAL_CHECK_THRESHOLD);
  if (ret != C2A_MATH_ERROR_OK)  return C2A_MATH_ERROR_UNNORMALIZED;
  return C2A_MATH_ERROR_OK;
}


Quaternion QUATERNION_normalize(const Quaternion q_in)
{
  Quaternion q_out;
  // ignore assertion, since correcting the error in succeeding process is difficult
  C2A_MATH_ERROR dummy_code = QUATERNION_normalize_with_error_check(&q_out, q_in);
  return q_out;
}


C2A_MATH_ERROR QUATERNION_normalize_with_error_check(Quaternion* q_out, const Quaternion q_in)
{
  float q_in_norm  = QUATERNION_calc_norm(q_in);
  C2A_MATH_ERROR is_norm_equals_0 =
  C2A_MATH_check_abs_range_violation(q_in_norm, MATH_CONST_PROHIBIT_DIVISION_VALUE);
  if (is_norm_equals_0 == C2A_MATH_ERROR_OK) // cannot process when norm is almost 0
  {
    *q_out = QUATERNION_make_unit();
    return C2A_MATH_ERROR_SINGULAR;
  }

  float norm_error = 1.0f - q_in_norm;
  C2A_MATH_ERROR is_norm_equals_1 =
  C2A_MATH_check_abs_range_violation(norm_error, MATH_CONST_NORMAL_CHECK_THRESHOLD);
  if (is_norm_equals_1 != C2A_MATH_ERROR_OK)
  {
    // returns warning when norm is far from 1, but process can be continued anyway
    is_norm_equals_1 = C2A_MATH_ERROR_UNNORMALIZED;
  }

  q_out->scalar_part = q_in.scalar_part / q_in_norm;
  VECTOR3_scalar_product(q_out->vector_part, (1.0f / q_in_norm), q_in.vector_part);

  return is_norm_equals_1;
}


float QUATERNION_calc_norm(const Quaternion q_in)
{
  float pow2_norm = q_in.scalar_part * q_in.scalar_part +
                    VECTOR3_inner_product(q_in.vector_part, q_in.vector_part);
  return (C2A_MATH_sqrtf(pow2_norm));
}


Quaternion QUATERNION_product(const Quaternion q_in1, const Quaternion q_in2)
{
  Quaternion q_out;

  // q_out.scalar = q1.scalar * q2.scalar - dot(q1.vector, q2.vector)
  q_out.scalar_part = q_in1.scalar_part * q_in2.scalar_part -
                      VECTOR3_inner_product(q_in1.vector_part, q_in2.vector_part);

  float q12_vector_cross[PHYSICAL_CONST_THREE_DIM];       //!< q1.vector x q2.vector
  VECTOR3_outer_product(q12_vector_cross, q_in1.vector_part, q_in2.vector_part);

  float mult_q1scalar_q2vector[PHYSICAL_CONST_THREE_DIM]; //!< q1.scalar*q2.vector
  VECTOR3_scalar_product(mult_q1scalar_q2vector, q_in1.scalar_part, q_in2.vector_part);

  float mult_q2scalar_q1vector[PHYSICAL_CONST_THREE_DIM]; //!< q2.scalar*q1.vector
  VECTOR3_scalar_product(mult_q2scalar_q1vector, q_in2.scalar_part, q_in1.vector_part);

  // q_out.vector = q1.vector x q2.vector + q1.scalar*q2.vector + q2.scalar*q1.vector
  float add_cross_mult[PHYSICAL_CONST_THREE_DIM];
  VECTOR3_add(add_cross_mult, q12_vector_cross, mult_q1scalar_q2vector);
  VECTOR3_add(q_out.vector_part, add_cross_mult, mult_q2scalar_q1vector);

  return QUATERNION_normalize(q_out);
}

float QUATERNION_inner_product(const Quaternion q_in1, const Quaternion q_in2)
{
  return q_in1.scalar_part * q_in2.scalar_part +
                      VECTOR3_inner_product(q_in1.vector_part, q_in2.vector_part);
}

Quaternion QUATERNION_calc_time_derivative(const Quaternion q_in, const float omega_in_rad_s[PHYSICAL_CONST_THREE_DIM])
{
  float q_array[PHYSICAL_CONST_QUATERNION_DIM];
  QUATERNION_to_array(q_array, q_in, QUATERNION_SCALAR_POSITION_LAST);

  // TODO_L: 4*4 matrixの計算が他の部分でも必要になったら，Matrixライブラリに4*4の計算も定義して，それを使うようにする
  float omega_matrix[PHYSICAL_CONST_QUATERNION_DIM][PHYSICAL_CONST_QUATERNION_DIM];

  omega_matrix[0][0] =  0.0f;
  omega_matrix[0][1] =  omega_in_rad_s[2];
  omega_matrix[0][2] = -omega_in_rad_s[1];
  omega_matrix[0][3] =  omega_in_rad_s[0];

  omega_matrix[1][0] = -omega_in_rad_s[2];
  omega_matrix[1][1] =  0.0f;
  omega_matrix[1][2] =  omega_in_rad_s[0];
  omega_matrix[1][3] =  omega_in_rad_s[1];

  omega_matrix[2][0] =  omega_in_rad_s[1];
  omega_matrix[2][1] = -omega_in_rad_s[0];
  omega_matrix[2][2] =  0.0f;
  omega_matrix[2][3] =  omega_in_rad_s[2];

  omega_matrix[3][0] = -omega_in_rad_s[0];
  omega_matrix[3][1] = -omega_in_rad_s[1];
  omega_matrix[3][2] = -omega_in_rad_s[2];
  omega_matrix[3][3] =  0.0f;

  float q_dot_array[PHYSICAL_CONST_QUATERNION_DIM];
  for (int i = 0; i < PHYSICAL_CONST_QUATERNION_DIM; i++)
  {
    q_dot_array[i] = 0.0f;
    for (int j = 0; j < PHYSICAL_CONST_QUATERNION_DIM; j++)
    {
      q_dot_array[i] += 0.5f * omega_matrix[i][j] * q_array[j];
    }
  }

  // Quaternionの微分のノルムは1とは限らないので，この先の処理には正規化処理を含むQUATERNION_make_from_arrayを使わないようにする
  float q_dot_array_vector_part[PHYSICAL_CONST_THREE_DIM];
  for (int i = 0; i < PHYSICAL_CONST_THREE_DIM; i++)
  {
    q_dot_array_vector_part[i] = q_dot_array[i];
  }

  Quaternion q_dot;
  VECTOR3_copy(q_dot.vector_part, q_dot_array_vector_part);
  q_dot.scalar_part = q_dot_array[QUATERNION_SCALAR_POSITION_LAST];

  return q_dot;
}

C2A_MATH_ERROR QUATERNION_calc_angular_velocity(float omega_rad_s[PHYSICAL_CONST_THREE_DIM],
                                                const Quaternion q_previous,
                                                const Quaternion q_current,
                                                const float time_diff_sec)
{
  Quaternion q_diff = QUATERNION_divide(q_previous, q_current);

  C2A_MATH_ERROR is_time_diff_sec_zero =
  C2A_MATH_check_abs_range_violation(time_diff_sec, MATH_CONST_PROHIBIT_DIVISION_VALUE);
  if (is_time_diff_sec_zero == C2A_MATH_ERROR_OK) return C2A_MATH_ERROR_SINGULAR;

  q_diff = QUATERNION_scalar_product(2.0f / time_diff_sec, q_diff);

  for (int i = 0; i < PHYSICAL_CONST_THREE_DIM; i++)
  {
    omega_rad_s[i] = q_diff.vector_part[i];
  }

  return C2A_MATH_ERROR_OK;
}

Quaternion QUATERNION_euler_propagate(const Quaternion q_previous,
                                      const float omega_in_rad_s[PHYSICAL_CONST_THREE_DIM],
                                      const float time_step_s)
{
  Quaternion q_time_derivative; //!< q_previousの時間微分
  q_time_derivative =  QUATERNION_calc_time_derivative(q_previous, omega_in_rad_s);

  Quaternion q_diff; //!< q_time_derivativeに時間ステップをかけた，クオータニオンの微小変化
  q_diff = QUATERNION_scalar_product(time_step_s, q_time_derivative);

  Quaternion q_current; //!< アップデートされたクオータニオン
  q_current = QUATERNION_normalize(QUATERNION_add(q_previous, q_diff));

  return q_current;
}

Quaternion QUATERNION_add(const Quaternion q_in1,
                          const Quaternion q_in2)
{
  float q_in1_array[PHYSICAL_CONST_QUATERNION_DIM]; //!< q_in1の配列形式での表示
  float q_in2_array[PHYSICAL_CONST_QUATERNION_DIM]; //!< q_in2の配列形式での表示

  // positionはFIRSTでもLASTでもよいが，かならず関数内で一貫するように変数に格納しておく
  QUATERNION_SCALAR_POSITION position = QUATERNION_SCALAR_POSITION_LAST;

  QUATERNION_to_array(q_in1_array, q_in1, position);
  QUATERNION_to_array(q_in2_array, q_in2, position);

  Quaternion q_out;
  float q_out_array[PHYSICAL_CONST_QUATERNION_DIM];
  for (int i = 0; i < PHYSICAL_CONST_QUATERNION_DIM; i++)
  {
    q_out_array[i] = q_in1_array[i] + q_in2_array[i];
  }

  QUATERNION_make_from_array_unnormalizing(&q_out, q_out_array, position);

  return q_out;
}

Quaternion QUATERNION_scalar_product(const float scalar,
                                     const Quaternion q_in)
{
  Quaternion q_out;

  VECTOR3_scalar_product(q_out.vector_part, scalar, q_in.vector_part);
  q_out.scalar_part = scalar * q_in.scalar_part;

  return q_out;
}

Quaternion QUATERNION_divide(const Quaternion q_in1, const Quaternion q_in2)
{
  Quaternion q_in1_inverse = QUATERNION_inverse(q_in1);
  return QUATERNION_product(q_in1_inverse, q_in2);
}

Quaternion QUATERNION_interpolate_slerp(const Quaternion q_in1, const Quaternion q_in2, const float factor)
{
  float clipped_factor = C2A_MATH_clip_value(factor, 1.0f, 0.0f);
  float inner_product = QUATERNION_inner_product(q_in1, q_in2);
  float angle = C2A_MATH_acos_rad(inner_product);
  float sin_angle = sin(angle);
  C2A_MATH_ERROR equal_zero =
    C2A_MATH_check_abs_range_violation(sin_angle, MATH_CONST_PROHIBIT_DIVISION_VALUE);
  if (equal_zero == C2A_MATH_ERROR_OK)  // means sin_angle is almost 0
  {
    return q_in1;
  }

  float scalar1 = sin((1.0f - clipped_factor) * angle) / sin_angle;
  float scalar2 = sin(clipped_factor * angle) / sin_angle;
  Quaternion q1 = QUATERNION_scalar_product(scalar1, q_in1);
  Quaternion q2 = QUATERNION_scalar_product(scalar2, q_in2);
  return QUATERNION_add(q1, q2);
}

void QUATERNION_trans_coordinate(float vector_out[PHYSICAL_CONST_THREE_DIM],
                                 const Quaternion q_in,
                                 const float vector_in[PHYSICAL_CONST_THREE_DIM])
{
  // TODO_L: vector_inを規格化してHamilton積で回して，norm掛けた方が速いかも…? 計算時間ヤバそうなら要確認．

  float rotation_matrix[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM];
  QUATERNION_to_rotation_matrix(rotation_matrix, q_in);
  MATRIX33_multiply_matrix_vector(vector_out, rotation_matrix, vector_in);
}

#pragma section
