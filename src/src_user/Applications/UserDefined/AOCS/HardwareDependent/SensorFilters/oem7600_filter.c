#pragma section REPRO
/**
* @file   oem7600_filter.c
* @brief  OEM7600出力フィルタ
*/

#include "oem7600_filter.h"

#include <src_core/TlmCmd/common_cmd_packet_util.h>
#include <src_core/Library/print.h>
#include <src_core/System/EventManager/event_logger.h>
#include "../../../../DriverInstances/di_oem7600.h"
#include "../../aocs_manager.h"
#include "../../../../../Library/vector3.h"
#include "../../../../../Library/matrix33.h"
#include "../../../../../Library/time_space.h"
#include "../../../../../Library/math_constants.h"

#define OEM7600_FILTER_POS_VEL_SIX_DIM (6)

static Oem7600Filter        oem7600_filter_;
const  Oem7600Filter* const oem7600_filter = &oem7600_filter_;

static SpikeFilter APP_OEM7600_FILTER_position_spike_[PHYSICAL_CONST_THREE_DIM];       //!< スパイク除去フィルタ for 衛星位置
static SpikeFilter APP_OEM7600_FILTER_velocity_spike_[PHYSICAL_CONST_THREE_DIM];       //!< スパイク除去フィルタ for 衛星速度

static void APP_OEM7600_FILTER_init_(void);
static void APP_OEM7600_FILTER_exec_(void);

static int APP_OEM7600_FILTER_init_spike_filter_(void);

AppInfo APP_OEM7600_FILTER_create_app(void)
{
  return AI_create_app_info("Oem7600Filter", APP_OEM7600_FILTER_init_, APP_OEM7600_FILTER_exec_);
}

static void APP_OEM7600_FILTER_init_(void)
{
  VECTOR3_initialize_double(oem7600_filter_.sat_pos_filtered_ecef_m,   0.0);
  VECTOR3_initialize_double(oem7600_filter_.sat_pos_filtered_eci_m,    0.0);
  VECTOR3_initialize_double(oem7600_filter_.sat_vel_filtered_ecef_m_s, 0.0);
  VECTOR3_initialize_double(oem7600_filter_.sat_vel_filtered_eci_m_s,  0.0);

  for (int i = 0; i < PHYSICAL_CONST_THREE_DIM; i++)
  {
    oem7600_filter_.position_filter_error[i]     = C2A_MATH_ERROR_OK;
    oem7600_filter_.velocity_filter_error[i]     = C2A_MATH_ERROR_OK;
  }

  int spike_filter_init_result = APP_OEM7600_FILTER_init_spike_filter_();
  if (spike_filter_init_result != 0)
  {
    Printf("OEM7600 SpikeFilter init Failed ! \n");
  }

  return;
}

