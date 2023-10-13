#pragma section REPRO
/**
* @file   rm3100_filter.c
* @brief  RM3100磁気センサ出力フィルタ
*/

#include "rm3100_filter.h"

#include <src_core/TlmCmd/common_cmd_packet_util.h>
#include <src_core/Library/print.h>
#include <src_core/System/EventManager/event_logger.h>
#include <src_user/Applications/UserDefined/AOCS/aocs_manager.h>

// Satellite Parameters
#include <src_user/Settings/SatelliteParameters/rm3100_parameters.h>

static Rm3100Filter        rm3100_filter_;
const  Rm3100Filter* const rm3100_filter = &rm3100_filter_;

static ZFilter     APP_RM3100_FILTER_lpf_1st_[RM3100_IDX_MAX][PHYSICAL_CONST_THREE_DIM];
static SpikeFilter APP_RM3100_FILTER_spike_[RM3100_IDX_MAX][PHYSICAL_CONST_THREE_DIM];

static void APP_RM3100_FILTER_init_(void);
static void APP_RM3100_FILTER_exec_(void);

static int APP_RM3100_FILTER_init_z_filter_(RM3100_IDX rm3100_id, const float cut_off_freq_Hz[PHYSICAL_CONST_THREE_DIM]);
static int APP_RM3100_FILTER_init_spike_filter_(RM3100_IDX rm3100_id,
                                                const uint8_t count_limit_to_accept[PHYSICAL_CONST_THREE_DIM],
                                                const uint8_t count_limit_to_reject_continued_warning[PHYSICAL_CONST_THREE_DIM],
                                                const float  reject_threshold_nT[PHYSICAL_CONST_THREE_DIM],
                                                const float  amplitude_limit_to_accept_as_step_nT[PHYSICAL_CONST_THREE_DIM]);

AppInfo APP_RM3100_FILTER_create_app(void)
{
  return AI_create_app_info("Rm3100Filter", APP_RM3100_FILTER_init_, APP_RM3100_FILTER_exec_);
}

static void APP_RM3100_FILTER_init_(void)
{
  // same sampling frequency is applied to all filters
  // (since the frequency must be coincide with the call interval of this application)
  rm3100_filter_.sampling_freq_Hz = 10.0f;

  int z_filter_aobc_init_result = APP_RM3100_FILTER_init_z_filter_(RM3100_IDX_ON_AOBC, RM3100_PARAMETERS_mag_aobc_cut_off_freq_lpf_1st_Hz);
  if (z_filter_aobc_init_result != 0)
  {
    Printf("RM3100 on AOBC ZFilter init Failed ! \n");
  }

  int z_filter_external_init_result = APP_RM3100_FILTER_init_z_filter_(RM3100_IDX_EXTERNAL, RM3100_PARAMETERS_mag_ext_cut_off_freq_lpf_1st_Hz);
  if (z_filter_external_init_result != 0)
  {
    Printf("RM3100 external ZFilter init Failed ! \n");
  }

  int spike_filter_aobc_init_result = APP_RM3100_FILTER_init_spike_filter_(RM3100_IDX_ON_AOBC,
                                                                           RM3100_PARAMETERS_mag_aobc_spike_count_limit_to_accept,
                                                                           RM3100_PARAMETERS_mag_aobc_spike_count_limit_to_reject_continued_warning,
                                                                           RM3100_PARAMETERS_mag_aobc_spike_reject_threshold_nT,
                                                                           RM3100_PARAMETERS_mag_aobc_spike_amplitude_limit_to_accept_as_step_nT);
  if (spike_filter_aobc_init_result != 0)
  {
    Printf("RM3100 on AOBC SpikeFilter init Failed ! \n");
  }

  int spike_filter_external_init_result =
    APP_RM3100_FILTER_init_spike_filter_(RM3100_IDX_EXTERNAL,
                                         RM3100_PARAMETERS_mag_ext_spike_count_limit_to_accept,
                                         RM3100_PARAMETERS_mag_ext_spike_count_limit_to_reject_continued_warning,
                                         RM3100_PARAMETERS_mag_ext_spike_reject_threshold_nT,
                                         RM3100_PARAMETERS_mag_ext_spike_amplitude_limit_to_accept_as_step_nT);
  if (spike_filter_external_init_result != 0)
  {
    Printf("RM3100 external SpikeFilter init Failed ! \n");
  }

  return;
}

