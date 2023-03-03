#pragma section REPRO
/**
* @file  aocs_data_recorder.c
* @brief データレコーダアプリ
* @note  汎用的なものではなく、特定のデータのみ保存する方針 TODO_L: 汎用化する？
*/

#include "aocs_data_recorder.h"

#include <string.h>
#include <src_core/TlmCmd/common_cmd_packet_util.h>
#include <src_core/Library/print.h>
#include <src_core/System/EventManager/event_logger.h>
#include "../../../Library/vector3.h"

#define APP_AOCS_DR_ALGORITHM_TLM_SIZE (104) //!< テレメトリ構造体サイズ(念のため実機で確認済み)

static AocsDataRecorder        aocs_data_recorder_;
const  AocsDataRecorder* const aocs_data_recorder = &aocs_data_recorder_;

static void  APP_AOCS_DR_init_(void);
static void  APP_AOCS_DR_exec_(void);

/**
 * @brief 不揮発メモリにデータを書き込む
 */
static void APP_AOCS_DR_record_data_(void);

/**
 * @brief 不揮発メモリからデータを読み出す
 */
static void APP_AOCS_DR_read_data_(void);


AppInfo APP_AOCS_DR_create_app(void)
{
  return AI_create_app_info("AOCS Data Recorder", APP_AOCS_DR_init_, APP_AOCS_DR_exec_);
}

static void APP_AOCS_DR_init_(void)
{
  memset(&aocs_data_recorder_, 0x00, sizeof(AocsDataRecorder));

  aocs_data_recorder_.write_partition_id = APP_NVM_PARTITION_ID_DR_3;
  aocs_data_recorder_.read_partition_id = APP_NVM_PARTITION_ID_DR_3;
  aocs_data_recorder_.write_end_address = non_volatile_memory_partition->elements[aocs_data_recorder_.write_partition_id].size_byte;
  aocs_data_recorder_.read_end_address = non_volatile_memory_partition->elements[aocs_data_recorder_.read_partition_id].size_byte;
  uint32_t tlm_size = sizeof(AOCS_DR_ALGORITHM_Telemetry);
  Printf("DR ALGORITHM SIZE = %d \n", tlm_size);

  return;
}

static void APP_AOCS_DR_exec_(void)
{
  if (aocs_data_recorder_.record_permission == APP_AOCS_DR_RECORD_PERMISSION_DISABLE) return;

  // データ保存
  uint32_t predicted_end_address = (aocs_data_recorder_.write_pointer + 1) * APP_AOCS_DR_ALGORITHM_TLM_SIZE;
  if (predicted_end_address > aocs_data_recorder_.write_end_address)
  {
    if (aocs_data_recorder_.overwrite_mode == APP_AOCS_DR_OVERWRITE_DISABLE)
    {
      // DR強制停止
      aocs_data_recorder_.record_permission = APP_AOCS_DR_RECORD_PERMISSION_DISABLE;
      return;
    }
    else if (aocs_data_recorder_.overwrite_mode == APP_AOCS_DR_OVERWRITE_ENABLE)
    {
      // TODO_L: 多少メモリが余ることになるので効率重視なら修正が必要
      aocs_data_recorder_.write_pointer = 0;
    }
    else
    {
      // No reach
    }
  }

  // データ書き込み
  APP_AOCS_DR_record_data_();

  return;
}