static void APP_OEM7600_FILTER_exec_(void)
{
  if (aocs_manager->gps_visibility == AOCS_MANAGER_GPS_INVISIBILE) return;

  for (uint8_t axis_id = 0; axis_id < PHYSICAL_CONST_THREE_DIM; axis_id++)
  {
    oem7600_filter_.position_filter_error[axis_id] =
    SPIKE_FILTER_calc_output_double(&APP_OEM7600_FILTER_position_spike_[axis_id],
                                    &oem7600_filter_.sat_pos_filtered_ecef_m[axis_id],
                                    aocs_manager->sat_pos_obs_ecef_m[axis_id]);

    // record to EL as EL_ERROR_LEVEL_HIGH if spike noise are removed a configured number of times straight
    if (oem7600_filter_.position_filter_error[axis_id] == C2A_MATH_ERROR_OTHERS)
    {
      EL_record_event(EL_GROUP_ERROR_OEM7600, (uint32_t)axis_id, EL_ERROR_LEVEL_HIGH, (uint32_t)OEM7600_EL_NOTE_NOISE_REMOVED);
    }
    else if (oem7600_filter_.position_filter_error[axis_id] == C2A_MATH_ERROR_RANGE_OVER)
    {
      // TODO_L: ここでのEL登録は過剰なのでコメントアウトするが、復活させるか検討してもよい
      // EL_record_event(EL_GROUP_ERROR_OEM7600, (uint32_t)axis_id, EL_ERROR_LEVEL_LOW, (uint32_t)OEM7600_EL_NOTE_NOISE_REMOVED);
    }
    else
    {
      // 何もしない
    }

    oem7600_filter_.velocity_filter_error[axis_id] =
    SPIKE_FILTER_calc_output_double(&APP_OEM7600_FILTER_velocity_spike_[axis_id],
                                    &oem7600_filter_.sat_vel_filtered_ecef_m_s[axis_id],
                                    aocs_manager->sat_vel_obs_ecef_m_s[axis_id]);
  }

  // TODO_L: filter_errorが出た時の対処は要検討
  AOCS_MANAGER_set_sat_pos_vel_obs_ecef_m(oem7600_filter_.sat_pos_filtered_ecef_m, oem7600_filter_.sat_vel_filtered_ecef_m_s);

  // EKF用の座標変換
  // ここではセンサ取得時のECEFをECIに戻すので，obsの時刻で座標変換行列を計算する
  float offset_time_s = 0.0f;
  double reference_jday = TIME_SPACE_convert_gpstime_to_julian_day(aocs_manager->current_gps_time, offset_time_s);
  double ref_gmst_rad = TIME_SPACE_calc_gmst_rad(reference_jday);

  double dcm_eci_to_ecef[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM];
  C2A_MATH_ERROR error = TIME_SPACE_trans_eci_to_ecef(reference_jday, ref_gmst_rad, dcm_eci_to_ecef);
  if (error != C2A_MATH_ERROR_OK)
  {
    error = TIME_SPACE_trans_eci_to_ecef_rotation_only(ref_gmst_rad, dcm_eci_to_ecef);
  }

  double dcm_ecef_to_eci[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM];
  MATRIX33_transpose_double(dcm_ecef_to_eci, dcm_eci_to_ecef);

  TIME_SPACE_convert_posvel_ecef_to_eci(oem7600_filter_.sat_pos_filtered_eci_m, oem7600_filter_.sat_vel_filtered_eci_m_s,
                                        dcm_ecef_to_eci, oem7600_filter_.sat_pos_filtered_ecef_m, oem7600_filter_.sat_vel_filtered_ecef_m_s);

  AOCS_MANAGER_set_sat_pos_vel_obs_eci_m(oem7600_filter_.sat_pos_filtered_eci_m, oem7600_filter_.sat_vel_filtered_eci_m_s);

  return;
}

static int APP_OEM7600_FILTER_init_spike_filter_(void)
{
  // 値は調整してよいが一旦これで進める
  for (uint8_t axis_id = 0; axis_id < PHYSICAL_CONST_THREE_DIM; axis_id++)
  {
    oem7600_filter_.position_spike_filter_config[axis_id].count_limit_to_accept = 3;
    oem7600_filter_.position_spike_filter_config[axis_id].count_limit_to_reject_continued_warning = 60;
    oem7600_filter_.position_spike_filter_config[axis_id].reject_threshold = 10000.0; // m
    oem7600_filter_.position_spike_filter_config[axis_id].amplitude_limit_to_accept_as_step = 8000.0; // m

    oem7600_filter_.velocity_spike_filter_config[axis_id].count_limit_to_accept = 3;
    oem7600_filter_.velocity_spike_filter_config[axis_id].count_limit_to_reject_continued_warning = 60;
    oem7600_filter_.velocity_spike_filter_config[axis_id].reject_threshold = 1000.0; // m/s
    oem7600_filter_.velocity_spike_filter_config[axis_id].amplitude_limit_to_accept_as_step = 100; // m/s
  }

  C2A_MATH_ERROR position_filter_setting_result = C2A_MATH_ERROR_OK;
  for (uint8_t axis_id = 0; axis_id < PHYSICAL_CONST_THREE_DIM; axis_id++)
  {
    C2A_MATH_ERROR filter_setting_result =
    SPIKE_FILTER_init(&APP_OEM7600_FILTER_position_spike_[axis_id],
                      oem7600_filter_.position_spike_filter_config[axis_id]);

    if (filter_setting_result != C2A_MATH_ERROR_OK)
    {
      position_filter_setting_result = filter_setting_result;
    }
  }

  C2A_MATH_ERROR velocity_filter_setting_result = C2A_MATH_ERROR_OK;
  for (uint8_t axis_id = 0; axis_id < PHYSICAL_CONST_THREE_DIM; axis_id++)
  {
    C2A_MATH_ERROR filter_setting_result =
    SPIKE_FILTER_init(&APP_OEM7600_FILTER_velocity_spike_[axis_id],
                      oem7600_filter_.velocity_spike_filter_config[axis_id]);

    if (filter_setting_result != C2A_MATH_ERROR_OK)
    {
      velocity_filter_setting_result = filter_setting_result;
    }
  }

  // TODO_L: position_filter_settingとvelocity_filter_settingのどちらでエラーが出たかを区別するか要検討
  if ((position_filter_setting_result != C2A_MATH_ERROR_OK) ||
      (velocity_filter_setting_result != C2A_MATH_ERROR_OK))
  {
    return -1;
  }

  return 0;
}

