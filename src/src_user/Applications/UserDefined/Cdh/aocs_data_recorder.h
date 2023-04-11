/**
* @file   aocs_data_recorder.h
* @brief  データレコーダアプリ
* @note   汎用的なものではなく、特定のデータのみ保存する方針 TODO_L: 汎用化する？
*/
#ifndef AOCS_DATA_RECORDER_H_
#define AOCS_DATA_RECORDER_H_

#include <src_core/System/ApplicationManager/app_info.h>
#include <src_core/TlmCmd/common_cmd_packet.h>

#include <src_core/System/TimeManager/obc_time.h>

#include "../AOCS/aocs_manager.h"
#include "non_volatile_memory_partition.h"


/**
 * @enum  APP_AOCS_DR_OVERWRITE
 * @brief 上書きするかどうか
 * @note  型はuint8_tを想定
 */
typedef enum
{
  APP_AOCS_DR_OVERWRITE_DISABLE,  // Disable
  APP_AOCS_DR_OVERWRITE_ENABLE,   // Enable
  APP_AOCS_DR_OVERWRITE_MAX
} APP_AOCS_DR_OVERWRITE;

/**
 * @enum  APP_AOCS_DR_RECORD_PERMISSION
 * @brief 書き込みするかどうか
 * @note  型はuint8_tを想定
 */
typedef enum
{
  APP_AOCS_DR_RECORD_PERMISSION_DISABLE,  // Disable
  APP_AOCS_DR_RECORD_PERMISSION_ENABLE,   // Enable
  APP_AOCS_DR_RECORD_PERMISSION_MAX
} APP_AOCS_DR_RECORD_PERMISSION;

/**
 * @enum  APP_AOCS_DR_READ_MODE
 * @brief 読み出しモード
 * @note  型はuint8_tを想定
 */
typedef enum
{
  APP_AOCS_DR_READ_MODE_MANUAL,      // Read pointを指定して読み出し
  APP_AOCS_DR_READ_MODE_CONTINUOUS,  // 前回読みだした次のデータを読み出し
  APP_AOCS_DR_READ_MODE_MAX
} APP_AOCS_DR_READ_MODE;

/**
 * @struct AOCS_DR_ALGORITHM_Telemetry
 * @brief  DRに保存するアルゴリズムテレメ
 */
typedef struct
{
  // カウンタ
  uint32_t counter;

  // 時間
  uint32_t obct_total_cycle_recorded;
  double  reference_jday;

  // アルゴリズムデータ
  Quaternion quaternion_est_i2b;
  float ang_vel_est_body_rad_s[PHYSICAL_CONST_THREE_DIM];
  float sun_vec_est_body[PHYSICAL_CONST_THREE_DIM];
  float mag_vec_est_body_nT[PHYSICAL_CONST_THREE_DIM];
  float internal_torque_target_body_Nm[PHYSICAL_CONST_THREE_DIM];
  float external_torque_target_body_Nm[PHYSICAL_CONST_THREE_DIM];
  float rw_angular_velocity_rad_s[AOCS_MANAGER_NUM_OF_RW];

  // 軌道
  // TODO_L: 現状DriverSuperの制限で144Byteを超えることができない
  // 軌道情報は地上でより高精度に計算可能なので一番優先度が低く、一旦消す
  // double sat_pos_est_eci_m  [PHYSICAL_CONST_THREE_DIM];
  // double sat_vel_est_eci_m_s[PHYSICAL_CONST_THREE_DIM];
} AOCS_DR_ALGORITHM_Telemetry;

/**
 * @struct AOCS_DR_COMPONENT_Telemetry
 * @brief  DRに保存するコンポーネントテレメ (TODO_L:今回は必要ないのでコメントアウト)
 */
/*
typedef struct
{
  // カウンタ
  uint32_t counter;

  // 時間
  ObcTime obct_recorded;

  // ジャイロ
  float mpu9250_ang_vel_raw_compo_rad_s[PHYSICAL_CONST_THREE_DIM];
  float stim210_ang_vel_raw_compo_rad_s[PHYSICAL_CONST_THREE_DIM];

  // 磁気センサ
  float mpu9250_mag_raw_compo_rad_s[PHYSICAL_CONST_THREE_DIM];
  float rm_aobc_mag_raw_compo_rad_s[PHYSICAL_CONST_THREE_DIM];
  float rm_ext_mag_raw_compo_rad_s [PHYSICAL_CONST_THREE_DIM];

  // サンセンサ
  float ss_py_angle_compo_deg[NANOSSOC_D60_ANGLE_ELEMENT_MAX];
  float ss_my_angle_compo_deg[NANOSSOC_D60_ANGLE_ELEMENT_MAX];
  float ss_pz_angle_compo_deg[NANOSSOC_D60_ANGLE_ELEMENT_MAX];
  float ss_mz_angle_compo_deg[NANOSSOC_D60_ANGLE_ELEMENT_MAX];

  // STT
  Quaternion stt_quaternion_i2c;

  // GPS-R
  double pos_antenna_ecef_m[PHYSICAL_CONST_THREE_DIM];
  double vel_antenna_ecef_m_s[PHYSICAL_CONST_THREE_DIM];
} AOCS_DR_COMPONENT_Telemetry;
*/

/**
 * @struct AocsDataRecorder
 * @brief  データレコーダー管理構造体
 */
typedef struct
{
  // read/write制御
  APP_NVM_PARTITION_ID write_partition_id;
  APP_NVM_PARTITION_ID read_partition_id;
  uint32_t write_end_address;
  uint32_t read_end_address;
  uint32_t write_pointer;  // write point
  uint32_t read_pointer;   // read  point

  // 設定
  APP_AOCS_DR_OVERWRITE         overwrite_mode;
  APP_AOCS_DR_RECORD_PERMISSION record_permission;
  APP_AOCS_DR_READ_MODE         read_mode;

  // 書き込み用テレメ
  AOCS_DR_ALGORITHM_Telemetry write_tlm_algorithm;

  // 保存用テレメ
  AOCS_DR_ALGORITHM_Telemetry read_tlm_algorithm;
} AocsDataRecorder;

extern const AocsDataRecorder* const aocs_data_recorder;
AppInfo APP_AOCS_DR_create_app(void);

// コマンド
CCP_CmdRet Cmd_APP_AOCS_DR_SET_WRITE(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_APP_AOCS_DR_READ(const CommonCmdPacket* packet);

#endif
