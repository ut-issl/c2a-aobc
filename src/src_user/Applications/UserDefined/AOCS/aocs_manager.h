/**
* @file   aocs_manager.h
* @brief  AOCS系の変数などをH/Wと切り分けて管理するマネージャーアプリ
*/
#ifndef AOCS_MANAGER_H_
#define AOCS_MANAGER_H_

#include <stdint.h>
#include <src_core/System/ApplicationManager/app_info.h>
#include <src_core/System/TimeManager/obc_time.h>
#include <src_user/Library/physical_constants.h>
#include <src_user/Library/quaternion.h>
#include <src_user/Applications/DriverInstances/di_rw0003.h>
#include <src_user/Applications/DriverInstances/di_mtq_seiren.h>

/**
 * @enum   AOCS_MANAGER_ERROR
 * @brief  AOCS MANAGERの関数のエラー出力
 * @note   uint8_tを想定
 */
typedef enum
{
  AOCS_MANAGER_ERROR_OK = 0,
  AOCS_MANAGER_ERROR_NG = 1,
} AOCS_MANAGER_ERROR;

/**
 * @enum   AOCS_MANAGER_MTQ_OUTPUT_STATE
 * @brief  MTQの出力状態
 * @note   MTQ_OUTPUT_STATE_ONまたなMTQ_OUTPUT_DEMAGNITIZINGのとき，MTQ出力と地磁場が干渉するため，磁気センサの値が正しくなくなる
 * @note   制御アプリがMTQをONにしているかどうかを決定アプリに伝えるために使う
 * @note   uint8_tを想定
 */
typedef enum
{
  AOCS_MANAGER_MTQ_OUTPUT_STATE_OFF           = 0, //!< MTQ出力オフ
  AOCS_MANAGER_MTQ_OUTPUT_STATE_ON            = 1, //!< MTQ出力オン
  AOCS_MANAGER_MTQ_OUTPUT_STATE_DEMAGNITIZING = 2  //!< MTQ消磁中
} AOCS_MANAGER_MTQ_OUTPUT_STATE;

/**
 * @enum   AOCS_MANAGER_SUN_VISIVILITY
 * @brief  太陽がサンセンサから見えているかどうか
 * @note   複数あるサンセンサの中で，どれか一つでも太陽方向ベクトルが取れていればVISIBLEとなる
 * @note   サンセンサ選択アプリから，太陽方向ベクトルが取れていないことを太陽方向伝播アプリに伝えるために使う
 * @note   uint8_tを想定
 */
typedef enum
{
  AOCS_MANAGER_SUN_VISIBLE = 0,     //!< 少なくとも一つのサンセンサ情報から，太陽方向ベクトルが取れている状態
  AOCS_MANAGER_SUN_INVISIBILE       //!< サンセンサから太陽方向ベクトルが取れないので，太陽方向ベクトルをダイナミクスによって伝搬する必要がある状態
} AOCS_MANAGER_SUN_VISIBILITY;

/**
 * @enum   AOCS_MANAGER_GPS_VISIVILITY
 * @brief  GPSRの可視衛星数が十分 (=>4) 採れているか否か
 * @note   時刻決定やクロックドリフト推定の際にGPSR情報を用いるか否かの判定に利用
 * @note   uint8_tを想定
 */
typedef enum
{
  AOCS_MANAGER_GPS_VISIBLE = 0,     //!< 可視衛星数が十分な状態
  AOCS_MANAGER_GPS_INVISIBILE       //!< 可視衛星数が不十分な状態
} AOCS_MANAGER_GPS_VISIBILITY;

/**
 * @enum   AOCS_MANAGER_STT_AVAILABILITY
 * @brief  STTが正常な出力をしているかどうか
 * @note   uint8_tを想定
 */
typedef enum
{
  AOCS_MANAGER_STT_AVAILABLE = 0,   //!< STTが正常なQuaternionを出力している状態
  AOCS_MANAGER_STT_UNAVAILABLE      //!< STTが正常なQuaternionを出力していない状態
} AOCS_MANAGER_STT_AVAILABILITY;

