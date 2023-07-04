/**
* @file   stt_gyro_ekf.h
* @brief  STTとジャイロでEKFを組み，高精度姿勢決定を行うアプリ
*/
#ifndef STT_GYRO_EKF_H_
#define STT_GYRO_EKF_H_

#include <src_core/System/ApplicationManager/app_info.h>
#include <src_core/TlmCmd/common_cmd_packet.h>
#include  <src_core/System/TimeManager/time_manager.h>
#include <src_user/Library/quaternion.h>
#include <src_user/Library/matrix.h>
#include <src_user/Library/vector3.h>

MATRIX_DEFINE_MATRIX_SRTUCT(6, 6, float);

/**
 * @struct EstimatedResult
 * @brief  EKFによる姿勢推定結果
 */
typedef struct
{
  Quaternion quaternion_i2b;                                   //!< 姿勢quaternion ECI -> body
  float rate_bias_body_rad_s[PHYSICAL_CONST_THREE_DIM];        //!< レートバイアス．外部から使われることはないがテレメとして公開
  float angular_velocity_body_rad_s[PHYSICAL_CONST_THREE_DIM]; //!< (バイアス補正済角速度最終推定値)=(観測値)-(バイアス推定値)
  MATRIX_T(6, 6, float) covariance_matrix;                     //!< 共分散行列P
} EstimatedResult;

/*
 * @struct InitialCovariance
 * @brief  共分散行列Pの初期値
 * @note   成分を圧縮して扱うため，対角行列を前提とし，対角成分のみを持つ．
 *         上側成分はSTTのノイズ由来であり，cross方向とroll方向の違いなどがあると考えて3成分別々に持つ．
 *         下側成分はジャイロのノイズ由来であり，3軸の間に差違はないと考え，1成分に圧縮して持つ．
 */
typedef struct
{
  float diagonal_component_stt[PHYSICAL_CONST_THREE_DIM]; //!< 共分散行列Pの対角成分のうち，上側3つ
  float diagonal_component_gyro; //!< 共分散行列Pの対角成分のうち，下側成分
} InitialCovariance;

/*
 * @struct ProcessNoiseCovariance
 * @brief  プロセスノイズの共分散行列
 * @note   成分を圧縮して扱うため，対角行列を前提とし，対角成分のみを持つ．
 *         上側対角三成分と下側対角三成分をまとめて，一つずつに圧縮して持つ．
 */
typedef struct
{
  // TODO_L: 以下はテレメに公開し，コマンドで変更できるようにする？
  float attitude_element;      //!< プロセスノイズ共分散行列Qのうち，姿勢由来成分
  float attitude_rate_element; //!< プロセスノイズ共分散行列Qのうち，姿勢レート由来成分
} ProcessNoiseCovariance;

/**
 * @struct GyroRandomNoise
 * @brief  ジャイロのランダムノイズ
 */
typedef struct
{
  //!< コンポ座標でのジャイロランダムノイズ標準偏差[rad/s]（テレメとして公開）
  float standard_deviation_compo_rad_s[PHYSICAL_CONST_THREE_DIM];
  //!< body座標でのジャイロランダムノイズ標準偏差[rad/s]（テレメには非公開）
  float standard_deviation_body_rad_s[PHYSICAL_CONST_THREE_DIM];
} GyroRandomNoise;

/**
 * @struct GyroRandomWalk
 * @brief  ジャイロのランダムウォークノイズ
 */
typedef struct
{
  //!< コンポ座標でのジャイロランダムウォークノイズ標準偏差[rad/s2]（テレメとして公開）
  float standard_deviation_compo_rad_s2[PHYSICAL_CONST_THREE_DIM];
  //!< body座標でのジャイロランダムウォークノイズ標準偏差[rad/s2]（テレメには非公開）
  float standard_deviation_body_rad_s2[PHYSICAL_CONST_THREE_DIM];
  float time_constant_s; //!< ジャイロランダムウォークノイズにおける，ECRVモデルの時定数（テレメとして公開）
} GyroRandomWalk;

/**
 * @struct SttError
 * @brief  STT観測誤差
 */
typedef struct
{
  //!< コンポ座標でのSTT観測誤差標準偏差[rad]（テレメとして公開）
  float standard_deviation_compo_rad[PHYSICAL_CONST_THREE_DIM];
  //!< body座標でのSTT観測誤差標準偏差[rad]（テレメには非公開）
  float standard_deviation_body_rad[PHYSICAL_CONST_THREE_DIM];
} SttError;

/**
* @enum  APP_STT_GYRO_EKF_USE_ESTIMATED_ATTITUDE
* @brief EKFで推定した姿勢を実際の姿勢決定に利用するかどうか
* @note  uint_8を想定
*/
typedef enum
{
  APP_STT_GYRO_EKF_USE_ESTIMATED_ATTITUDE_ENABLE = 0, //!< 利用する
  APP_STT_GYRO_EKF_USE_ESTIMATED_ATTITUDE_DISABLE,    //!< 利用しない
  APP_STT_GYRO_EKF_USE_ESTIMATED_ATTITUDE_MAX
} APP_STT_GYRO_EKF_USE_ESTIMATED_ATTITUDE;