static void APP_AOCS_DR_record_data_(void)
{
  // calc address
  uint32_t write_address = aocs_data_recorder_.write_pointer * APP_AOCS_DR_ALGORITHM_TLM_SIZE;

  // TLM update
  // 時間
  ObcTime temp_obct = TMGR_get_master_clock();
  aocs_data_recorder_.write_tlm_algorithm.obct_total_cycle_recorded = temp_obct.total_cycle;
  aocs_data_recorder_.write_tlm_algorithm.reference_jday = aocs_manager->reference_jday;

  // アルゴリズムデータ
  aocs_data_recorder_.write_tlm_algorithm.quaternion_est_i2b = aocs_manager->quaternion_est_i2b;
  VECTOR3_copy(aocs_data_recorder_.write_tlm_algorithm.ang_vel_est_body_rad_s,
               aocs_manager->ang_vel_est_body_rad_s);
  VECTOR3_copy(aocs_data_recorder_.write_tlm_algorithm.sun_vec_est_body,
               aocs_manager->sun_vec_est_body);
  VECTOR3_copy(aocs_data_recorder_.write_tlm_algorithm.mag_vec_est_body_nT,
               aocs_manager->mag_vec_est_body_nT);
  VECTOR3_copy(aocs_data_recorder_.write_tlm_algorithm.internal_torque_target_body_Nm,
               aocs_manager->internal_torque_target_body_Nm);
  VECTOR3_copy(aocs_data_recorder_.write_tlm_algorithm.external_torque_target_body_Nm,
               aocs_manager->external_torque_target_body_Nm);
  VECTOR3_copy(aocs_data_recorder_.write_tlm_algorithm.rw_angular_velocity_rad_s,
               aocs_manager->rw_angular_velocity_rad_s);

  // 軌道
  /*
  VECTOR3_copy_double(aocs_data_recorder_.write_tlm_algorithm.sat_pos_est_eci_m,
                      aocs_manager->sat_pos_est_eci_m);
  VECTOR3_copy_double(aocs_data_recorder_.write_tlm_algorithm.sat_vel_est_eci_m_s,
                      aocs_manager->sat_vel_est_eci_m_s);
  */

  // Data record
  uint8_t data_tmp[APP_AOCS_DR_ALGORITHM_TLM_SIZE];
  memcpy(data_tmp, &(aocs_data_recorder_.write_tlm_algorithm), APP_AOCS_DR_ALGORITHM_TLM_SIZE);
  APP_NVM_MANAGER_ERROR ret = APP_NVM_PARTITION_write_bytes(aocs_data_recorder_.write_partition_id,
                                                            write_address, APP_AOCS_DR_ALGORITHM_TLM_SIZE, data_tmp);
  if (ret != APP_NVM_MANAGER_ERROR_OK)
  {
    EL_record_event(EL_GROUP_DR_WRITE, (uint32_t)ret, EL_ERROR_LEVEL_LOW, aocs_data_recorder_.write_pointer);
  }

  // 終了処理
  aocs_data_recorder_.write_tlm_algorithm.counter++;
  aocs_data_recorder_.write_pointer++;
  return;
}

static void APP_AOCS_DR_read_data_()
{
  // calc address
  uint32_t read_address = aocs_data_recorder_.read_pointer * APP_AOCS_DR_ALGORITHM_TLM_SIZE;

  // Data read
  uint8_t data_tmp[APP_AOCS_DR_ALGORITHM_TLM_SIZE];
  APP_NVM_MANAGER_ERROR ret = APP_NVM_PARTITION_read_bytes(data_tmp, aocs_data_recorder_.read_partition_id,
                                                           read_address, APP_AOCS_DR_ALGORITHM_TLM_SIZE);
  if (ret != APP_NVM_MANAGER_ERROR_OK)
  {
    EL_record_event(EL_GROUP_DR_READ, (uint32_t)ret, EL_ERROR_LEVEL_LOW, aocs_data_recorder_.read_pointer);
  }

  memcpy(&(aocs_data_recorder_.read_tlm_algorithm), data_tmp, APP_AOCS_DR_ALGORITHM_TLM_SIZE);

  return;
}

