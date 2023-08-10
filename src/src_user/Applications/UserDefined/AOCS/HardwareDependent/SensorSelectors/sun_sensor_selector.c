#pragma section REPRO
/**
* @file   sun_sensor_selector.c
* @brief  複数あるサンセンサ出力を組み合わせて一つの太陽方向ベクトルを計算するアプリ
*/

#include "sun_sensor_selector.h"

#include <src_core/Library/print.h>
#include <src_core/TlmCmd/common_cmd_packet_util.h>
#include <src_core/TlmCmd/packet_handler.h>
#include <src_user/Applications/DriverInstances/di_nanossoc_d60.h>
#include <src_user/Applications/UserDefined/AOCS/aocs_manager.h>

// Satellite Parameters
#include <src_user/Settings/SatelliteParameters/nanossoc_d60_parameters.h>

static SunSensorSelector        sun_sensor_selector_;
const  SunSensorSelector* const sun_sensor_selector = &sun_sensor_selector_;

/**
 * @brief 利用可能なサンセンサの配列
 * @note  固定長配列なので，余った部分にはMAXを入れておく
 * @note  この配列内では，サンセンサの受光強度の順番に並べられているわけではない
 */
static NANOSSOC_D60_IDX APP_SS_SELECTOR_available_sun_sensor_list_[NANOSSOC_D60_PARAMETERS_NUMBER_OF_MOUNTED_SENSOR];

/*
 * @brief 利用可能なサンセンサのインデックスが受光強度順に並べられた配列
 * @note  固定長配列なので，余った部分にはMAXを入れておく
 */
static NANOSSOC_D60_IDX APP_SS_SELECTOR_sun_sensor_intensity_ranking_[NANOSSOC_D60_PARAMETERS_NUMBER_OF_MOUNTED_SENSOR];
static int APP_SS_SELECTOR_num_of_available_sun_sensor_;

static void APP_SS_SELECTOR_init_(void);
static void APP_SS_SELECTOR_exec_(void);

static APP_SS_SELECTOR_AVAILABLE APP_SS_SELECTOR_check_availability_(NANOSSOC_D60_IDX idx);
static void APP_SS_SELECTOR_generate_available_list_(void);
static void APP_SS_SELECTOR_fusion_(void);

AppInfo APP_SS_SELECTOR_create_app(void)
{
  return AI_create_app_info("SunSensorSelector", APP_SS_SELECTOR_init_, APP_SS_SELECTOR_exec_);
}

static void APP_SS_SELECTOR_init_(void)
{
  sun_sensor_selector_.sun_intensity_lower_threshold_percent = NANOSSOC_D60_PARAMETERS_sun_intensity_lower_threshold_percent;
  sun_sensor_selector_.sun_intensity_upper_threshold_percent = NANOSSOC_D60_PARAMETERS_sun_intensity_upper_threshold_percent;
  for (int idx = 0; idx < NANOSSOC_D60_PARAMETERS_NUMBER_OF_MOUNTED_SENSOR; idx++)
  {
    APP_SS_SELECTOR_available_sun_sensor_list_[idx] = (NANOSSOC_D60_IDX)NANOSSOC_D60_PARAMETERS_NUMBER_OF_MOUNTED_SENSOR;
    APP_SS_SELECTOR_sun_sensor_intensity_ranking_[idx] = (NANOSSOC_D60_IDX)NANOSSOC_D60_PARAMETERS_NUMBER_OF_MOUNTED_SENSOR;
  }
  APP_SS_SELECTOR_num_of_available_sun_sensor_ = 0;
  return;
}

