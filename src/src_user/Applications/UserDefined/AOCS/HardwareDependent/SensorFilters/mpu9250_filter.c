#pragma section REPRO
/**
* @file   mpu9250_filter.c
* @brief  MPU9250ジャイロ、磁気センサ出力フィルタ
*/

#include "mpu9250_filter.h"

#include <src_core/Library/print.h>
#include <src_core/TlmCmd/common_cmd_packet_util.h>
#include <src_core/System/EventManager/event_logger.h>
#include <src_user/Applications/DriverInstances/di_mpu9250.h>
#include <src_user/Applications/UserDefined/AOCS/aocs_manager.h>
#include <src_user/Library/vector3.h>

// Satellite Parameters
#include <src_user/Settings/SatelliteParameters/mpu9250_parameters.h>

static Mpu9250Filter        mpu9250_filter_;
const  Mpu9250Filter* const mpu9250_filter = &mpu9250_filter_;

static ZFilter     APP_MPU9250_FILTER_lpf_1st_mag_[PHYSICAL_CONST_THREE_DIM];     //!< 磁気センサ用z_filter
static ZFilter     APP_MPU9250_FILTER_lpf_1st_gyro_[PHYSICAL_CONST_THREE_DIM];    //!< Gyroセンサ用z_filter
static SpikeFilter APP_MPU9250_FILTER_spike_mag_[PHYSICAL_CONST_THREE_DIM];       //!< 磁気センサ用スパイク除去フィルタ
static SpikeFilter APP_MPU9250_FILTER_spike_gyro_[PHYSICAL_CONST_THREE_DIM];      //!< Gyroセンサ用スパイク除去フィルタ

static void APP_MPU9250_FILTER_init_(void);
static void APP_MPU9250_FILTER_exec_(void);

static int APP_MPU9250_FILTER_init_z_filter_mag_(void);
static int APP_MPU9250_FILTER_init_z_filter_gyro_(void);
static int APP_MPU9250_FILTER_init_spike_filter_mag_(void);
static int APP_MPU9250_FILTER_init_spike_filter_gyro_(void);


AppInfo APP_MPU9250_FILTER_create_app(void)
{
  return AI_create_app_info("Mpu9250Filter", APP_MPU9250_FILTER_init_, APP_MPU9250_FILTER_exec_);
}

static void APP_MPU9250_FILTER_init_(void)
{
  // same sampling frequency is applied to all filters
  // (since the frequency must be coincide with the call interval of this application)
  mpu9250_filter_.sampling_freq_Hz = 10.0f;

  int z_filter_mag_init_result      = APP_MPU9250_FILTER_init_z_filter_mag_();
  if (z_filter_mag_init_result != 0)
  {
    Printf("MPU9250 ZFilterMag init Failed ! \n");
  }

  int z_filter_gyro_init_result     = APP_MPU9250_FILTER_init_z_filter_gyro_();
  if (z_filter_gyro_init_result != 0)
  {
    Printf("MPU9250 ZFilterGyro init Failed ! \n");
  }

  int spike_filter_mag_init_result  = APP_MPU9250_FILTER_init_spike_filter_mag_();
  if (spike_filter_mag_init_result != 0)
  {
    Printf("MPU9250 SpikeFilterMag init Failed ! \n");
  }

  int spike_filter_gyro_init_result = APP_MPU9250_FILTER_init_spike_filter_gyro_();
  if (spike_filter_gyro_init_result != 0)
  {
    Printf("MPU9250 SpikeFilterGyro init Failed ! \n");
  }

  return;
}