/**
 * @enum   AOCS_MANAGER_CONSTANT_TORQUE_PERMISSION
 * @brief  定常トルクを出力するかどうかの許可フラグ用enum
 * @note   uint8_tを想定
 */
typedef enum
{
  AOCS_MANAGER_CONSTANT_TORQUE_DISABLE = 0, //!< 定常トルクは出力しない
  AOCS_MANAGER_CONSTANT_TORQUE_ENABLE  = 1, //!< 定常トルクを出力する
  AOCS_MANAGER_CONSTANT_TORQUE_MAX
} AOCS_MANAGER_CONSTANT_TORQUE_PERMISSION;

/**
 * @enum   AOCS_MANAGER_MAG_EXCLUSIVE_STATE
 * @brief  排他制御タイミング管理のOFF/ON状態
 * @note   uint8_tを想定
 */
typedef enum
{
  AOCS_MANAGER_MAG_EXCLUSIVE_STATE_IDLE   = 0, //!< 排他制御タイミング管理を停止
  AOCS_MANAGER_MAG_EXCLUSIVE_STATE_ACTIVE = 1, //!< 排他制御タイミング管理を開始
  AOCS_MANAGER_MAG_EXCLUSIVE_STATE_MAX
} AOCS_MANAGER_MAG_EXCLUSIVE_STATE;


#define AOCS_MANAGER_NUM_OF_MTQ (MTQ_SEIREN_IDX_MAX) //!< MTQ搭載個数
#define AOCS_MANAGER_NUM_OF_RW  (RW0003_IDX_MAX)     //!< RW搭載個数

/**
 * @struct AocsManager
 * @brief  AOCS系の変数などをH/Wと切り分けて管理するための構造体
 */
