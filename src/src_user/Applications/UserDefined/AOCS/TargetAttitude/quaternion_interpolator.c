#pragma section REPRO
/**
* @file   quaternion_interpolator.c
* @brief  目標Quaternionを内挿計算するアプリ
*/

#include <src_core/TlmCmd/common_cmd_packet_util.h>
#include <src_core/System/EventManager/event_logger.h>
#include "../../../app_registry.h"

#include "quaternion_interpolator.h"
#include "../aocs_manager.h"

static QuaternionInterpolator        quaternion_interpolator_;
const  QuaternionInterpolator* const quaternion_interpolator = &quaternion_interpolator_;

/**
 * @enum   APP_QI_SET_PREV_TARGET_TYPE
 * @brief  Quaternion内挿計算の始点の設定方法を指定する
 * @note   uint8_tを想定
 */
typedef enum
{
  APP_QI_SET_PREV_TARGET_TYPE_CURRENT_ATTITUDE = 0, //!< 現在の推定QuaternionとTIを使う．コマンドの第2引数以降は無視される
  APP_QI_SET_PREV_TARGET_TYPE_ARBITRARY_ATTITUDE,   //!< コマンド指定のQuaternionとTIを使う．コマンドの第2引数以降で指定する
  APP_QI_SET_PREV_TARGET_TYPE_MAX
} APP_QI_SET_PREV_TARGET_TYPE;

static void APP_QI_init_(void);
static void APP_QI_exec_(void);


AppInfo APP_QI_create_app(void)
{
  return AI_create_app_info("QuaternionInterpolator", APP_QI_init_, APP_QI_exec_);
}


static void APP_QI_init_(void)
{
  quaternion_interpolator_.quaternion_target_i2t = QUATERNION_make_unit();
  quaternion_interpolator_.is_enabled = 0;
  for (int i = 0; i < APP_QI_TARGET_MAX; i++)
  {
    quaternion_interpolator_.target_list[i].attitude_changed_ti = 0;
    quaternion_interpolator_.target_list[i].quaternion_target_i2t = QUATERNION_make_unit();
  }
  quaternion_interpolator_.current_target_num = 0;
  quaternion_interpolator_.index = 0;
  quaternion_interpolator_.previous_attitude_changed_ti = 0;
  quaternion_interpolator_.previous_quaternion_target_i2t = QUATERNION_make_unit();
}


static void APP_QI_exec_(void)
{
  cycle_t current_ti = TMGR_get_master_total_cycle();
  uint8_t current_target_num = quaternion_interpolator_.current_target_num;
  const uint8_t index = quaternion_interpolator_.index;
  const cycle_t next_attitude_changed_ti = quaternion_interpolator_.target_list[index].attitude_changed_ti;
  const cycle_t previous_attitude_changed_ti = quaternion_interpolator_.previous_attitude_changed_ti;
  const Quaternion next_quaternion_i2t = quaternion_interpolator_.target_list[index].quaternion_target_i2t;
  const Quaternion previous_quaternion_i2t = quaternion_interpolator_.previous_quaternion_target_i2t;

  if (index >= current_target_num)
  {
    // すべての姿勢変更が完了しているため何もしない
    return;
  }

  if (current_ti < previous_attitude_changed_ti)
  {
    // TODO_L: noteについて整理する
    EL_record_event(EL_GROUP_CALCULATION_ERROR, (uint32_t)AR_APP_QUATERNION_INTERPOLATOR, EL_ERROR_LEVEL_LOW, 0);
    return;
  }

  if (next_attitude_changed_ti <= previous_attitude_changed_ti)
  {
    // TODO_L: noteについて整理する
    EL_record_event(EL_GROUP_CALCULATION_ERROR, (uint32_t)AR_APP_QUATERNION_INTERPOLATOR, EL_ERROR_LEVEL_LOW, 1);
    return;
  }

  if (current_ti < next_attitude_changed_ti)
  {
    cycle_t diff_ti = next_attitude_changed_ti - previous_attitude_changed_ti;
    float factor = ((float)(current_ti - previous_attitude_changed_ti) / (float)diff_ti);
    Quaternion interpolated_quaternion_i2t = QUATERNION_interpolate_slerp(previous_quaternion_i2t,
                                                                          next_quaternion_i2t,
                                                                          factor);
    quaternion_interpolator_.quaternion_target_i2t = interpolated_quaternion_i2t;
  }
  else
  {
    // 姿勢変更が完了したので次の姿勢変更に進む
    quaternion_interpolator_.index++;
    quaternion_interpolator_.previous_attitude_changed_ti = next_attitude_changed_ti;
    quaternion_interpolator_.previous_quaternion_target_i2t = next_quaternion_i2t;
  }

  if (quaternion_interpolator_.is_enabled)
  {
    AOCS_MANAGER_set_quaternion_target_i2t(quaternion_interpolator_.quaternion_target_i2t);
  }

  return;
}


