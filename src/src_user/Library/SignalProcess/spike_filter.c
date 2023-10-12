#pragma section REPRO
/**
* @file   spike_filter.c
* @brief  スパイク除去フィルタ関連の関数群の定義
*/

#include "spike_filter.h"
#include <math.h>


C2A_MATH_ERROR SPIKE_FILTER_init(SpikeFilter* filter, const SpikeFilter_Config filter_setting)
{
  C2A_MATH_ERROR param_error = C2A_MATH_ERROR_OK;
  double reject_threshold_set = filter_setting.reject_threshold;
  double amplitude_limit_to_accept_as_step_set = filter_setting.amplitude_limit_to_accept_as_step;
  if ((reject_threshold_set < 0.0) || (amplitude_limit_to_accept_as_step_set < 0.0))
  {
    reject_threshold_set = 0.0;
    amplitude_limit_to_accept_as_step_set = 0.0;
    param_error = C2A_MATH_ERROR_RANGE_OVER;
  }

  filter->config.count_limit_to_accept = filter_setting.count_limit_to_accept;
  filter->config.reject_threshold      = reject_threshold_set;
  filter->config.amplitude_limit_to_accept_as_step = amplitude_limit_to_accept_as_step_set;
  filter->config.count_limit_to_reject_continued_warning = filter_setting.count_limit_to_reject_continued_warning;

  SPIKE_FILTER_reset(filter);

  return param_error;
}


void SPIKE_FILTER_reset(SpikeFilter* filter)
{
  filter->last_accept_val_         = 0.0;
  filter->count_to_accept_         = 0;
  filter->last_reject_val_         = 0.0;
  filter->count_to_reject_continued_warning_ = 0;

  return;
}


C2A_MATH_ERROR SPIKE_FILTER_calc_output(SpikeFilter* filter, float* output, const float input)
{
  double output_double = 0.0;

  C2A_MATH_ERROR judgement_result = SPIKE_FILTER_calc_output_double(filter, &output_double, (double)(input));
  *output = (float)(output_double);

  return judgement_result;
}


C2A_MATH_ERROR SPIKE_FILTER_calc_output_uint32(SpikeFilter* filter, uint32_t* output, const uint32_t input)
{
  double output_double = 0.0;

  C2A_MATH_ERROR judgement_result = SPIKE_FILTER_calc_output_double(filter, &output_double, (double)(input));
  *output = (uint32_t)(output_double);

  return judgement_result;
}


C2A_MATH_ERROR SPIKE_FILTER_calc_output_double(SpikeFilter* filter, double* output, const double input)
{
  C2A_MATH_ERROR judgement_result = C2A_MATH_ERROR_OK;

  double diff = input - filter->last_accept_val_;
  C2A_MATH_ERROR limit_check = C2A_MATH_check_abs_range_violation(diff, filter->config.reject_threshold);

  if ((limit_check != C2A_MATH_ERROR_OK) && (filter->count_to_accept_ < filter->config.count_limit_to_accept))
  {
    filter->count_to_accept_++;
    filter->count_to_reject_continued_warning_++;

    // reset counter if the differential amplitude of successive spikes is too large
    // so as not to mis-recognize oscillating signal as step signal
    double diff_of_successive_spikes = input - filter->last_reject_val_;
    if (fabs(diff_of_successive_spikes) > filter->config.amplitude_limit_to_accept_as_step)
    {
      filter->count_to_accept_ = 0;
    }

    filter->last_reject_val_ = input;
    *output = filter->last_accept_val_;
    judgement_result = C2A_MATH_ERROR_RANGE_OVER;
  }
  else
  {
    filter->count_to_accept_ = 0;
    filter->count_to_reject_continued_warning_ = 0;
    filter->last_accept_val_ = input;
    filter->last_reject_val_ = input;
    *output = input;
    judgement_result = C2A_MATH_ERROR_OK;
  }

  if (filter->count_to_reject_continued_warning_ > filter->config.count_limit_to_reject_continued_warning)
  {
    filter->count_to_reject_continued_warning_ = 0;
    judgement_result = C2A_MATH_ERROR_OTHERS;
  }

  return judgement_result;
}

#pragma section
