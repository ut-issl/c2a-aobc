#pragma section REPRO
/**
* @file   z_filter.c
* @brief  離散線形フィルタ関連の関数群の定義
*/

#include "z_filter.h"

#include <math.h>
#include <src_user/Library/math_constants.h>


//!< bilinear transformation
static C2A_MATH_ERROR Z_FILTER_bilinear_trans_(ZFilter* filter,
                                               const double ts_sec,
                                               const double numerator_continuous_coeff[Z_FILTER_ORDER_MAX + 1],
                                               const double denominator_continuous_coeff[Z_FILTER_ORDER_MAX + 1]);

static void Z_FILTER_zero_clear_coeff_(double numerator_coeff[Z_FILTER_ORDER_MAX + 1],
                                       double denominator_coeff[Z_FILTER_ORDER_MAX + 1]);


C2A_MATH_ERROR Z_FILTER_init_as_lpf_1st(ZFilter* filter, const float f_sample_Hz, const float f_cut_Hz)
{
  // in these case, the filter maight not work
  if (f_cut_Hz <= 0)
  {
    filter->order = 0; // set the filter as feed-through
    return C2A_MATH_ERROR_OTHERS;
  }
  if ((f_sample_Hz < 2.0f * f_cut_Hz) || (f_sample_Hz < MATH_CONST_PROHIBIT_DIVISION_VALUE))
  {
    filter->order = 0; // set the filter as feed-through
    return C2A_MATH_ERROR_OTHERS;
  }

  //!< coeffs of filter function in continuous time domain
  double numerator_continuous_coeff[Z_FILTER_ORDER_MAX + 1];
  double denominator_continuous_coeff[Z_FILTER_ORDER_MAX + 1];
  Z_FILTER_zero_clear_coeff_(numerator_continuous_coeff, denominator_continuous_coeff);

  numerator_continuous_coeff[2] = 0.0;
  numerator_continuous_coeff[1] = 0.0;
  numerator_continuous_coeff[0] = MATH_CONST_2PI_DOUBLE * (double)(f_cut_Hz);

  denominator_continuous_coeff[2] = 0.0;
  denominator_continuous_coeff[1] = 1.0;
  denominator_continuous_coeff[0] = MATH_CONST_2PI_DOUBLE * (double)(f_cut_Hz);

  filter->order = 1;
  double ts_sec = 1.0 / (double)(f_sample_Hz);
  C2A_MATH_ERROR transration_error = Z_FILTER_bilinear_trans_(filter, ts_sec, numerator_continuous_coeff, denominator_continuous_coeff);
  if (transration_error != C2A_MATH_ERROR_OK) return transration_error;

  Z_FILTER_reset(filter);

  return C2A_MATH_ERROR_OK;
}


C2A_MATH_ERROR Z_FILTER_init_as_lpf_2nd(ZFilter* filter, const float f_sample_Hz, const float f_cut_Hz, const float xi)
{
  // in these case, the filter maight not work
  if (f_cut_Hz <= 0)
  {
    filter->order = 0; // set the filter as feed-through
    return C2A_MATH_ERROR_OTHERS;
  }
  if ((f_sample_Hz < 2.0f * f_cut_Hz) || (f_sample_Hz < MATH_CONST_PROHIBIT_DIVISION_VALUE))
  {
    filter->order = 0; // set the filter as feed-through
    return C2A_MATH_ERROR_OTHERS;
  }

  //!< coeffs of filter function in continuous time domain
  double numerator_continuous_coeff[Z_FILTER_ORDER_MAX + 1];
  double denominator_continuous_coeff[Z_FILTER_ORDER_MAX + 1];
  Z_FILTER_zero_clear_coeff_(numerator_continuous_coeff, denominator_continuous_coeff);

  numerator_continuous_coeff[2] = 0.0;
  numerator_continuous_coeff[1] = 0.0;
  double fc_rad = MATH_CONST_2PI_DOUBLE * (double)(f_cut_Hz);
  numerator_continuous_coeff[0] = fc_rad * fc_rad;

  denominator_continuous_coeff[2] = 1.0;
  denominator_continuous_coeff[1] = 2.0 * (double)(xi) * fc_rad;
  denominator_continuous_coeff[0] = fc_rad * fc_rad;

  filter->order = 2;
  double ts_sec = 1.0 / (double)(f_sample_Hz);
  C2A_MATH_ERROR transration_error = Z_FILTER_bilinear_trans_(filter, ts_sec, numerator_continuous_coeff, denominator_continuous_coeff);
  if (transration_error != C2A_MATH_ERROR_OK) return transration_error;

  Z_FILTER_reset(filter);

  return C2A_MATH_ERROR_OK;
}