static void APP_MPU9250_FILTER_exec_(void)
{
  float spike_filter_mag_out, spike_filter_gyro_out;

  for (uint8_t axis_id = 0; axis_id < PHYSICAL_CONST_THREE_DIM; axis_id++)
  {
    // 磁場
    if (aocs_manager->mtq_output_state == AOCS_MANAGER_MTQ_OUTPUT_STATE_OFF)
    {
      // spike noise should be removed before put the sensor output into lpf
      mpu9250_filter_.filter_mag_error[axis_id] =
      SPIKE_FILTER_calc_output(&APP_MPU9250_FILTER_spike_mag_[axis_id],
                               &spike_filter_mag_out,
                               mpu9250_driver[MPU9250_IDX_ON_AOBC]->info.mag_body_nT[axis_id]);

      // record to EL as EL_ERROR_LEVEL_HIGH if spike noise are removed a configured number of times straight
      if (mpu9250_filter_.filter_mag_error[axis_id] == C2A_MATH_ERROR_OTHERS)
      {
        EL_record_event(EL_GROUP_ERROR_MPU9250, (uint32_t)axis_id, EL_ERROR_LEVEL_HIGH, (uint32_t)MPU9250_EL_NOTE_NOISE_REMOVED_MAG);
      }
      else if (mpu9250_filter_.filter_mag_error[axis_id] == C2A_MATH_ERROR_RANGE_OVER)
      {
        // TODO_L: ここでのEL登録は過剰なのでコメントアウトするが、復活させるか検討してもよい
        // EL_record_event(EL_GROUP_ERROR_MPU9250, (uint32_t)axis_id, EL_ERROR_LEVEL_LOW, (uint32_t)MPU9250_EL_NOTE_NOISE_REMOVED_MAG);
      }
      else
      {
        // 何もしない
      }

      mpu9250_filter_.mag_est_body_nT[axis_id] =
      Z_FILTER_calc_output(&APP_MPU9250_FILTER_lpf_1st_mag_[axis_id], spike_filter_mag_out);
    }

    // ジャイロ
    mpu9250_filter_.filter_gyro_error[axis_id] =
    SPIKE_FILTER_calc_output(&APP_MPU9250_FILTER_spike_gyro_[axis_id],
                             &spike_filter_gyro_out,
                             mpu9250_driver[MPU9250_IDX_ON_AOBC]->info.ang_vel_body_rad_s[axis_id]);

    // record to EL as EL_ERROR_LEVEL_HIGH if spike noise are removed a configured number of times straight
    if (mpu9250_filter_.filter_gyro_error[axis_id] == C2A_MATH_ERROR_OTHERS)
    {
      EL_record_event(EL_GROUP_ERROR_MPU9250, (uint32_t)axis_id, EL_ERROR_LEVEL_HIGH, (uint32_t)MPU9250_EL_NOTE_NOISE_REMOVED_GYRO);
    }
    else if (mpu9250_filter_.filter_gyro_error[axis_id] == C2A_MATH_ERROR_RANGE_OVER)
    {
      // TODO_L: ここでのEL登録は過剰なのでコメントアウトするが、復活させるか検討してもよい
      // EL_record_event(EL_GROUP_ERROR_MPU9250, (uint32_t)axis_id, EL_ERROR_LEVEL_LOW, (uint32_t)MPU9250_EL_NOTE_NOISE_REMOVED_GYRO);
    }

    mpu9250_filter_.ang_vel_est_body_rad_s[axis_id] =
    Z_FILTER_calc_output(&APP_MPU9250_FILTER_lpf_1st_gyro_[axis_id], spike_filter_gyro_out);
  }

  return;
}


static int APP_MPU9250_FILTER_init_z_filter_mag_(void)
{
  VECTOR3_copy(mpu9250_filter_.cut_off_freq_lpf_1st_mag_Hz, MPU9250_PARAMETERS_mag_cut_off_freq_lpf_1st_Hz);

  C2A_MATH_ERROR filter_setting_result_three_axis = C2A_MATH_ERROR_OK;
  for (uint8_t axis_id = 0; axis_id < PHYSICAL_CONST_THREE_DIM; axis_id++)
  {
    C2A_MATH_ERROR filter_setting_result =
    Z_FILTER_init_as_lpf_1st(&APP_MPU9250_FILTER_lpf_1st_mag_[axis_id],
                             mpu9250_filter_.sampling_freq_Hz,
                             mpu9250_filter_.cut_off_freq_lpf_1st_mag_Hz[axis_id]);

    if (filter_setting_result != C2A_MATH_ERROR_OK)
    {
      filter_setting_result_three_axis = filter_setting_result;
    }
  }

  if (filter_setting_result_three_axis != C2A_MATH_ERROR_OK) return -1;

  return 0;
}


static int APP_MPU9250_FILTER_init_z_filter_gyro_(void)
{
  VECTOR3_copy(mpu9250_filter_.cut_off_freq_lpf_1st_gyro_Hz, MPU9250_PARAMETERS_gyro_cut_off_freq_lpf_1st_Hz);

  C2A_MATH_ERROR filter_setting_result_three_axis = C2A_MATH_ERROR_OK;
  for (uint8_t axis_id = 0; axis_id < PHYSICAL_CONST_THREE_DIM; axis_id++)
  {
    C2A_MATH_ERROR filter_setting_result =
    Z_FILTER_init_as_lpf_1st(&APP_MPU9250_FILTER_lpf_1st_gyro_[axis_id],
                             mpu9250_filter_.sampling_freq_Hz,
                             mpu9250_filter_.cut_off_freq_lpf_1st_gyro_Hz[axis_id]);

    if (filter_setting_result != C2A_MATH_ERROR_OK)
    {
      filter_setting_result_three_axis = filter_setting_result;
    }
  }

  if (filter_setting_result_three_axis != C2A_MATH_ERROR_OK) return -1;

  return 0;
}