CCP_EXEC_STS Cmd_APP_QI_SET_NEXT_TARGET_QUATERNION(const CommonCmdPacket* packet)
{
  C2A_MATH_ERROR error;
  float quaternion_target_i2t_array[PHYSICAL_CONST_QUATERNION_DIM] = {0.0f, 0.0f, 0.0f, 1.0f};
  Quaternion quaternion_target_i2t;
  uint8_t current_target_num = quaternion_interpolator_.current_target_num;

  cycle_t attitude_changed_ti = (cycle_t)CCP_get_param_from_packet(packet, 0, uint32_t);
  if (current_target_num > 0)
  {
    // 今回セットした姿勢変更完了時刻が，現時刻より前の場合エラー
    cycle_t current_ti = TMGR_get_master_total_cycle();
    if (attitude_changed_ti <= current_ti)
    {
      return CCP_EXEC_ILLEGAL_CONTEXT;
    }
    // 今回セットした姿勢変更完了時刻が，直前の姿勢変更完了時刻より前の場合エラー
    cycle_t previous_attitude_changed_ti = quaternion_interpolator_.target_list[current_target_num - 1].attitude_changed_ti;
    if (attitude_changed_ti <= previous_attitude_changed_ti)
    {
      return CCP_EXEC_ILLEGAL_CONTEXT;
    }
  }

  for (int i = 0; i < PHYSICAL_CONST_QUATERNION_DIM; i++)
  {
    quaternion_target_i2t_array[i] = CCP_get_param_from_packet(packet, 1 + i, float);
  }
  error = QUATERNION_make_from_array(&quaternion_target_i2t, quaternion_target_i2t_array, QUATERNION_SCALAR_POSITION_LAST);
  if (error != C2A_MATH_ERROR_OK)
  {
    return CCP_EXEC_ILLEGAL_PARAMETER;
  }

  quaternion_interpolator_.target_list[current_target_num].attitude_changed_ti = attitude_changed_ti;
  quaternion_interpolator_.target_list[current_target_num].quaternion_target_i2t = quaternion_target_i2t;
  quaternion_interpolator_.current_target_num++;
  return CCP_EXEC_SUCCESS;
}

CCP_EXEC_STS Cmd_APP_QI_SET_NEXT_TARGET_QUATERNION_JDAY(const CommonCmdPacket* packet)
{
  uint8_t arg_idx = 0;
  C2A_MATH_ERROR error;
  float quaternion_target_i2t_array[PHYSICAL_CONST_QUATERNION_DIM] = {0.0f, 0.0f, 0.0f, 1.0f};
  Quaternion quaternion_target_i2t;
  uint8_t current_target_num = quaternion_interpolator_.current_target_num;

  double attitude_changed_jday = (double)CCP_get_param_from_packet(packet, arg_idx, double);
  arg_idx++;
  double diff_sec = (attitude_changed_jday - aocs_manager->reference_jday) * PHYSICAL_CONST_EARTH_SOLAR_DAY_s;
  cycle_t diff_ti = (cycle_t)(diff_sec) * OBCT_CYCLES_PER_SEC;
  cycle_t attitude_changed_ti = diff_ti + aocs_manager->obct_reference.total_cycle;
  if (current_target_num > 0)
  {
    // 今回セットした姿勢変更完了時刻が，現時刻より前の場合エラー
    cycle_t current_ti = TMGR_get_master_total_cycle();
    if (attitude_changed_ti <= current_ti)
    {
      return CCP_EXEC_ILLEGAL_CONTEXT;
    }
    // 今回セットした姿勢変更完了時刻が，直前の姿勢変更完了時刻より前の場合エラー
    cycle_t previous_attitude_changed_ti = quaternion_interpolator_.target_list[current_target_num - 1].attitude_changed_ti;
    if (attitude_changed_ti <= previous_attitude_changed_ti)
    {
      return CCP_EXEC_ILLEGAL_CONTEXT;
    }
  }

  for (int i = 0; i < PHYSICAL_CONST_QUATERNION_DIM; i++)
  {
    quaternion_target_i2t_array[i] = CCP_get_param_from_packet(packet, arg_idx, float);
    arg_idx++;
  }
  error = QUATERNION_make_from_array(&quaternion_target_i2t, quaternion_target_i2t_array, QUATERNION_SCALAR_POSITION_LAST);
  if (error != C2A_MATH_ERROR_OK)
  {
    return CCP_EXEC_ILLEGAL_PARAMETER;
  }

  quaternion_interpolator_.target_list[current_target_num].attitude_changed_ti = attitude_changed_ti;
  quaternion_interpolator_.target_list[current_target_num].quaternion_target_i2t = quaternion_target_i2t;
  quaternion_interpolator_.current_target_num++;
  return CCP_EXEC_SUCCESS;
}