static void APP_SS_SELECTOR_exec_(void)
{
  APP_SS_SELECTOR_generate_available_list_();

  if (APP_SS_SELECTOR_num_of_available_sun_sensor_ == 0)
  {
    // どのサンセンサからも太陽が見えていないときは，ひとまず最後の太陽方向ベクトルを保持するために，サンセンサセレクタからは太陽方向ベクトルの更新をしない
    AOCS_MANAGER_set_sun_visibility(AOCS_MANAGER_SUN_INVISIBILE);
    return;
  }

  // 太陽が見えているサンセンサが存在するときは，最も受光強度が高いサンセンサの太陽方向ベクトルを採用する
  NANOSSOC_D60_IDX max_intensity_idx = (NANOSSOC_D60_IDX)NANOSSOC_D60_PARAMETERS_NUMBER_OF_MOUNTED_SENSOR;
  float max_intensity = 0.0;

  for (int idx = 0; idx < APP_SS_SELECTOR_num_of_available_sun_sensor_; idx++)
  {
    if (nanossoc_d60_driver[APP_SS_SELECTOR_available_sun_sensor_list_[idx]]->info.sun_intensity_percent > max_intensity)
    {
      max_intensity_idx = APP_SS_SELECTOR_available_sun_sensor_list_[idx];
      APP_SS_SELECTOR_sun_sensor_intensity_ranking_[0] = max_intensity_idx;
      max_intensity = nanossoc_d60_driver[APP_SS_SELECTOR_available_sun_sensor_list_[idx]]->info.sun_intensity_percent;
    }
  }
  AOCS_MANAGER_set_sun_vec_obs_body(nanossoc_d60_driver[max_intensity_idx]->info.sun_vec_body);
  AOCS_MANAGER_set_sun_visibility(AOCS_MANAGER_SUN_VISIBLE);

  // TODO_L: 複数のサンセンサの出力を合成して太陽方向ベクトルをセットする処理（fusion）も実装する
  return;
}

static void APP_SS_SELECTOR_generate_available_list_(void)
{
  for (int idx = 0; idx < NANOSSOC_D60_PARAMETERS_NUMBER_OF_MOUNTED_SENSOR; idx++)
  {
    APP_SS_SELECTOR_available_sun_sensor_list_[idx] = (NANOSSOC_D60_IDX)NANOSSOC_D60_PARAMETERS_NUMBER_OF_MOUNTED_SENSOR;
  }
  APP_SS_SELECTOR_num_of_available_sun_sensor_ = 0;

  for (int idx = 0; idx < NANOSSOC_D60_PARAMETERS_NUMBER_OF_MOUNTED_SENSOR; idx++)
  {
    if (APP_SS_SELECTOR_check_availability_((NANOSSOC_D60_IDX)idx) == APP_SS_SELECTOR_AVAILABLE_OK)
    {
      APP_SS_SELECTOR_available_sun_sensor_list_[APP_SS_SELECTOR_num_of_available_sun_sensor_] = (NANOSSOC_D60_IDX)idx;
      APP_SS_SELECTOR_num_of_available_sun_sensor_++;
    }
  }
}

static APP_SS_SELECTOR_AVAILABLE APP_SS_SELECTOR_check_availability_(NANOSSOC_D60_IDX idx)
{
  float sun_intensity_percent = nanossoc_d60_driver[idx]->info.sun_intensity_percent;

  if (nanossoc_d60_driver[idx]->info.checksum_state != NANOSSOC_D60_CHECKSUM_STATE_OK)
  {
    return APP_SS_SELECTOR_AVAILABLE_NG;
  }
  else if (sun_intensity_percent < sun_sensor_selector->sun_intensity_lower_threshold_percent)
  {
    return APP_SS_SELECTOR_AVAILABLE_NG;
  }
  else if (sun_intensity_percent > sun_sensor_selector->sun_intensity_upper_threshold_percent)
  {
    return APP_SS_SELECTOR_AVAILABLE_NG;
  }
  else
  {
    return APP_SS_SELECTOR_AVAILABLE_OK;
  }
}

static void APP_SS_SELECTOR_fusion_(void)
{
  // TODO_L 実装する．
  // available_sun_sensor_listをソートしてsun_sensor_intensity_rankingにする処理などもここで行う
}

CCP_CmdRet Cmd_SUN_SENSOR_SELECTOR_SET_INTENSITY_THRESHOLD(const CommonCmdPacket* packet)
{
  size_t arg_num = 0;
  float intensity_lower_threshold_percent = CCP_get_param_from_packet(packet, arg_num, float);
  arg_num++;
  float intensity_upper_threshold_percent = CCP_get_param_from_packet(packet, arg_num, float);

  if (intensity_lower_threshold_percent < 0.0f || intensity_upper_threshold_percent < 0.0f)
  {
    return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
  }

  if (intensity_lower_threshold_percent >= intensity_upper_threshold_percent)
  {
    return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_CONTEXT);
  }

  const float threshold_upper_limit_percent = 1000.0f;
  if (intensity_lower_threshold_percent > threshold_upper_limit_percent ||
    intensity_upper_threshold_percent > threshold_upper_limit_percent)
  {
    return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_CONTEXT);
  }

  sun_sensor_selector_.sun_intensity_lower_threshold_percent = intensity_lower_threshold_percent;
  sun_sensor_selector_.sun_intensity_upper_threshold_percent = intensity_upper_threshold_percent;

  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

#pragma section
