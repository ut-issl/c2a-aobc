#pragma section REPRO
/**
* @file   stim210_filter.c
* @brief  STIM210ジャイロセンサ出力フィルタ
*/

#include "stim210_filter.h"

#include <src_core/TlmCmd/common_cmd_packet_util.h>
#include <src_core/Library/print.h>
#include <src_core/System/EventManager/event_logger.h>
#include "../../../../DriverInstances/di_stim210.h"

static Stim210Filter        stim210_filter_;
const  Stim210Filter* const stim210_filter = &stim210_filter_;

static ZFilter     APP_STIM210_FILTER_lpf_1st_[PHYSICAL_CONST_THREE_DIM];     //!< z_filter
static SpikeFilter APP_STIM210_FILTER_spike_[PHYSICAL_CONST_THREE_DIM];       //!< スパイク除去フィルタ

static void APP_STIM210_FILTER_init_(void);
static void APP_STIM210_FILTER_exec_(void);

static int APP_STIM210_FILTER_init_z_filter_(void);
static int APP_STIM210_FILTER_init_spike_filter_(void);

AppInfo APP_STIM210_FILTER_create_app(void)
{
  return AI_create_app_info("Stim210Filter", APP_STIM210_FILTER_init_, APP_STIM210_FILTER_exec_);
}

static void APP_STIM210_FILTER_init_(void)
{
  // same sampling frequency is applied to all filters
  // (since the frequency must be coincide with the call interval of this apllication)
  stim210_filter_.sampling_freq_Hz = 10.0f;

  int z_filter_init_result = APP_STIM210_FILTER_init_z_filter_();
  if (z_filter_init_result != 0)
  {
    Printf("STIM210 ZFilter init Failed ! \n");
  }

  int spike_filter_init_result = APP_STIM210_FILTER_init_spike_filter_();
  if (spike_filter_init_result != 0)
  {
    Printf("STIM210 SpikeFilter init Failed ! \n");
  }

  return;
}

static void APP_STIM210_FILTER_exec_(void)
{
  float spike_filter_out;

  for (uint8_t axis_id = 0; axis_id < PHYSICAL_CONST_THREE_DIM; axis_id++)
  {
    // spike noise should be removed before put the sensor output into lpf
    stim210_filter_.filter_error[axis_id] =
      SPIKE_FILTER_calc_output(&APP_STIM210_FILTER_spike_[axis_id],
                               &spike_filter_out,
                               stim210_driver[STIM210_IDX_IN_UNIT]->info.ang_vel_body_rad_sec[axis_id]);

    // record to EL as EL_ERROR_LEVEL_HIGH if spike noise are removed a configured number of times straight
    if (stim210_filter_.filter_error[axis_id] == C2A_MATH_ERROR_OTHERS)
    {
      EL_record_event(EL_GROUP_ERROR_STIM210, (uint32_t)axis_id, EL_ERROR_LEVEL_HIGH, (uint32_t)STIM210_EL_NOTE_NOISE_REMOVED);
    }
    else if (stim210_filter_.filter_error[axis_id] == C2A_MATH_ERROR_RANGE_OVER)
    {
      // TODO_L: ここでのEL登録は過剰なのでコメントアウトするが、復活させるか検討してもよい
      // EL_record_event(EL_GROUP_ERROR_STIM210, (uint32_t)axis_id, EL_ERROR_LEVEL_LOW, (uint32_t)STIM210_EL_NOTE_NOISE_REMOVED);
    }
    else
    {
      // 特に何もしない
    }

    stim210_filter_.ang_vel_est_body_rad_s[axis_id] =
      Z_FILTER_calc_output(&APP_STIM210_FILTER_lpf_1st_[axis_id], spike_filter_out);
  }

  return;
}