CCP_EXEC_STS Cmd_APP_QI_SET_PREV_TARGET_QUATERNION(const CommonCmdPacket* packet)
{
  C2A_MATH_ERROR error;
  float quaternion_target_i2t_array[PHYSICAL_CONST_QUATERNION_DIM] = {0.0f, 0.0f, 0.0f, 1.0f};
  Quaternion quaternion_target_i2t;
  cycle_t current_ti = TMGR_get_master_total_cycle();

  uint8_t set_type = CCP_get_param_from_packet(packet, 0, uint8_t);
  if (set_type >= APP_QI_SET_PREV_TARGET_TYPE_MAX)
  {
    return CCP_EXEC_ILLEGAL_PARAMETER;
  }
  else if (set_type == APP_QI_SET_PREV_TARGET_TYPE_CURRENT_ATTITUDE)
  {
    // 現在のTIと推定Quaternionをセット
    quaternion_interpolator_.previous_attitude_changed_ti = current_ti;
    quaternion_interpolator_.previous_quaternion_target_i2t = aocs_manager->quaternion_est_i2b;
    return CCP_EXEC_SUCCESS;
  }
  else if (set_type == APP_QI_SET_PREV_TARGET_TYPE_ARBITRARY_ATTITUDE)
  {
    // コマンド指定のTIとQuaternionをセット
    cycle_t attitude_changed_ti = (cycle_t)CCP_get_param_from_packet(packet, 1, uint32_t);
    // コマンド指定のTIが現在のTIより未来の場合はエラー
    if (attitude_changed_ti > current_ti)
    {
      return CCP_EXEC_ILLEGAL_CONTEXT;
    }

    for (int i = 0; i < PHYSICAL_CONST_QUATERNION_DIM; i++)
    {
      quaternion_target_i2t_array[i] = CCP_get_param_from_packet(packet, 2 + i, float);
    }
    error = QUATERNION_make_from_array(&quaternion_target_i2t, quaternion_target_i2t_array, QUATERNION_SCALAR_POSITION_LAST);
    if (error != C2A_MATH_ERROR_OK)
    {
      return CCP_EXEC_ILLEGAL_PARAMETER;
    }

    quaternion_interpolator_.previous_attitude_changed_ti = attitude_changed_ti;
    quaternion_interpolator_.previous_quaternion_target_i2t = quaternion_target_i2t;
    return CCP_EXEC_SUCCESS;
  }
  else
  {
    // NOT REACHED
    return CCP_EXEC_ILLEGAL_PARAMETER;
  }
}

CCP_EXEC_STS Cmd_APP_QI_RESET_TARGET_QUATERNION(const CommonCmdPacket* packet)
{
  // 引数なし

  // 有効時はリセット禁止
  if (quaternion_interpolator_.is_enabled)
  {
    return CCP_EXEC_ILLEGAL_CONTEXT;
  }

  for (int i = 0; i < APP_QI_TARGET_MAX; i++)
  {
    quaternion_interpolator_.target_list[i].attitude_changed_ti = 0;
    quaternion_interpolator_.target_list[i].quaternion_target_i2t = QUATERNION_make_unit();
  }
  quaternion_interpolator_.current_target_num = 0;
  quaternion_interpolator_.index = 0;
  quaternion_interpolator_.previous_attitude_changed_ti = 0;
  quaternion_interpolator_.previous_quaternion_target_i2t = QUATERNION_make_unit();
  quaternion_interpolator_.is_enabled = 0;
  return CCP_EXEC_SUCCESS;
}


CCP_EXEC_STS Cmd_APP_QI_ENABLE(const CommonCmdPacket* packet)
{
  uint8_t is_enabled = CCP_get_param_from_packet(packet, 0, uint8_t);
  if (is_enabled > 1)
  {
    return CCP_EXEC_ILLEGAL_PARAMETER;
  }

  quaternion_interpolator_.is_enabled = is_enabled;

  return CCP_EXEC_SUCCESS;
}

#pragma section