static int APP_MPU9250_FILTER_init_spike_filter_mag_(void)
{

  for (uint8_t axis_id = 0; axis_id < PHYSICAL_CONST_THREE_DIM; axis_id++)
  {
    mpu9250_filter_.spike_filter_config_mag[axis_id].count_limit_to_accept = MPU9250_PARAMETERS_mag_spike_count_limit_to_accept[axis_id];
    mpu9250_filter_.spike_filter_config_mag[axis_id].count_limit_to_reject_continued_warning = MPU9250_PARAMETERS_mag_spike_count_limit_to_reject_continued_warning[axis_id];
    mpu9250_filter_.spike_filter_config_mag[axis_id].reject_threshold = MPU9250_PARAMETERS_mag_spike_reject_threshold_nT[axis_id];
    mpu9250_filter_.spike_filter_config_mag[axis_id].amplitude_limit_to_accept_as_step = MPU9250_PARAMETERS_mag_spike_amplitude_limit_to_accept_as_step_nT[axis_id];
  }

  C2A_MATH_ERROR filter_setting_result_three_axis = C2A_MATH_ERROR_OK;
  for (uint8_t axis_id = 0; axis_id < PHYSICAL_CONST_THREE_DIM; axis_id++)
  {
    C2A_MATH_ERROR filter_setting_result =
    SPIKE_FILTER_init(&APP_MPU9250_FILTER_spike_mag_[axis_id],
                      mpu9250_filter_.spike_filter_config_mag[axis_id]);

    if (filter_setting_result != C2A_MATH_ERROR_OK)
    {
      filter_setting_result_three_axis = filter_setting_result;
    }
  }

  if (filter_setting_result_three_axis != C2A_MATH_ERROR_OK) return -1;

  return 0;
}


static int APP_MPU9250_FILTER_init_spike_filter_gyro_(void)
{
  for (uint8_t axis_id = 0; axis_id < PHYSICAL_CONST_THREE_DIM; axis_id++)
  {
    mpu9250_filter_.spike_filter_config_gyro[axis_id].count_limit_to_accept = MPU9250_PARAMETERS_gyro_spike_count_limit_to_accept[axis_id];
    mpu9250_filter_.spike_filter_config_gyro[axis_id].count_limit_to_reject_continued_warning = MPU9250_PARAMETERS_gyro_spike_count_limit_to_reject_continued_warning[axis_id];
    mpu9250_filter_.spike_filter_config_gyro[axis_id].reject_threshold = MPU9250_PARAMETERS_gyro_spike_reject_threshold_rad_s[axis_id];
    mpu9250_filter_.spike_filter_config_gyro[axis_id].amplitude_limit_to_accept_as_step = MPU9250_PARAMETERS_gyro_spike_amplitude_limit_to_accept_as_step_rad_s[axis_id];
  }

  C2A_MATH_ERROR filter_setting_result_three_axis = C2A_MATH_ERROR_OK;
  for (uint8_t axis_id = 0; axis_id < PHYSICAL_CONST_THREE_DIM; axis_id++)
  {
    C2A_MATH_ERROR filter_setting_result =
    SPIKE_FILTER_init(&APP_MPU9250_FILTER_spike_gyro_[axis_id],
                      mpu9250_filter_.spike_filter_config_gyro[axis_id]);

    if (filter_setting_result != C2A_MATH_ERROR_OK)
    {
      filter_setting_result_three_axis = filter_setting_result;
    }
  }

  if (filter_setting_result_three_axis != C2A_MATH_ERROR_OK) return -1;

  return 0;
}


CCP_CmdRet Cmd_APP_MPU9250_FILTER_SET_ZFILTER_PARAM(const CommonCmdPacket* packet)
{
  const uint8_t* param = CCP_get_param_head(packet);
  size_t read_out_offset = 0;

  MPU9250_FILTER_SENSOR_ID sensor_id = (MPU9250_FILTER_SENSOR_ID)(param[0]);
  if (sensor_id >= MPU9250_FILTER_SENSOR_ID_MAX) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
  read_out_offset++;

  uint8_t axis_id = param[1];
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
  switch (sensor_id)
  {
  case MPU9250_FILTER_SENSOR_ID_MAG:
    init_error = Z_FILTER_init_as_lpf_1st(&APP_MPU9250_FILTER_lpf_1st_mag_[axis_id], sampling_freq_Hz, cut_off_feq_Hz);

    if (init_error == C2A_MATH_ERROR_OK)
    {
      mpu9250_filter_.cut_off_freq_lpf_1st_mag_Hz[axis_id] = cut_off_feq_Hz;
      mpu9250_filter_.sampling_freq_Hz = sampling_freq_Hz;
      return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
    }
    else
    {
      // in this case, filter setting is already overwritten as through due to numerical error
      mpu9250_filter_.cut_off_freq_lpf_1st_mag_Hz[axis_id] = 0.0f;
      mpu9250_filter_.sampling_freq_Hz = 0.0f;
      return CCP_make_cmd_ret(CCP_EXEC_ILLEGAL_CONTEXT, init_error);
    }
    break;
  case MPU9250_FILTER_SENSOR_ID_GYRO:
    init_error = Z_FILTER_init_as_lpf_1st(&APP_MPU9250_FILTER_lpf_1st_gyro_[axis_id], sampling_freq_Hz, cut_off_feq_Hz);

    if (init_error == C2A_MATH_ERROR_OK)
    {
      mpu9250_filter_.cut_off_freq_lpf_1st_gyro_Hz[axis_id] = cut_off_feq_Hz;
      mpu9250_filter_.sampling_freq_Hz = sampling_freq_Hz;
      return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
    }
    else
    {
      // in this case, filter setting is already overwritten as through due to numerical error
      mpu9250_filter_.cut_off_freq_lpf_1st_gyro_Hz[axis_id] = 0.0f;
      mpu9250_filter_.sampling_freq_Hz = 0.0f;
      return CCP_make_cmd_ret(CCP_EXEC_ILLEGAL_CONTEXT, init_error);
    }
    break;
  default:
    break;
  }

  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}