static void APP_RM3100_FILTER_exec_(void)
{
  float spike_filter_out[RM3100_IDX_MAX];

  if (aocs_manager->mtq_output_state != AOCS_MANAGER_MTQ_OUTPUT_STATE_OFF)
  {
    return;  // 消磁対応
  }
  for (uint8_t rm3100_id = 0; rm3100_id < RM3100_IDX_MAX; rm3100_id++)
  {
    for (uint8_t axis_id = 0; axis_id < PHYSICAL_CONST_THREE_DIM; axis_id++)
    {
      // spike noise should be removed before put the sensor output into lpf
      rm3100_filter_.filter_error[rm3100_id][axis_id] =
        SPIKE_FILTER_calc_output(&APP_RM3100_FILTER_spike_[rm3100_id][axis_id],
                                 &spike_filter_out[rm3100_id],
                                 rm3100_driver[rm3100_id]->info.mag_body_nT[axis_id]);

      // record to EL as EL_ERROR_LEVEL_HIGH if spike noise are removed a configured number of times straight
      if (rm3100_filter_.filter_error[rm3100_id][axis_id] == C2A_MATH_ERROR_OTHERS)
      {
        EL_record_event(EL_GROUP_ERROR_RM3100,
                        (uint32_t)(rm3100_id * PHYSICAL_CONST_THREE_DIM + axis_id),
                        EL_ERROR_LEVEL_HIGH,
                        (uint32_t)RM3100_EL_NOTE_NOISE_REMOVED);
      }
      else if (rm3100_filter_.filter_error[rm3100_id][axis_id] == C2A_MATH_ERROR_RANGE_OVER)
      {
        // TODO_L: ここでのEL登録は過剰なのでコメントアウトするが、復活させるか検討してもよい
        // EL_record_event(EL_GROUP_ERROR_RM3100,
        //                 (uint32_t)(rm3100_id * PHYSICAL_CONST_THREE_DIM + axis_id),
        //                 EL_ERROR_LEVEL_LOW,
        //                 (uint32_t)RM3100_EL_NOTE_NOISE_REMOVED);
      }
      else
      {
        // 何もしない
      }

      rm3100_filter_.mag_est_body_nT[rm3100_id][axis_id] =
        Z_FILTER_calc_output(&APP_RM3100_FILTER_lpf_1st_[rm3100_id][axis_id], spike_filter_out[rm3100_id]);
    }
  }

  return;
}