CCP_EXEC_STS Cmd_APP_AOCS_DR_SET_WRITE(const CommonCmdPacket* packet)
{
  uint8_t arg_num = 0;
  APP_AOCS_DR_RECORD_PERMISSION permission = (APP_AOCS_DR_RECORD_PERMISSION)CCP_get_param_from_packet(packet, arg_num, uint8_t);
  arg_num++;
  if (permission >= APP_AOCS_DR_RECORD_PERMISSION_MAX)
  {
    return CCP_EXEC_ILLEGAL_PARAMETER;
  }

  APP_AOCS_DR_OVERWRITE overwrite = (APP_AOCS_DR_OVERWRITE)CCP_get_param_from_packet(packet, arg_num, uint8_t);
  arg_num++;
  if (overwrite >= APP_AOCS_DR_OVERWRITE_MAX)
  {
    return CCP_EXEC_ILLEGAL_PARAMETER;
  }

  aocs_data_recorder_.record_permission = permission;
  aocs_data_recorder_.overwrite_mode = overwrite;

  if (permission == APP_AOCS_DR_RECORD_PERMISSION_DISABLE)
  {
    // Disableしたらcounterをリセットする
    aocs_data_recorder_.write_tlm_algorithm.counter = 0;
    return CCP_EXEC_SUCCESS;
  }

  // permission enable時のみpartition, write_pointerを設定
  APP_NVM_PARTITION_ID partition_id = (APP_NVM_PARTITION_ID)CCP_get_param_from_packet(packet, arg_num, uint8_t);
  arg_num++;
  if (partition_id < APP_NVM_PARTITION_ID_DR_1 || partition_id > APP_NVM_PARTITION_ID_DR_3)
  {
    return CCP_EXEC_ILLEGAL_PARAMETER;
  }

  uint32_t write_pointer = CCP_get_param_from_packet(packet, arg_num, uint32_t);
  arg_num++;
  if (write_pointer * APP_AOCS_DR_ALGORITHM_TLM_SIZE >= aocs_data_recorder_.write_end_address)
  {
    return CCP_EXEC_ILLEGAL_PARAMETER;
  }
  aocs_data_recorder_.write_partition_id = partition_id;
  aocs_data_recorder_.write_end_address = non_volatile_memory_partition->elements[aocs_data_recorder_.write_partition_id].size_byte;
  aocs_data_recorder_.write_pointer = write_pointer;

  return CCP_EXEC_SUCCESS;
}

CCP_EXEC_STS Cmd_APP_AOCS_DR_READ(const CommonCmdPacket* packet)
{
  uint8_t arg_num = 0;
  APP_AOCS_DR_READ_MODE read_mode = (APP_AOCS_DR_READ_MODE)CCP_get_param_from_packet(packet, arg_num, uint8_t);
  arg_num++;
  if (read_mode >= APP_AOCS_DR_READ_MODE_MAX)
  {
    return CCP_EXEC_ILLEGAL_PARAMETER;
  }

  APP_NVM_PARTITION_ID partition_id = (APP_NVM_PARTITION_ID)CCP_get_param_from_packet(packet, arg_num, uint8_t);
  arg_num++;
  if (partition_id < APP_NVM_PARTITION_ID_DR_1 || partition_id > APP_NVM_PARTITION_ID_DR_3)
  {
    return CCP_EXEC_ILLEGAL_PARAMETER;
  }

  uint32_t read_pointer = (APP_AOCS_DR_RECORD_PERMISSION)CCP_get_param_from_packet(packet, arg_num, uint32_t);
  arg_num++;
  if (read_pointer * APP_AOCS_DR_ALGORITHM_TLM_SIZE >= aocs_data_recorder_.read_end_address)
  {
    return CCP_EXEC_ILLEGAL_PARAMETER;
  }

  // 読み出し処理
  aocs_data_recorder_.read_mode = read_mode;
  if (read_mode == APP_AOCS_DR_READ_MODE_MANUAL)
  {
    aocs_data_recorder_.read_partition_id = partition_id;
    aocs_data_recorder_.read_end_address = non_volatile_memory_partition->elements[aocs_data_recorder_.read_partition_id].size_byte;
    aocs_data_recorder_.read_pointer = read_pointer;
  }
  else
  {
    aocs_data_recorder_.read_pointer++;
  }
  APP_AOCS_DR_read_data_();

  return CCP_EXEC_SUCCESS;
}

#pragma section