typedef struct
{
  // 衛星特性
  float mass_sc_kg;                                 //!< 宇宙機質量 [kg]
  float rmm_sc_body_Am2[PHYSICAL_CONST_THREE_DIM];  //!< 機体固定座標系での宇宙機残留磁気モーメント [Am2]
  float inertia_tensor_sc_body_kgm2[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM];  //!< 機体固定座標系での宇宙機慣性テンソル [kg m2]
  // 角速度
  float ang_vel_obs_body_rad_s   [PHYSICAL_CONST_THREE_DIM];  //!< センサで観測された機体固定座標系角速度 [rad/s]
  float ang_vel_est_body_rad_s   [PHYSICAL_CONST_THREE_DIM];  //!< 推定系で推定された機体固定座標系角速度 [rad/s]
  float ang_vel_target_body_rad_s[PHYSICAL_CONST_THREE_DIM];  //!< 制御系が目標とする機体固定座標系角速度 [rad/s]
  float ang_vel_error_body_rad_s [PHYSICAL_CONST_THREE_DIM];  //!< 目標値と推定角速度の機体固定座標系角速度差 [rad/s]
  float limit_angular_velocity_rad_s;                         //!< 目標値設定時の最大角速度 [rad/s]
  // 角運動量
  float ang_mom_est_body_Nms[PHYSICAL_CONST_THREE_DIM];  //!< 推定系で推定された機体全体の機体固定座標系角運動量 [Nms]
  // Quaternion
  Quaternion quaternion_obs_i2b;    //!< センサで観測されたECI->Body変換Quaternion
  Quaternion quaternion_est_i2b;    //!< 推定系で推定されたECI->Body変換Quaternion
  Quaternion quaternion_target_i2t; //!< 制御系が目標とするECI->目標変換Quaternion
  Quaternion quaternion_error_b2t;  //!< Body->目標変換Quaternion
  float limit_maneuver_angle_rad;   //!< 目標値設定時の最大マヌーバ角 [rad]
  // 太陽方向
  float sun_vec_obs_body   [PHYSICAL_CONST_THREE_DIM];  //!< センサで観測された機体固定座標系での機体->太陽方向ベクトル
  float sun_vec_est_body   [PHYSICAL_CONST_THREE_DIM];  //!< 推定系で推定された機体固定座標系での機体->太陽方向ベクトル
  float sun_vec_est_eci    [PHYSICAL_CONST_THREE_DIM];  //!< 推定系で推定されたECI座標系での機体->太陽方向ベクトル
  float sun_vec_target_body[PHYSICAL_CONST_THREE_DIM];  //!< 制御系が目標とする機体固定座標系での機体->太陽方向ベクトル
  float sun_vec_error_rad;                              //!< 目標ベクトルと推定ベクトルのなす角 [rad]
  // 磁場ベクトル
  float mag_vec_obs_body_nT[PHYSICAL_CONST_THREE_DIM];  //!< センサで観測された機体固定座標系での磁場ベクトル [nT]
  float mag_vec_est_body_nT[PHYSICAL_CONST_THREE_DIM];  //!< 推定系で推定された機体固定座標系での磁場ベクトル [nT]
  float mag_vec_est_eci_nT [PHYSICAL_CONST_THREE_DIM];  //!< 推定系で推定されたECI座標系での磁場ベクトル [nT]
  // トルク
  float internal_torque_target_body_Nm[PHYSICAL_CONST_THREE_DIM];     //!< 機体固定座標系での制御目標内力トルク [Nm]
  float external_torque_target_body_Nm[PHYSICAL_CONST_THREE_DIM];     //!< 機体固定座標系での制御目標外力トルク [Nm]
  float internal_torque_max_body_Nm   [PHYSICAL_CONST_THREE_DIM];     //!< 機体固定座標系での最大内力トルク [Nm]
  float external_torque_max_body_Nm   [PHYSICAL_CONST_THREE_DIM];     //!< 機体固定座標系での最大外力トルク [Nm]
  float torque_est_body_Nm            [PHYSICAL_CONST_THREE_DIM];     //!< 機体固定座標系で衛星に加わっているトルク推定値 [Nm]
  float constant_torque_body_Nm       [PHYSICAL_CONST_THREE_DIM];     //!< 機体固定座標系で出力させるべき固定トルク出力 [Nm]
  AOCS_MANAGER_CONSTANT_TORQUE_PERMISSION constant_torque_permission; //!< 固定トルク出力をするかどうかの許可フラグ
  // 磁気モーメント
  float mag_moment_target_body_Am2[PHYSICAL_CONST_THREE_DIM];  //!< 機体固定座標系での制御目標磁気モーメント [Am2]
  // MTQ情報
  float mtq_magnetic_moment_direction_matrix[AOCS_MANAGER_NUM_OF_MTQ][PHYSICAL_CONST_THREE_DIM]; //!< MTQの磁気モーメント方向をまとめたもの
  float mtq_distribution_matrix[PHYSICAL_CONST_THREE_DIM][AOCS_MANAGER_NUM_OF_MTQ];              //!< MTQの磁気モーメントの分配行列
  AOCS_MANAGER_MTQ_OUTPUT_STATE mtq_output_state;                                                //!< MTQ出力状態
  AOCS_MANAGER_MAG_EXCLUSIVE_STATE mag_exclusive_state;                                          //!< MTQ/磁気センサ排他制御タイミング管理状態
  // RW情報
  float rw_angular_velocity_rad_s[AOCS_MANAGER_NUM_OF_RW];                              //!< RWそれぞれの回転数(body座標系でないので注意) [rad/s]
  float rw_angular_momentum_Nms[AOCS_MANAGER_NUM_OF_RW];                                //!< RWそれぞれの角運動量(body座標系でないので注意) [Nms]
  float rw_angular_momentum_body_Nms[PHYSICAL_CONST_THREE_DIM];                         //!< body座標系でのRWの角運動量ベクトル [Nms]
  float rw_moment_of_inertia_kgm2[AOCS_MANAGER_NUM_OF_RW];                              //!< RWそれぞれのローター慣性モーメント [kgm2]
  float rw_rotation_direction_matrix[AOCS_MANAGER_NUM_OF_RW][PHYSICAL_CONST_THREE_DIM]; //!< RWの回転向きをRWの個数分まとめたもの
  float rw_distribution_matrix[PHYSICAL_CONST_THREE_DIM][AOCS_MANAGER_NUM_OF_RW];       //!< RW分配行列
  // 時刻
  GPS_TIME current_gps_time_obs;          //!< センサで観測されたGPS時刻
  ObcTime  obct_gps_time_obs;             //!< 上記をセンサで観測したタイミングのC2Aマスタークロック [-]
  // uint32_t current_gps_time_est_msec;  //!< 推定系で推定されたGPS時刻の秒成分 [msec] : 22_08_24時点で使用箇所がないため，一旦コメントアウト
  // uint16_t current_gps_time_est_week;  //!< 推定系で推定されたGPS時刻の週成分 [week] : 22_08_24時点で使用箇所がないため，一旦コメントアウト
  ObcTime  obct_gps_time_est;          //!< 上記を推定したタイミングのC2Aマスタークロック [-]
  double   reference_jday;             //!< 慣性系計算の基準時刻として用いるユリウス日 [ユリウス日]
  ObcTime  obct_reference;             //!< 慣性系計算の基準時刻として用いるユリウス日を更新した時のOBC時刻 [-]
  float obct_diff_max_limit_s;         //!< OBC時刻差計算時のアサーションに使う上限 [s]
  // センサ状態
  AOCS_MANAGER_GPS_VISIBILITY gps_visibility;     //!< GPS情報の利用可否
  AOCS_MANAGER_SUN_VISIBILITY sun_visibility;     //!< 太陽がサンセンサから見えているかどうか
  AOCS_MANAGER_STT_AVAILABILITY stt_availability; //!< STTが利用可能かどうか
  // 軌道
  double sat_pos_obs_ecef_m   [PHYSICAL_CONST_THREE_DIM];  //!< センサで観測されたECEF座標系での衛星位置ベクトル [m]
  double sat_vel_obs_ecef_m_s [PHYSICAL_CONST_THREE_DIM];  //!< センサで観測されたECEF座標系での衛星速度ベクトル [m/s]
  double sat_pos_obs_eci_m    [PHYSICAL_CONST_THREE_DIM];  //!< センサで観測されたECEF座標系での衛星位置ベクトルをECIに変換したもの [m]
  double sat_vel_obs_eci_m_s  [PHYSICAL_CONST_THREE_DIM];  //!< センサで観測されたECEF座標系での衛星速度ベクトルをECIに変換したもの [m/s]
  double sat_pos_est_ecef_m   [PHYSICAL_CONST_THREE_DIM];  //!< 推定系で推定されたECEF座標系での衛星位置ベクトル [m]
  double sat_vel_est_ecef_m_s [PHYSICAL_CONST_THREE_DIM];  //!< 推定系で推定されたECEF座標系での衛星速度ベクトル [m/s]
  double sat_pos_est_eci_m    [PHYSICAL_CONST_THREE_DIM];  //!< 推定系で推定されたECI座標系での衛星位置ベクトル [m]
  double sat_vel_est_eci_m_s  [PHYSICAL_CONST_THREE_DIM];  //!< 推定系で推定されたECEF座標系での衛星速度ベクトル [m/s]
  double sat_pos_est_lla_rad_m[PHYSICAL_CONST_THREE_DIM];  //!< 推定系で推定された緯度経度高度座標系での衛星位置ベクトル [rad, m]
  // 座標変換
  double dcm_eci_to_ecef[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM];  //!< ECI->ECEF変換行列
  // 各種フラグ
  uint8_t flags_for_tlm;   //!< テレメ用に各種フラグをまとめたもの
} AocsManager;