static int APP_RM3100_FILTER_init_z_filter_(RM3100_IDX rm3100_id, const float cut_off_freq_Hz[PHYSICAL_CONST_THREE_DIM])
{
  for (uint8_t axis_id = 0; axis_id < PHYSICAL_CONST_THREE_DIM; axis_id++)
  {
    rm3100_filter_.cut_off_freq_lpf_1st_Hz[rm3100_id][axis_id] = cut_off_freq_Hz[axis_id];
  }

  C2A_MATH_ERROR filter_setting_result_three_axis = C2A_MATH_ERROR_OK;
  for (uint8_t axis_id = 0; axis_id < PHYSICAL_CONST_THREE_DIM; axis_id++)
  {
    C2A_MATH_ERROR filter_setting_result =
      Z_FILTER_init_as_lpf_1st(&APP_RM3100_FILTER_lpf_1st_[rm3100_id][axis_id],
                               rm3100_filter_.sampling_freq_Hz,
                               rm3100_filter_.cut_off_freq_lpf_1st_Hz[rm3100_id][axis_id]);

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

static int APP_RM3100_FILTER_init_spike_filter_(RM3100_IDX rm3100_id,
                                                const uint8_t count_limit_to_accept[PHYSICAL_CONST_THREE_DIM],
                                                const uint8_t count_limit_to_reject_continued_warning[PHYSICAL_CONST_THREE_DIM],
                                                const float  reject_threshold_nT[PHYSICAL_CONST_THREE_DIM],
                                                const float  amplitude_limit_to_accept_as_step_nT[PHYSICAL_CONST_THREE_DIM])
{
  for (uint8_t axis_id = 0; axis_id < PHYSICAL_CONST_THREE_DIM; axis_id++)
  {
    rm3100_filter_.spike_filter_config[rm3100_id][axis_id].count_limit_to_accept =
      count_limit_to_accept[axis_id];
    rm3100_filter_.spike_filter_config[rm3100_id][axis_id].count_limit_to_reject_continued_warning =
      count_limit_to_reject_continued_warning[axis_id];
    rm3100_filter_.spike_filter_config[rm3100_id][axis_id].reject_threshold = reject_threshold_nT[axis_id];
    rm3100_filter_.spike_filter_config[rm3100_id][axis_id].amplitude_limit_to_accept_as_step =
      amplitude_limit_to_accept_as_step_nT[axis_id];
  }

  C2A_MATH_ERROR filter_setting_result_three_axis = C2A_MATH_ERROR_OK;
  for (uint8_t axis_id = 0; axis_id < PHYSICAL_CONST_THREE_DIM; axis_id++)
  {
    C2A_MATH_ERROR filter_setting_result =
      SPIKE_FILTER_init(&APP_RM3100_FILTER_spike_[rm3100_id][axis_id],
                        rm3100_filter_.spike_filter_config[rm3100_id][axis_id]);

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

CCP_CmdRet Cmd_APP_RM3100_FILTER_SET_ZFILTER_PARAM(const CommonCmdPacket* packet)
{
  const uint8_t* param = CCP_get_param_head(packet);
  size_t read_out_offset = 0;

  RM3100_IDX rm3100_id = (RM3100_IDX)(param[read_out_offset]);
  if (rm3100_id >= RM3100_IDX_MAX) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
  read_out_offset++;

  uint8_t axis_id = param[read_out_offset];
  if (axis_id >= PHYSICAL_CONST_THREE_DIM) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
  read_out_offset++;

  float sampling_freq_Hz;
  ENDIAN_memcpy(&sampling_freq_Hz, param + read_out_offset, sizeof(float));
  if (sampling_freq_Hz < 0) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
  read_out_offset += sizeof(float);

  float cut_off_feq_Hz;
  ENDIAN_memcpy(&cut_off_feq_Hz, param + read_out_offset, sizeof(float));
  if (cut_off_feq_Hz < 0) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);

  C2A_MATH_ERROR init_error;
  switch (rm3100_id)
  {
  case RM3100_IDX_ON_AOBC:
    init_error = Z_FILTER_init_as_lpf_1st(&APP_RM3100_FILTER_lpf_1st_[RM3100_IDX_ON_AOBC][axis_id],
                                          sampling_freq_Hz, cut_off_feq_Hz);
    if (init_error != C2A_MATH_ERROR_OK)
    {
      // filter setting is already overwritten as through due to numerical error
      rm3100_filter_.cut_off_freq_lpf_1st_Hz[RM3100_IDX_ON_AOBC][axis_id] = 0.0f;
      rm3100_filter_.sampling_freq_Hz = 0.0f;
      return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_CONTEXT);
    }

    rm3100_filter_.cut_off_freq_lpf_1st_Hz[RM3100_IDX_ON_AOBC][axis_id] = cut_off_feq_Hz;
    rm3100_filter_.sampling_freq_Hz = sampling_freq_Hz;
    return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
  case RM3100_IDX_EXTERNAL:
    init_error = Z_FILTER_init_as_lpf_1st(&APP_RM3100_FILTER_lpf_1st_[RM3100_IDX_EXTERNAL][axis_id],
                                          sampling_freq_Hz, cut_off_feq_Hz);
    if (init_error != C2A_MATH_ERROR_OK)
    {
      // filter setting is already overwritten as through due to numerical error
      rm3100_filter_.cut_off_freq_lpf_1st_Hz[RM3100_IDX_EXTERNAL][axis_id] = 0.0f;
      rm3100_filter_.sampling_freq_Hz = 0.0f;
      return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_CONTEXT);
    }

    rm3100_filter_.cut_off_freq_lpf_1st_Hz[RM3100_IDX_EXTERNAL][axis_id] = cut_off_feq_Hz;
    rm3100_filter_.sampling_freq_Hz = sampling_freq_Hz;
    return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
  default:
    return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
  }
}

CCP_CmdRet Cmd_APP_RM3100_FILTER_SET_SPIKE_FILTER_PARAM(const CommonCmdPacket* packet)
{
  const uint8_t* param = CCP_get_param_head(packet);
  size_t read_out_offset = 0;

  RM3100_IDX rm3100_id = (RM3100_IDX)(param[read_out_offset]);
  if (rm3100_id >= RM3100_IDX_MAX) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
  read_out_offset++;

  uint8_t axis_id = param[read_out_offset];
  if (axis_id >= PHYSICAL_CONST_THREE_DIM) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
  read_out_offset++;

  SpikeFilter_Config config_received;
  config_received.count_limit_to_accept = param[read_out_offset];
  read_out_offset++;

  config_received.count_limit_to_reject_continued_warning = param[read_out_offset];
  read_out_offset++;

  float reject_threshold_float;
  ENDIAN_memcpy(&reject_threshold_float, param + read_out_offset, sizeof(float));
  if (reject_threshold_float < 0.0) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
  config_received.reject_threshold = (double)(reject_threshold_float);
  read_out_offset += sizeof(float);

  float amplitude_limit_to_accept_as_step_float;
  ENDIAN_memcpy(&amplitude_limit_to_accept_as_step_float, param + read_out_offset, sizeof(float));
  if (amplitude_limit_to_accept_as_step_float < 0.0) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
  config_received.amplitude_limit_to_accept_as_step = (double)(amplitude_limit_to_accept_as_step_float);

  C2A_MATH_ERROR init_error;
  switch (rm3100_id)
  {
  case RM3100_IDX_ON_AOBC:
    init_error = SPIKE_FILTER_init(&APP_RM3100_FILTER_spike_[RM3100_IDX_ON_AOBC][axis_id],
                                   config_received);
    if (init_error != C2A_MATH_ERROR_OK)
    {
      return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_CONTEXT);
    }

    rm3100_filter_.spike_filter_config[RM3100_IDX_ON_AOBC][axis_id] = config_received;
    return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
  case RM3100_IDX_EXTERNAL:
    init_error = SPIKE_FILTER_init(&APP_RM3100_FILTER_spike_[RM3100_IDX_EXTERNAL][axis_id],
                                   config_received);
    if (init_error != C2A_MATH_ERROR_OK)
    {
      return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_CONTEXT);
    }

    rm3100_filter_.spike_filter_config[RM3100_IDX_EXTERNAL][axis_id] = config_received;
    return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
  default:
    return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
  }
}

#pragma section