C2A_MATH_ERROR Z_FILTER_init_as_notch(ZFilter* filter, const float f_sample_Hz, const float f_cut_Hz,
                                      const float xi, const float depth)
{
  // in these case, the filter maight not work
  if (f_cut_Hz <= 0)
  {
    filter->order = 0; // set the filter as feed-through
    return C2A_MATH_ERROR_OTHERS;
  }
  if ((f_sample_Hz < 2.0f * f_cut_Hz) || (f_sample_Hz < MATH_CONST_PROHIBIT_DIVISION_VALUE))
  {
    filter->order = 0; // set the filter as feed-through
    return C2A_MATH_ERROR_OTHERS;
  }

  //!< coeffs of filter function in continuous time domain
  double numerator_continuous_coeff[Z_FILTER_ORDER_MAX + 1];
  double denominator_continuous_coeff[Z_FILTER_ORDER_MAX + 1];
  Z_FILTER_zero_clear_coeff_(numerator_continuous_coeff, denominator_continuous_coeff);

  double fc_rad = MATH_CONST_2PI_DOUBLE * (double)(f_cut_Hz);

  numerator_continuous_coeff[2] = 1.0;
  numerator_continuous_coeff[1] = 2.0 * (double)(depth) * (double)(xi)* fc_rad;
  numerator_continuous_coeff[0] = fc_rad * fc_rad;

  denominator_continuous_coeff[2] = 1.0;
  denominator_continuous_coeff[1] = 2.0 * (double)(xi)* fc_rad;
  denominator_continuous_coeff[0] = fc_rad * fc_rad;

  filter->order = 2;
  double ts_sec = 1.0 / (double)(f_sample_Hz);
  C2A_MATH_ERROR transration_error = Z_FILTER_bilinear_trans_(filter, ts_sec, numerator_continuous_coeff, denominator_continuous_coeff);
  if (transration_error != C2A_MATH_ERROR_OK) return transration_error;

  Z_FILTER_reset(filter);

  return C2A_MATH_ERROR_OK;
}


void Z_FILTER_reset(ZFilter* filter)
{
  for (uint8_t i_order = 0; i_order < Z_FILTER_ORDER_MAX; i_order++)
  {
    filter->input_previous[i_order]  = 0.0;
    filter->output_previous[i_order] = 0.0;
  }
}


float Z_FILTER_calc_output(ZFilter* filter, const float input)
{
  double output_double = Z_FILTER_calc_output_double(filter, (double)(input));
  return (float)(output_double);
}


double Z_FILTER_calc_output_double(ZFilter* filter, const double input)
{
  double output_d = input;

  switch (filter->order)
  {
  case 2:
    output_d = filter->numerator_discrete_coeff[0] * input +
               filter->numerator_discrete_coeff[1] * filter->input_previous[0] +
               filter->numerator_discrete_coeff[2] * filter->input_previous[1] -
               filter->denominator_discrete_coeff[0] * filter->output_previous[0] -
               filter->denominator_discrete_coeff[1] * filter->output_previous[1];

    filter->input_previous[1]  = filter->input_previous[0];
    filter->output_previous[1] = filter->output_previous[0];
    filter->input_previous[0]  = input;
    filter->output_previous[0] = output_d;
    break;
  case 1:
    output_d = filter->numerator_discrete_coeff[0] * input +
               filter->numerator_discrete_coeff[1] * filter->input_previous[0] -
               filter->denominator_discrete_coeff[0] * filter->output_previous[0];

    filter->input_previous[0]  = input;
    filter->output_previous[0] = output_d;
    break;
  default:
    // in this case this function returns input as output, thus nothing to do.
    break;
  }

  return output_d;
}