static int APP_STIM210_FILTER_init_z_filter_(void)
{
  // 値は調整してよいが一旦これで進める
  stim210_filter_.cut_off_freq_lpf_1st_Hz[0] = 1.0f;
  stim210_filter_.cut_off_freq_lpf_1st_Hz[1] = 1.0f;
  stim210_filter_.cut_off_freq_lpf_1st_Hz[2] = 1.0f;

  C2A_MATH_ERROR filter_setting_result_three_axis = C2A_MATH_ERROR_OK;
  for (uint8_t axis_id = 0; axis_id < PHYSICAL_CONST_THREE_DIM; axis_id++)
  {
    C2A_MATH_ERROR filter_setting_result =
    Z_FILTER_init_as_lpf_1st(&APP_STIM210_FILTER_lpf_1st_[axis_id],
                             stim210_filter_.sampling_freq_Hz,
                             stim210_filter_.cut_off_freq_lpf_1st_Hz[axis_id]);

    if (filter_setting_result != C2A_MATH_ERROR_OK)
    {
      filter_setting_result_three_axis = filter_setting_result;
    }
  }

  if (filter_setting_result_three_axis != C2A_MATH_ERROR_OK)
  {
    return -1;
  }

  return 0;
}

static int APP_STIM210_FILTER_init_spike_filter_(void)
{
  // 値は調整してよいが一旦これで進める
  for (uint8_t axis_id = 0; axis_id < PHYSICAL_CONST_THREE_DIM; axis_id++)
  {
    stim210_filter_.spike_filter_config[axis_id].count_limit_to_accept = 3;
    stim210_filter_.spike_filter_config[axis_id].count_limit_to_reject_continued_warning = 60;
    stim210_filter_.spike_filter_config[axis_id].reject_threshold = 0.01; // rad/s
    stim210_filter_.spike_filter_config[axis_id].amplitude_limit_to_accept_as_step = 0.002; // rad/s
  }

  C2A_MATH_ERROR filter_setting_result_three_axis = C2A_MATH_ERROR_OK;
  for (uint8_t axis_id = 0; axis_id < PHYSICAL_CONST_THREE_DIM; axis_id++)
  {
    C2A_MATH_ERROR filter_setting_result =
    SPIKE_FILTER_init(&APP_STIM210_FILTER_spike_[axis_id],
                      stim210_filter_.spike_filter_config[axis_id]);

    if (filter_setting_result != C2A_MATH_ERROR_OK)
    {
      filter_setting_result_three_axis = filter_setting_result;
    }
  }

  if (filter_setting_result_three_axis != C2A_MATH_ERROR_OK)
  {
    return -1;
  }

  return 0;
}

CCP_CmdRet Cmd_APP_STIM210_FILTER_SET_ZFILTER_PARAM(const CommonCmdPacket* packet)
{
  const uint8_t* param = CCP_get_param_head(packet);
  size_t read_out_offset = 0;

  STIM210_IDX sensor_id = (STIM210_IDX)(param[read_out_offset]);
  if (sensor_id >= STIM210_IDX_MAX) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
  read_out_offset++;

  uint8_t axis_id = param[read_out_offset];
  if (axis_id >= PHYSICAL_CONST_THREE_DIM) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
  read_out_offset++;

  float sampling_freq_Hz;
  ENDIAN_memcpy(&sampling_freq_Hz, param + read_out_offset, sizeof(float));
  if (sampling_freq_Hz < 0) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
  read_out_offset += sizeof(float);

  float cut_off_freq_Hz;
  ENDIAN_memcpy(&cut_off_freq_Hz, param + read_out_offset, sizeof(float));
  if (cut_off_freq_Hz < 0) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);

  C2A_MATH_ERROR init_error;
  init_error = Z_FILTER_init_as_lpf_1st(&APP_STIM210_FILTER_lpf_1st_[axis_id], sampling_freq_Hz, cut_off_freq_Hz);

  if (init_error == C2A_MATH_ERROR_OK)
  {
    stim210_filter_.cut_off_freq_lpf_1st_Hz[axis_id] = cut_off_freq_Hz;
    stim210_filter_.sampling_freq_Hz = sampling_freq_Hz;
    CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
  }
  else
  {
    // in this case, filter setting is already overwritten as through due to numerical error
    stim210_filter_.cut_off_freq_lpf_1st_Hz[axis_id] = 0.0f;
    stim210_filter_.sampling_freq_Hz = 0.0f;
    CCP_make_cmd_ret(CCP_EXEC_ILLEGAL_CONTEXT, init_error);
  }

  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_CONTEXT);
}