extern const AocsManager* const aocs_manager;

AppInfo APP_AOCS_MANAGER_create_app(void);

// Setter functions
// 衛星特性
AOCS_MANAGER_ERROR AOCS_MANAGER_set_mass_sc_kg(const float mass_sc_kg);
AOCS_MANAGER_ERROR AOCS_MANAGER_set_rmm_sc_body_Am2(const float rmm_sc_body_Am2[PHYSICAL_CONST_THREE_DIM]);
AOCS_MANAGER_ERROR AOCS_MANAGER_set_inertia_tensor_sc_body_kgm2(const float inertia_tensor_sc_body_kgm2[][PHYSICAL_CONST_THREE_DIM]);
AOCS_MANAGER_ERROR AOCS_MANAGER_set_limit_angular_velocity_rad_s(const float limit_angular_velocity_rad_s);
// 角速度
AOCS_MANAGER_ERROR AOCS_MANAGER_set_ang_vel_obs_body_rad_s(const float ang_vel_obs_body_rad_s[PHYSICAL_CONST_THREE_DIM]);
AOCS_MANAGER_ERROR AOCS_MANAGER_set_ang_vel_est_body_rad_s(const float ang_vel_est_body_rad_s[PHYSICAL_CONST_THREE_DIM]);
AOCS_MANAGER_ERROR AOCS_MANAGER_set_ang_vel_target_body_rad_s(const float ang_vel_target_body_rad_s[PHYSICAL_CONST_THREE_DIM]);
AOCS_MANAGER_ERROR AOCS_MANAGER_set_ang_vel_error_body_rad_s(const float ang_vel_error_body_rad_s[PHYSICAL_CONST_THREE_DIM]);
// Quaternion
AOCS_MANAGER_ERROR AOCS_MANAGER_set_quaternion_obs_i2b(const Quaternion quaternion_obs_i2b);
AOCS_MANAGER_ERROR AOCS_MANAGER_set_quaternion_est_i2b(const Quaternion quaternion_est_i2b);
AOCS_MANAGER_ERROR AOCS_MANAGER_set_quaternion_target_i2t(const Quaternion quaternion_target_i2t);
AOCS_MANAGER_ERROR AOCS_MANAGER_set_quaternion_error_b2t(const Quaternion quaternion_error_b2t);
AOCS_MANAGER_ERROR AOCS_MANAGER_set_limit_maneuver_angle_rad(const float limit_maneuver_angle_rad);
// 太陽方向
AOCS_MANAGER_ERROR AOCS_MANAGER_set_sun_vec_obs_body(const float sun_vec_obs_body[PHYSICAL_CONST_THREE_DIM]);
AOCS_MANAGER_ERROR AOCS_MANAGER_set_sun_vec_est_body(const float sun_vec_est_body[PHYSICAL_CONST_THREE_DIM]);
AOCS_MANAGER_ERROR AOCS_MANAGER_set_sun_vec_est_eci (const float sun_vec_est_eci [PHYSICAL_CONST_THREE_DIM]);
AOCS_MANAGER_ERROR AOCS_MANAGER_set_sun_vec_target_body(const float sun_vec_target_body[PHYSICAL_CONST_THREE_DIM]);
AOCS_MANAGER_ERROR AOCS_MANAGER_set_sun_vec_error_rad(const float sun_vec_error_rad);
// 磁場ベクトル
AOCS_MANAGER_ERROR AOCS_MANAGER_set_mag_vec_obs_body_nT(const float mag_vec_obs_body_nT[PHYSICAL_CONST_THREE_DIM]);
AOCS_MANAGER_ERROR AOCS_MANAGER_set_mag_vec_est_body_nT(const float mag_vec_est_body_nT[PHYSICAL_CONST_THREE_DIM]);
AOCS_MANAGER_ERROR AOCS_MANAGER_set_mag_vec_est_eci_nT (const float mag_vec_est_eci_nT [PHYSICAL_CONST_THREE_DIM]);
// トルク
AOCS_MANAGER_ERROR AOCS_MANAGER_set_internal_torque_target_body_Nm(const float internal_torque_target_body_Nm[PHYSICAL_CONST_THREE_DIM]);
AOCS_MANAGER_ERROR AOCS_MANAGER_set_external_torque_target_body_Nm(const float external_torque_target_body_Nm[PHYSICAL_CONST_THREE_DIM]);
AOCS_MANAGER_ERROR AOCS_MANAGER_set_internal_torque_max_body_Nm(const float internal_torque_max_body_Nm[PHYSICAL_CONST_THREE_DIM]);
AOCS_MANAGER_ERROR AOCS_MANAGER_set_external_torque_max_body_Nm(const float external_torque_max_body_Nm[PHYSICAL_CONST_THREE_DIM]);
AOCS_MANAGER_ERROR AOCS_MANAGER_set_torque_est_body_Nm(const float torque_est_body_Nm[PHYSICAL_CONST_THREE_DIM]);
AOCS_MANAGER_ERROR AOCS_MANAGER_set_constant_torque_body_Nm(const float constant_torque_body_Nm[PHYSICAL_CONST_THREE_DIM]);
AOCS_MANAGER_ERROR AOCS_MANAGER_set_constant_torque_permission(const AOCS_MANAGER_CONSTANT_TORQUE_PERMISSION constant_torque_permission);
// 磁気モーメント
AOCS_MANAGER_ERROR AOCS_MANAGER_set_mag_moment_target_body_Am2(const float mag_moment_target_body_Am2[PHYSICAL_CONST_THREE_DIM]);
// MTQ
AOCS_MANAGER_ERROR AOCS_MANAGER_set_mtq_magnetic_moment_direction_matrix(
  const float mtq_magnetic_moment_direction_matrix[AOCS_MANAGER_NUM_OF_MTQ][PHYSICAL_CONST_THREE_DIM]);