/**
* @enum  APP_STT_GYRO_EKF_CALCULATION
* @brief 計算が収束しているかどうか
* @note  uint_8を想定
*/
typedef enum
{
  APP_STT_GYRO_EKF_CALCULATION_CONVERGED = 0, //!< 収束している
  APP_STT_GYRO_EKF_CALCULATION_DIVERGED,      //!< 発散している
  APP_STT_GYRO_EKF_CALCULATION_MAX
} APP_STT_GYRO_EKF_CALCULATION;

/**
 * @struct SttGyroEkf
 * @brief  STT-Gyro EKF関連のテレメトリ
 */
typedef struct
{
  EstimatedResult estimated_result;                //!< 姿勢推定結果
  InitialCovariance initial_covariance;            //!< 共分散行列P初期値
  ProcessNoiseCovariance process_noise_covariance; //!< プロセスノイズの共分散
  GyroRandomNoise gyro_random_noise;               //!< ジャイロランダムノイズ
  GyroRandomWalk gyro_random_walk;                 //!< ジャイロランダムウォーク
  SttError stt_error;                              //!< STT観測誤差
  APP_STT_GYRO_EKF_USE_ESTIMATED_ATTITUDE use_ekf_estimated_attitude; //!< 推定姿勢を実際の姿勢決定に利用するかどうか
  APP_STT_GYRO_EKF_CALCULATION calculation_state; //!< EKF推定計算が発散したかどうか
} SttGyroEkf;

extern const SttGyroEkf* const stt_gyro_ekf;

/**
 * @brief  STT-Gyro EKFアプリ生成関数
 * @param  void
 * @return AppInfo
 */
AppInfo APP_STT_GYRO_EKF_create_app(void);

/**
 * @brief  STT-Gyro EKF 姿勢推定実行関数
 * @param  time_step_s: 時間刻み
 * @return void
 */
 void APP_STT_GYRO_EKF_execute_estimation(float time_step_s);

/*
* @brief  STT Gyro EKF ジャイロランダムノイズ標準偏差設定
* @param  CommonCmdPacket
* @return CCP_EXEC_STSに準拠
* @note   入力はコンポ座標系の値で行う．関数内部でbody座標系に変換した値も同時に入る．
*         あわせて，EKFの計算で使う観測ノイズ共分散行列の値も変更される．
*/
CCP_CmdRet Cmd_STT_GYRO_EKF_SET_GYRO_RANDOM_NOISE_STANDARD_DEVIATION(const CommonCmdPacket* packet);

/*
* @brief  STT Gyro EKF ジャイロランダムウォークノイズ標準偏差設定
* @param  CommonCmdPacket
* @return CCP_EXEC_STSに準拠
* @note   入力はコンポ座標系の値で行う．関数内部でbody座標系に変換した値も同時に入る．
*/
CCP_CmdRet Cmd_STT_GYRO_EKF_SET_GYRO_RANDOM_WALK_STANDARD_DEVIATION(const CommonCmdPacket* packet);

/*
* @brief  STT Gyro EKF STT観測誤差標準偏差設定
* @param  CommonCmdPacket
* @return CCP_EXEC_STSに準拠
* @note   入力はコンポ座標系の値で行う．関数内部でbody座標系に変換した値も同時に入る．
*         あわせて，EKFの計算で使う観測ノイズ共分散行列の値も変更される．
*/
CCP_CmdRet Cmd_STT_GYRO_EKF_SET_STT_ERROR_STANDARD_DEVIATION(const CommonCmdPacket* packet);

/*
* @brief  STT Gyro EKF 推定値リセットコマンド
* @param  CommonCmdPacket
* @return CCP_EXEC_STSに準拠
* @note   状態変数と共分散の推定値をリセットし，新たに姿勢推定をやり直す．
*/
CCP_CmdRet Cmd_STT_GYRO_EKF_RESET_ESTIMATION(const CommonCmdPacket* packet);

/*
* @brief  STT Gyro EKF 推定値反映コマンド
* @param  CommonCmdPacket
* @return CCP_EXEC_STSに準拠
* @note   EKFの姿勢推定値を衛星の姿勢推定に反映するかどうかを決める．
*/
CCP_CmdRet Cmd_STT_GYRO_EKF_ENABLE_ESTIMATED_ATTITUDE(const CommonCmdPacket* packet);

/*
* @brief  STT Gyro EKF 共分散行列初期値設定コマンド
* @param  CommonCmdPacket
* @return CCP_EXEC_STSに準拠
* @note   共分散行列Pの初期値を設定する．
*/
CCP_CmdRet Cmd_STT_GYRO_EKF_SET_INITIAL_COVARIANCE_MATRIX(const CommonCmdPacket* packet);

/*
* @brief  STT Gyro EKF プロセスノイズ設定コマンド
* @param  CommonCmdPacket
* @return CCP_EXEC_STSに準拠
* @note   プロセスノイズ共分散行列Qの初期値を設定する．
*/
CCP_CmdRet Cmd_STT_GYRO_EKF_SET_PROCESS_NOISE_MATRIX(const CommonCmdPacket* packet);

#endif