CCP_CmdRet Cmd_APP_STIM210_FILTER_SET_SPIKE_FILTER_PARAM(const CommonCmdPacket* packet)
{
  const uint8_t* param = CCP_get_param_head(packet);
  size_t read_out_offset = 0;

  STIM210_IDX sensor_id = (STIM210_IDX)(param[read_out_offset]);
  if (sensor_id >= STIM210_IDX_MAX) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
  read_out_offset++;

  uint8_t axis_id = param[read_out_offset];
  if (axis_id >= PHYSICAL_CONST_THREE_DIM) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
  read_out_offset++;

  SpikeFilter_Config config_recieved;
  config_recieved.count_limit_to_accept = param[read_out_offset];
  read_out_offset++;

  config_recieved.count_limit_to_reject_continued_warning = param[read_out_offset];
  read_out_offset++;

  float reject_threshold_float;
  ENDIAN_memcpy(&reject_threshold_float, param + read_out_offset, sizeof(float));
  if (reject_threshold_float < 0.0) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
  config_recieved.reject_threshold = (double)(reject_threshold_float);
  read_out_offset += sizeof(float);

  float amplitude_limit_to_accept_as_step_float;
  ENDIAN_memcpy(&amplitude_limit_to_accept_as_step_float, param + read_out_offset, sizeof(float));
  if (amplitude_limit_to_accept_as_step_float < 0.0) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
  config_recieved.amplitude_limit_to_accept_as_step = (double)(amplitude_limit_to_accept_as_step_float);

  C2A_MATH_ERROR init_error;
  init_error = SPIKE_FILTER_init(&APP_STIM210_FILTER_spike_[axis_id], config_recieved);

  if (init_error == C2A_MATH_ERROR_OK)
  {
    stim210_filter_.spike_filter_config[axis_id] = config_recieved;
    CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
  }
  else
  {
    CCP_make_cmd_ret(CCP_EXEC_ILLEGAL_CONTEXT, init_error);
  }

  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_CONTEXT);
}

CCP_CmdRet Cmd_APP_STIM210_FILTER_RESET_ZFILTER(const CommonCmdPacket* packet)
{
  const uint8_t* param = CCP_get_param_head(packet);
  size_t read_out_offset = 0;

  STIM210_IDX sensor_id = (STIM210_IDX)(param[read_out_offset]);
  if (sensor_id >= STIM210_IDX_MAX) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
  read_out_offset++;

  uint8_t axis_id = param[read_out_offset];
  if (axis_id >= PHYSICAL_CONST_THREE_DIM) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);

  Z_FILTER_reset(&APP_STIM210_FILTER_lpf_1st_[axis_id]);
  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

CCP_CmdRet Cmd_APP_STIM210_FILTER_RESET_SPIKE_FILTER(const CommonCmdPacket* packet)
{
  const uint8_t* param = CCP_get_param_head(packet);
  size_t read_out_offset = 0;

  STIM210_IDX sensor_id = (STIM210_IDX)(param[read_out_offset]);
  if (sensor_id >= STIM210_IDX_MAX) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
  read_out_offset++;

  uint8_t axis_id = param[read_out_offset];
  if (axis_id >= PHYSICAL_CONST_THREE_DIM) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);

  SPIKE_FILTER_reset(&APP_STIM210_FILTER_spike_[axis_id]);
  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

#pragma section