CCP_CmdRet Cmd_APP_MPU9250_FILTER_SET_SPIKE_FILTER_PARAM(const CommonCmdPacket* packet)
{
  const uint8_t* param = CCP_get_param_head(packet);
  size_t read_out_offset = 0;

  MPU9250_FILTER_SENSOR_ID sensor_id = (MPU9250_FILTER_SENSOR_ID)(param[0]);
  if (sensor_id >= MPU9250_FILTER_SENSOR_ID_MAX) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
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
  switch (sensor_id)
  {
  case MPU9250_FILTER_SENSOR_ID_MAG:
    init_error = SPIKE_FILTER_init(&APP_MPU9250_FILTER_spike_mag_[axis_id], config_recieved);

    if (init_error == C2A_MATH_ERROR_OK)
    {
      mpu9250_filter_.spike_filter_config_mag[axis_id] = config_recieved;
      return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
    }
    else
    {
      return CCP_make_cmd_ret(CCP_EXEC_ILLEGAL_CONTEXT, init_error);
    }
    break;
  case MPU9250_FILTER_SENSOR_ID_GYRO:
    init_error = SPIKE_FILTER_init(&APP_MPU9250_FILTER_spike_gyro_[axis_id], config_recieved);

    if (init_error == C2A_MATH_ERROR_OK)
    {
      mpu9250_filter_.spike_filter_config_gyro[axis_id] = config_recieved;
      return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
    }
    else
    {
      return CCP_make_cmd_ret(CCP_EXEC_ILLEGAL_CONTEXT, init_error);
    }
    break;
  default:
    break;
  }

  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_CONTEXT);
}


CCP_CmdRet Cmd_APP_MPU9250_FILTER_RESET_ZFILTER(const CommonCmdPacket* packet)
{
  const uint8_t* param = CCP_get_param_head(packet);

  MPU9250_FILTER_SENSOR_ID sensor_id = (MPU9250_FILTER_SENSOR_ID)(param[0]);
  if (sensor_id >= MPU9250_FILTER_SENSOR_ID_MAX) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);

  uint8_t axis_id = param[1];
  if (axis_id >= PHYSICAL_CONST_THREE_DIM) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);

  switch (sensor_id)
  {
  case MPU9250_FILTER_SENSOR_ID_MAG:
    Z_FILTER_reset(&APP_MPU9250_FILTER_lpf_1st_mag_[axis_id]);
    return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
  case MPU9250_FILTER_SENSOR_ID_GYRO:
    Z_FILTER_reset(&APP_MPU9250_FILTER_lpf_1st_gyro_[axis_id]);
    return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
  default:
    break;
  }

  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_CONTEXT);
}


CCP_CmdRet Cmd_APP_MPU9250_FILTER_RESET_SPIKE_FILTER(const CommonCmdPacket* packet)
{
  const uint8_t* param = CCP_get_param_head(packet);

  MPU9250_FILTER_SENSOR_ID sensor_id = (MPU9250_FILTER_SENSOR_ID)(param[0]);
  if (sensor_id >= MPU9250_FILTER_SENSOR_ID_MAX) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);

  uint8_t axis_id = param[1];
  if (axis_id >= PHYSICAL_CONST_THREE_DIM) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);

  switch (sensor_id)
  {
  case MPU9250_FILTER_SENSOR_ID_MAG:
    SPIKE_FILTER_reset(&APP_MPU9250_FILTER_spike_mag_[axis_id]);
    return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
  case MPU9250_FILTER_SENSOR_ID_GYRO:
    SPIKE_FILTER_reset(&APP_MPU9250_FILTER_spike_gyro_[axis_id]);
    return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
  default:
    break;
  }

  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_CONTEXT);
}

#pragma section