AOCS_MANAGER_ERROR AOCS_MANAGER_set_mtq_output_state(const AOCS_MANAGER_MTQ_OUTPUT_STATE mtq_output_state);
AOCS_MANAGER_ERROR AOCS_MANAGER_set_mag_exclusive_state(const AOCS_MANAGER_MAG_EXCLUSIVE_STATE mag_exclusive_state);
// RW
AOCS_MANAGER_ERROR AOCS_MANAGER_set_rw_angular_velocity_rad_s(const float rw_angular_velocity_rad_s[AOCS_MANAGER_NUM_OF_RW]);
AOCS_MANAGER_ERROR AOCS_MANAGER_set_rw_rotation_direction_matrix(
  const float rw_rotation_direction_body[AOCS_MANAGER_NUM_OF_RW][PHYSICAL_CONST_THREE_DIM]);
// 時刻
AOCS_MANAGER_ERROR AOCS_MANAGER_set_current_gps_time_obs(const GPS_TIME current_gps_time,
                                                         const ObcTime  obct_gps_time_obs);
// 22_08_24時点で使用箇所がないため，一旦コメントアウト
// AOCS_MANAGER_ERROR AOCS_MANAGER_set_current_gps_time_est(const uint32_t current_gps_time_est_msec,
//                                                          const uint16_t current_gps_time_est_week,
//                                                          const ObcTime  obct_gps_time_est);
AOCS_MANAGER_ERROR AOCS_MANAGER_set_reference_jday(const double reference_jday, const ObcTime obct_reference);
// センサ状態
AOCS_MANAGER_ERROR AOCS_MANAGER_set_sun_visibility(const AOCS_MANAGER_SUN_VISIBILITY sun_visibility);
AOCS_MANAGER_ERROR AOCS_MANAGER_set_stt_availability(const AOCS_MANAGER_STT_AVAILABILITY stt_availability);
AOCS_MANAGER_ERROR AOCS_MANAGER_set_gps_visibility(const AOCS_MANAGER_GPS_VISIBILITY gps_visibility);
// 軌道
AOCS_MANAGER_ERROR AOCS_MANAGER_set_sat_pos_vel_obs_ecef_m (const double sat_pos_obs_ecef_m   [PHYSICAL_CONST_THREE_DIM],
                                                            const double sat_vel_obs_ecef_m_s [PHYSICAL_CONST_THREE_DIM]);