static C2A_MATH_ERROR Z_FILTER_bilinear_trans_(ZFilter* filter,
                                               const double ts_sec,
                                               const double numerator_continuous_coeff[Z_FILTER_ORDER_MAX + 1],
                                               const double denominator_continuous_coeff[Z_FILTER_ORDER_MAX + 1])
{
  double pow2_ts = ts_sec * ts_sec;
  double inverse_trans_coeff = 1.0;         //!< inverse of coeffs for bilinear trans
  C2A_MATH_ERROR transration_success = C2A_MATH_ERROR_OK;

  switch (filter->order)
  {
  case 2:
    inverse_trans_coeff = 4.0 * denominator_continuous_coeff[2] +
                          2.0 * denominator_continuous_coeff[1] * ts_sec +
                          denominator_continuous_coeff[0] * pow2_ts;

    if (fabs(inverse_trans_coeff) < MATH_CONST_PROHIBIT_DIVISION_VALUE)
    {
      transration_success = C2A_MATH_ERROR_SINGULAR;
      break;
    }

    filter->numerator_discrete_coeff[0] = 4.0 * numerator_continuous_coeff[2] +
                                          2.0 * numerator_continuous_coeff[1] * ts_sec +
                                          numerator_continuous_coeff[0] * pow2_ts;
    filter->numerator_discrete_coeff[0] = filter->numerator_discrete_coeff[0] / inverse_trans_coeff;

    filter->numerator_discrete_coeff[1] = 2.0 * numerator_continuous_coeff[0] * pow2_ts -
                                          8.0 * numerator_continuous_coeff[2];
    filter->numerator_discrete_coeff[1] = filter->numerator_discrete_coeff[1] / inverse_trans_coeff;

    filter->numerator_discrete_coeff[2] = 4.0 * numerator_continuous_coeff[2] -
                                          2.0 * numerator_continuous_coeff[1] * ts_sec +
                                          numerator_continuous_coeff[0] * pow2_ts;
    filter->numerator_discrete_coeff[2] = filter->numerator_discrete_coeff[2] / inverse_trans_coeff;

    filter->denominator_discrete_coeff[0] = 2.0 * denominator_continuous_coeff[0] * pow2_ts -
                                            8.0 * denominator_continuous_coeff[2];
    filter->denominator_discrete_coeff[0] = filter->denominator_discrete_coeff[0] / inverse_trans_coeff;

    filter->denominator_discrete_coeff[1] = 4.0 * denominator_continuous_coeff[2] -
                                            2.0 * denominator_continuous_coeff[1] * ts_sec +
                                            denominator_continuous_coeff[0] * pow2_ts;
    filter->denominator_discrete_coeff[1] = filter->denominator_discrete_coeff[1] / inverse_trans_coeff;
    break;
  case 1:
    inverse_trans_coeff = 2.0 * denominator_continuous_coeff[1] + denominator_continuous_coeff[0] * ts_sec;

    if (fabs(inverse_trans_coeff) < MATH_CONST_PROHIBIT_DIVISION_VALUE)
    {
      transration_success = C2A_MATH_ERROR_SINGULAR;
      break;
    }

    filter->numerator_discrete_coeff[0] = (numerator_continuous_coeff[0] * ts_sec) / inverse_trans_coeff;
    filter->numerator_discrete_coeff[1] = (numerator_continuous_coeff[0] * ts_sec) / inverse_trans_coeff;
    filter->numerator_discrete_coeff[2] = 0.0;

    filter->denominator_discrete_coeff[0] = (denominator_continuous_coeff[0] * ts_sec -
                                             2.0 * denominator_continuous_coeff[1]) / inverse_trans_coeff;
    filter->denominator_discrete_coeff[1] = 0.0;
    break;
  default:
    transration_success = C2A_MATH_ERROR_SINGULAR;
    break;
  }

  if (transration_success == C2A_MATH_ERROR_SINGULAR)
  {
    // set the filter order as feed-through
    filter->order = 0;

    // set the discrete_coeffs as that of feed-through
    // however, in this case, discrete_coeffs are not used in Z_Filter_calc_output
    filter->numerator_discrete_coeff[0] = 1.0;
  }

  return transration_success;
}


static void Z_FILTER_zero_clear_coeff_(double numerator_coeff[Z_FILTER_ORDER_MAX + 1],
                                       double denominator_coeff[Z_FILTER_ORDER_MAX + 1])
{
  for (uint8_t i_order = 0; i_order < (Z_FILTER_ORDER_MAX + 1); i_order++)
  {
    numerator_coeff[i_order]   = 0.0;
    denominator_coeff[i_order] = 0.0;
  }

  return;
}


#pragma section