CCP_CmdRet Cmd_APP_OEM7600_FILTER_SET_SPIKE_FILTER_PARAM(const CommonCmdPacket* packet)
{
  const uint8_t* param = CCP_get_param_head(packet);
  size_t read_out_offset = 0;

  OEM7600_IDX sensor_id = (OEM7600_IDX)(param[read_out_offset]);
  if (sensor_id >= OEM7600_IDX_MAX) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
  read_out_offset++;

  uint8_t axis_id = param[read_out_offset];
  if (axis_id >= OEM7600_FILTER_POS_VEL_SIX_DIM) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
  read_out_offset++;

  SpikeFilter_Config config_received;
  config_received.count_limit_to_accept = param[read_out_offset];
  read_out_offset++;

  config_received.count_limit_to_reject_continued_warning = param[read_out_offset];
  read_out_offset++;

  float reject_threshold_float;
  ENDIAN_memcpy(&reject_threshold_float, param + read_out_offset, sizeof(float));
  if (reject_threshold_float < 0.0f) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
  config_received.reject_threshold = (double)(reject_threshold_float);
  read_out_offset += sizeof(float);

  float amplitude_limit_to_accept_as_step_float;
  ENDIAN_memcpy(&amplitude_limit_to_accept_as_step_float, param + read_out_offset, sizeof(float));
  if (amplitude_limit_to_accept_as_step_float < 0.0f) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
  config_received.amplitude_limit_to_accept_as_step = (double)(amplitude_limit_to_accept_as_step_float);

  C2A_MATH_ERROR init_error;

  if (axis_id < PHYSICAL_CONST_THREE_DIM)
  {
    init_error = SPIKE_FILTER_init(&APP_OEM7600_FILTER_position_spike_[axis_id], config_received);

    // 初期化に成功した場合のみ，oem7600_filterのconfigステータスを変更
    if (init_error == C2A_MATH_ERROR_OK)
    {
      oem7600_filter_.position_spike_filter_config[axis_id] = config_received;
      return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
    }
  }
  else
  {
    uint8_t axis_id_vel = axis_id - PHYSICAL_CONST_THREE_DIM;
    init_error = SPIKE_FILTER_init(&APP_OEM7600_FILTER_velocity_spike_[axis_id_vel], config_received);

    if (init_error == C2A_MATH_ERROR_OK)
    {
      oem7600_filter_.velocity_spike_filter_config[axis_id_vel] = config_received;
      return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
    }
  }

  return CCP_make_cmd_ret(CCP_EXEC_ILLEGAL_CONTEXT, init_error);
}

CCP_CmdRet Cmd_APP_OEM7600_FILTER_RESET_SPIKE_FILTER(const CommonCmdPacket* packet)
{
  const uint8_t* param = CCP_get_param_head(packet);
  size_t read_out_offset = 0;

  OEM7600_IDX sensor_id = (OEM7600_IDX)(param[read_out_offset]);
  if (sensor_id >= OEM7600_IDX_MAX) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
  read_out_offset++;

  uint8_t axis_id = param[read_out_offset];
  if (axis_id >= OEM7600_FILTER_POS_VEL_SIX_DIM) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);

  if (axis_id < PHYSICAL_CONST_THREE_DIM)
  {
    SPIKE_FILTER_reset(&APP_OEM7600_FILTER_position_spike_[axis_id]);
  }
  else
  {
    uint8_t axis_id_vel = axis_id - PHYSICAL_CONST_THREE_DIM;
    SPIKE_FILTER_reset(&APP_OEM7600_FILTER_velocity_spike_[axis_id_vel]);
  }

  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

#pragma section