AOCS_MANAGER_ERROR AOCS_MANAGER_set_sat_pos_vel_obs_eci_m(const double sat_pos_obs_eci_m[PHYSICAL_CONST_THREE_DIM],
                                                          const double sat_vel_obs_eci_m_s[PHYSICAL_CONST_THREE_DIM]);
AOCS_MANAGER_ERROR AOCS_MANAGER_set_sat_pos_vel_est_ecef_m (const double sat_pos_est_ecef_m   [PHYSICAL_CONST_THREE_DIM],
                                                            const double sat_vel_est_ecef_m_s [PHYSICAL_CONST_THREE_DIM]);
AOCS_MANAGER_ERROR AOCS_MANAGER_set_sat_pos_vel_est_eci_m  (const double sat_pos_est_eci_m    [PHYSICAL_CONST_THREE_DIM],
                                                            const double sat_vel_est_eci_m_s  [PHYSICAL_CONST_THREE_DIM]);
AOCS_MANAGER_ERROR AOCS_MANAGER_set_sat_pos_est_lla_rad_m(const double sat_pos_est_lla_rad_m[PHYSICAL_CONST_THREE_DIM]);
// 座標変換
AOCS_MANAGER_ERROR AOCS_MANAGER_set_dcm_eci_to_ecef(const double dcm_eci_to_ecef[PHYSICAL_CONST_THREE_DIM][PHYSICAL_CONST_THREE_DIM]);


// 計算関数
/**
 * @brief      定常トルク出力許可フラグを確認した上で、定常トルクを加える
 * @param[out] out_torque_body_Nm : 定常トルクを足した後のトルクベクトル[Nm]
 * @param[in]  in_torque_body_Nm  : 定常トルクに足されるトルクベクトル[Nm]
 * @return     AOCS_MANAGER_ERRORに準拠
 */
AOCS_MANAGER_ERROR AOCS_MANAGER_add_constant_torque_body_nm(float out_torque_body_Nm[PHYSICAL_CONST_THREE_DIM],
                                                            const float in_torque_body_Nm[PHYSICAL_CONST_THREE_DIM]);

/**
 * @brief      ジャイロトルクを計算し、目標トルクに追加する
 * @param[out] out_torque_body_Nm : ジャイロトルクを足した後のトルクベクトル[Nm]
 * @param[in]  in_torque_body_Nm  : ジャイロトルクに足されるトルクベクトル[Nm]
 * @return     AOCS_MANAGER_ERRORに準拠
 */
AOCS_MANAGER_ERROR AOCS_MANAGER_add_gyroscopic_torque_body_nm(float out_torque_body_Nm[PHYSICAL_CONST_THREE_DIM],
                                                              const float in_torque_body_Nm[PHYSICAL_CONST_THREE_DIM]);


// Command Functions
// Structure
CCP_CmdRet Cmd_APP_AOCS_MANAGER_SET_MASS(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_APP_AOCS_MANAGER_SET_INERTIA_TENSOR(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_APP_AOCS_MANAGER_SET_RMM(const CommonCmdPacket* packet);
// Target
CCP_CmdRet Cmd_APP_AOCS_MANAGER_SET_TARGET_QUATERNION(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_APP_AOCS_MANAGER_SET_TARGET_QUATERNION_FROM_CURRENT_ATTITUDE(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_APP_AOCS_MANAGER_SET_LIMIT_MANEUVER_ANGLE(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_APP_AOCS_MANAGER_SET_LIMIT_ANGULAR_VELOCITY(const CommonCmdPacket* packet);
// Others
CCP_CmdRet Cmd_APP_AOCS_MANAGER_SET_REFERENCE_TIME(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_APP_AOCS_MANAGER_SET_CONSTANT_TORQUE(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_APP_AOCS_MANAGER_SET_CONSTANT_TORQUE_PERMISSION(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_APP_AOCS_MANAGER_SET_MAG_EXCLUSIVE_STATE(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_APP_AOCS_MANAGER_SET_MAX_IN_TORQUE(const CommonCmdPacket* packet);
CCP_CmdRet Cmd_APP_AOCS_MANAGER_SET_MAX_EXT_TORQUE(const CommonCmdPacket* packet);

#endif
