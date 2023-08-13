/**
* @file   sagitta.h
* @brief  SAGITTA恒星センサのドライバ
*/
#ifndef SAGITTA_H_
#define SAGITTA_H_

#include <src_core/IfWrapper/uart.h>
#include <src_core/Drivers/Super/driver_super.h>
#include <src_user/Library/physical_constants.h>
#include <src_user/Library/quaternion.h>

#define SAGITTA_PARAMETER_LOG_LEVEL_LENGTH                (16)
#define SAGITTA_PARAMETER_LIMITS_LENGTH                   (10)
#define SAGITTA_PARAMETER_CAMERA_OVERRIDE_REGISTER_LENGTH (16)
#define SAGITTA_PARAMETER_SUBSCRIPTION_LENGTH             (16)

/**
 * @enum   SAGITTA_TLM_TYPE
 * @brief  テレメフレームのタイプ
 * @note   uint8_tを想定
 */
typedef enum
{
  SAGITTA_TLM_TYPE_SET_PARAMETER_REPLY = 128,
  SAGITTA_TLM_TYPE_PARAMETER_REPLY = 129,
  SAGITTA_TLM_TYPE_TELEMETRY_REPLY = 130,
  SAGITTA_TLM_TYPE_ACTION_REPLY = 131,
  SAGITTA_TLM_TYPE_ASYNCHRONOUS_TELEMETRY_REPLY = 132
} SAGITTA_TLM_TYPE;

/**
 * @enum   SAGITTA_PARAM_ID
 * @brief  パラメータ設定に用いるID
 * @note   uint8_tを想定
 */
typedef enum
{
  SAGITTA_PARAMETER_ID_LOG_LEVEL = 3,
  SAGITTA_PARAMETER_ID_LIMITS = 5,
  SAGITTA_PARAMETER_ID_MOUNTING = 6,
  SAGITTA_PARAMETER_ID_CAMERA = 9,
  SAGITTA_PARAMETER_ID_IMAGE_PROCESSOR = 10,
  SAGITTA_PARAMETER_ID_CENTROIDING = 11,
  SAGITTA_PARAMETER_ID_LISA = 12,
  SAGITTA_PARAMETER_ID_MATCHING = 13,
  SAGITTA_PARAMETER_ID_TRACKING = 14,
  SAGITTA_PARAMETER_ID_VALIDATION = 15,
  SAGITTA_PARAMETER_ID_ALGO = 16,
  SAGITTA_PARAMETER_ID_SUBSCRIPTION = 18
} SAGITTA_PARAMETER_ID;

/**
 * @enum   SAGITTA_TLM_ID
 * @brief  テレメ要求コマンド・テレメ解釈に用いる受信モード
 * @note   uint8_tを想定
 */
typedef enum
{
  SAGITTA_TLM_ID_SOLUTION = 24,
  SAGITTA_TLM_ID_TEMPERATURE = 27
} SAGITTA_TLM_ID;

/**
 * @enum   SAGITTA_ACTION_ID
 * @brief  Actionコマンドに用いるID
 * @note   uint8_tを想定
 */
typedef enum
{
  SAGITTA_ACTION_ID_BOOT = 1,
  SAGITTA_ACTION_ID_SETTIME = 14
} SAGITTA_ACTION_ID;

/**
 * @enum   SAGITTA_REC_ERR_CODE
 * @brief  Sagittaの受信エラーコード
 * @note   uint8_tを想定
 */
typedef enum
{
  SAGITTA_REC_ERR_CODE_OK = 0,
  SAGITTA_REC_ERR_CODE_ILLEGAL_LENGTH,
  SAGITTA_REC_ERR_CODE_ILLEGAL_ENCODE,
  SAGITTA_REC_ERR_CODE_MAX,
} SAGITTA_REC_ERR_CODE;

/**
 * @enum  SAGITTA_XXHASH_STATE
 * @brief
 * SAGITTAから送られてきたxxhashとドライバで計算したxxhashが一致しているかどうか
 * @note  uint_8を想定
 */
typedef enum
{
  SAGITTA_XXHASH_STATE_OK = 0,
  SAGITTA_XXHASH_STATE_NG = 1
} SAGITTA_XXHASH_STATE;

/**
 * @enum  SAGITTA_SOLUTION_STRATEGY
 * @brief The algorithm path in the autonomous mode that was used to obtain the star tracker attitude
 * @note  uint_8を想定
 * @note  2に対応する状態はない
 */
typedef enum
{
  SAGITTA_SOLUTION_STRATEGY_ONLY_LISA = 0,
  SAGITTA_SOLUTION_STRATEGY_ONLY_TRACKING = 1,
  SAGITTA_SOLUTION_STRATEGY_LISA_THEN_TRACKING = 3,
  SAGITTA_SOLUTION_STRATEGY_AUTONOMOUS_LISA_OK = 4,
  SAGITTA_SOLUTION_STRATEGY_AUTONOMOUS_TRACKING_OK = 5,
  SAGITTA_SOLUTION_STRATEGY_AUTONOMOUS_TRACKING_LOW_CONFIDENCE_RECOVERED = 6,
  SAGITTA_SOLUTION_STRATEGY_AUTONOMOUS_TRACKING_DEMOTO_TO_LISA = 7,
  SAGITTA_SOLUTION_STRATEGY_AUTONOMOUS_LISA_LOW_CONFIDENCE = 8
} SAGITTA_SOLUTION_STRATEGY;

/**
 * @struct SAGITTA_PARAMETER_LIMITS
 * @brief  Sagittaの過電流・高温検知パラメータを格納する
 */
typedef struct
{
  uint8_t action;
  float max_value[SAGITTA_PARAMETER_LIMITS_LENGTH];
} SAGITTA_PARAMETER_LIMITS;

/**
 * @struct SAGITTA_PARAMETER_CAMERA
 * @brief  SagittaのCAMERAパラメータを格納する
 */
typedef struct
{
  uint8_t mode;
  float focal_length_mm;
  float exposure_s;
  float interval_s;
  int16_t offset_pix;
  uint8_t pga_gain;
  uint8_t adc_gain;
  uint8_t override_register[SAGITTA_PARAMETER_CAMERA_OVERRIDE_REGISTER_LENGTH];
  uint32_t freq;
} SAGITTA_PARAMETER_CAMERA;

/**
 * @struct SAGITTA_PARAMETER_IMAGE_PROCESSOR
 * @brief  SagittaのIMAGE PROCESSORパラメータを格納する
 */
typedef struct
{
  uint8_t mode;
  uint8_t store;
  uint16_t signal_threshold_pix;
  uint16_t dark_threshold_pix;
  uint8_t background_compensation;
} SAGITTA_PARAMETER_IMAGE_PROCESSOR;

/**
 * @struct SAGITTA_PARAMETER_CENTROIDING
 * @brief  SagittaのCENTROIDINGパラメータを格納する
 */
typedef struct
{
  uint8_t enable_filter;
  float max_quality;
  float dark_threshold_pix;
  float min_quality;
  float max_intensity;
  float min_intensity;
  float max_magnitude;
  float gaussian_cmax_pix;
  float gaussian_cmin_pix;
  float transmatrix_00;
  float transmatrix_01;
  float transmatrix_10;
  float transmatrix_11;
} SAGITTA_PARAMETER_CENTROIDING;

/**
 * @struct SAGITTA_PARAMETER_LISA
 * @brief  SagittaのLost In Space Algorithmパラメータを格納する
 */
typedef struct
{
  uint32_t mode;
  float prefilter_distance_threshold_mm;
  float prefilter_angle_threshold_rad;
  float field_of_view_width_rad;
  float field_of_view_height_rad;
  float float_star_limit_mm;
  float close_star_limit_mm;
  float rating_weight_close_star_count;
  float rating_weight_fraction_close;
  float rating_weight_mean_sum;
  float rating_weight_db_star_count;
  uint8_t max_combinations;
  uint8_t nr_stars_stop;
  float fraction_close_stop;
} SAGITTA_PARAMETER_LISA;

/**
 * @struct SAGITTA_PARAMETER_MATCHING
 * @brief  SagittaのMATCHINGパラメータを格納する
 */
typedef struct
{
  float squared_distance_limit_mm;
  float squared_shift_limit_mm;
} SAGITTA_PARAMETER_MATCHING;

/**
 * @struct SAGITTA_PARAMETER_TRACKING
 * @brief  SagittaのTrackingパラメータを格納する
 */
typedef struct
{
  float thin_limit_deg;
  float outlier_threshold_mm;
  float outlier_threshold_quest_mm;
  uint8_t tracker_choice;
} SAGITTA_PARAMETER_TRACKING;

/**
 * @struct SAGITTA_PARAMETER_VALIDATION
 * @brief  SagittaのValidationパラメータを格納する
 */
typedef struct
{
  uint8_t stable_count;
  float max_difference_deg;
  float min_tracker_confidence;
  uint8_t min_matched_stars;
} SAGITTA_PARAMETER_VALIDATION;

/**
 * @struct SAGITTA_PARAMETER_ALGO
 * @brief  SagittaのALGOパラメータを格納する
 * @note   Algo: Algorithm. ICDでのparameter名称.
 */
typedef struct
{
  uint8_t mode;
  float l2t_min_confidence;
  uint8_t l2t_min_matched;
  float t2l_min_confidence;
  uint8_t t2l_min_matched;
} SAGITTA_PARAMETER_ALGO;

/**
 * @struct SAGITTA_Parameter
 * @brief  Sagittaの内部パラメータを格納する
 */
typedef struct
{
  uint8_t log_level[SAGITTA_PARAMETER_LOG_LEVEL_LENGTH];
  SAGITTA_PARAMETER_LIMITS limits;
  Quaternion mounting;
  SAGITTA_PARAMETER_CAMERA camera;
  SAGITTA_PARAMETER_IMAGE_PROCESSOR image_processor;
  SAGITTA_PARAMETER_CENTROIDING centroiding;
  SAGITTA_PARAMETER_LISA lisa;
  SAGITTA_PARAMETER_MATCHING matching;
  SAGITTA_PARAMETER_TRACKING tracking;
  SAGITTA_PARAMETER_VALIDATION validation;
  SAGITTA_PARAMETER_ALGO algo;
  uint8_t subscription[SAGITTA_PARAMETER_SUBSCRIPTION_LENGTH];
} SAGITTA_PARAMETER;

/**
 * @struct SAGITTA_Info
 * @brief  Sagittaのモードや変数などを格納する
 */
typedef struct
{
  uint8_t is_valid_quaternion;    //!< Status of attitude determination (0: not valid, 1: valid)
  Quaternion quaternion_i2c;      //!< Quaternion ECI -> Component Frame
  Quaternion quaternion_i2b;      //!< Quaternion ECI -> Body
  Quaternion frame_transform_c2b; //!< frame transformation quaternion from component frame to body frame
  float track_confidence;         //!< Tracker Confidence Value. The lower this value is, the more confidence we can have in the solution.
  uint8_t num_stars_removed;      //!< The number of stars that were removed by the tracking algorithms that can remove outlier stars
  uint8_t num_stars_centroided;   //!< The number of stars for which a valid centroid was found
  uint8_t num_stars_matched;      //!< The number of stars that was matched to a database star
  float lisa_percentage_close;    //!< The percentage of identified stars in the image
  uint8_t num_stars_lisa_close;   //!< The number of identified stars in the image
  uint8_t star_tracker_mode;      //!< LISA = 1, Tracking = 0
  uint32_t stable_count;          //!< The number of times the validation criteria were met.
  SAGITTA_SOLUTION_STRATEGY solution_strategy; //!< Solution strategy based on LISA parameters, stars_centroided and stars_matched

  SAGITTA_TLM_TYPE tlm_type;         //!< テレメのタイプ
  uint8_t tlm_id;                    //!< テレメに含まれるparameter, action, telemetryのID
  uint8_t tlm_status;                //!< テレメに含まれるset parameter, action, telemetryのstatus
  uint32_t unix_time_ms;             //!< Unix time [ms]
  float temperature_mcu_degC;        //!< Temperature of micro-computer [0 ~ 100 degC]
  float temperature_fpga_degC;       //!< Temperature of FPGA [0 ~ 100 degC]
  SAGITTA_REC_ERR_CODE err_status;   //!< Receive error status
  uint32_t xxhash;                   //!< 受信したxxhash
  SAGITTA_XXHASH_STATE xxhash_state; //!< 受信したxxhashが正しいか
  SAGITTA_PARAMETER set_parameter;   //!< Sagittaに設定するパラメータ
  SAGITTA_PARAMETER read_parameter;  //!< Sagittaか読み出したパラメータ
} SAGITTA_Info;

/**
 * @struct SAGITTA_Driver
 * @brief SAGITTA_Driver構造体
 */
typedef struct
{
  struct
  {
    DriverSuper super;       //!< DriverSuper class
    UART_Config uart_config;  //!< UART class
  } driver;
  SAGITTA_Info info;
} SAGITTA_Driver;

// 基本関数

/**
 * @brief  SAGITTA初期化
 *
 *         SAGITTA_Driver構造体のポインタを渡すことでポートを初期化し，SAGITTA_Driverの各メンバも初期化する
 * @param  sagitta_driver    : 初期化するSAGITTA_Driver構造体へのポインタ
 * @param  ch          : SAGITTAが接続されているUARTポート番号
 * @param  rx_buffer: 受信バッファ
 * @return DS_INIT_ERR_CODE
 */
DS_INIT_ERR_CODE SAGITTA_init(SAGITTA_Driver* sagitta_driver, uint8_t ch, DS_StreamRecBuffer* rx_buffer);

/**
 * @brief  SAGITTA受信
 * @param  sagitta_driver    : SAGITTA_Driver構造体へのポインタ
 * @retval DS_REC_ERR_CODEに準拠
 */
DS_REC_ERR_CODE SAGITTA_rec(SAGITTA_Driver* sagitta_driver);

/**
 * @brief  SAGITTAをブートする
 * @param  sagitta_driver    : SAGITTA_Driver構造体へのポインタ
 * @return DS_CMD_ERR_CODEを参照
 */
DS_CMD_ERR_CODE SAGITTA_boot(SAGITTA_Driver* sagitta_driver);

/**
 * @brief  SAGITTAのUnixTime[us]を設定する
 * @param  sagitta_driver    : SAGITTA_Driver構造体へのポインタ
 * @param  unix_time_us_upper : UnixTime上位32bit
 * @param  unix_time_us_lower : UnixTime下位32bit
 * @return DS_CMD_ERR_CODEを参照
 */
DS_CMD_ERR_CODE SAGITTA_set_unix_time_us(SAGITTA_Driver* sagitta_driver, const uint32_t unix_time_us_upper, const uint32_t unix_time_us_lower);

/**
 * @brief  SAGITTAのLog Levelパラメータを設定する
 * @param  sagitta_driver    : SAGITTA_Driver構造体へのポインタ
 * @return DS_CMD_ERR_CODEを参照
 */
DS_CMD_ERR_CODE SAGITTA_set_log_level(SAGITTA_Driver* sagitta_driver);

/**
 * @brief  SAGITTAのLimitsパラメータを設定する
 * @param  sagitta_driver    : SAGITTA_Driver構造体へのポインタ
 * @return DS_CMD_ERR_CODEを参照
 */
DS_CMD_ERR_CODE SAGITTA_set_limits(SAGITTA_Driver* sagitta_driver);

/**
 * @brief  SAGITTAのMountingパラメータを設定する
 * @param  sagitta_driver    : SAGITTA_Driver構造体へのポインタ
 * @return DS_CMD_ERR_CODEを参照
 */
DS_CMD_ERR_CODE SAGITTA_set_mounting(SAGITTA_Driver* sagitta_driver);

/**
 * @brief  SAGITTAのCameraパラメータを設定する
 * @param  sagitta_driver    : SAGITTA_Driver構造体へのポインタ
 * @return DS_CMD_ERR_CODEを参照
 */
DS_CMD_ERR_CODE SAGITTA_set_camera(SAGITTA_Driver* sagitta_driver);

/**
 * @brief  SAGITTAのImage Processorパラメータを設定する
 * @param  sagitta_driver    : SAGITTA_Driver構造体へのポインタ
 * @return DS_CMD_ERR_CODEを参照
 */
DS_CMD_ERR_CODE SAGITTA_set_image_processor(SAGITTA_Driver* sagitta_driver);

/**
 * @brief  SAGITTAのCentroidingパラメータを設定する
 * @param  sagitta_driver    : SAGITTA_Driver構造体へのポインタ
 * @return DS_CMD_ERR_CODEを参照
 */
DS_CMD_ERR_CODE SAGITTA_set_centroiding(SAGITTA_Driver* sagitta_driver);

/**
 * @brief  SAGITTAのLost in Space Algorithmパラメータを設定する
 * @param  sagitta_driver    : SAGITTA_Driver構造体へのポインタ
 * @return DS_CMD_ERR_CODEを参照
 */
DS_CMD_ERR_CODE SAGITTA_set_lisa(SAGITTA_Driver* sagitta_driver);

/**
 * @brief  SAGITTAのMatchingパラメータを設定する
 * @param  sagitta_driver    : SAGITTA_Driver構造体へのポインタ
 * @return DS_CMD_ERR_CODEを参照
 */
DS_CMD_ERR_CODE SAGITTA_set_matching(SAGITTA_Driver* sagitta_driver);

/**
 * @brief  SAGITTAのTrackingパラメータを設定する
 * @param  sagitta_driver    : SAGITTA_Driver構造体へのポインタ
 * @return DS_CMD_ERR_CODEを参照
 */
DS_CMD_ERR_CODE SAGITTA_set_tracking(SAGITTA_Driver* sagitta_driver);

/**
 * @brief  SAGITTAのValidationパラメータを設定する
 * @param  sagitta_driver    : SAGITTA_Driver構造体へのポインタ
 * @return DS_CMD_ERR_CODEを参照
 */
DS_CMD_ERR_CODE SAGITTA_set_validation(SAGITTA_Driver* sagitta_driver);

/**
 * @brief  SAGITTAのAlgoパラメータを設定する
 * @param  sagitta_driver    : SAGITTA_Driver構造体へのポインタ
 * @return DS_CMD_ERR_CODEを参照
 * @note   Algo: Algorithm. ICDでのparameter名称.
 */
DS_CMD_ERR_CODE SAGITTA_set_algo(SAGITTA_Driver* sagitta_driver);

/**
 * @brief  SAGITTAの非同期テレメパラメータを設定する
 * @param  sagitta_driver    : SAGITTA_Driver構造体へのポインタ
 * @return DS_CMD_ERR_CODEを参照
 */
DS_CMD_ERR_CODE SAGITTA_set_subscription(SAGITTA_Driver* sagitta_driver);

/**
 * @brief  SAGITTAのLog Levelパラメータを変更する
 * @param  sagitta_driver    : SAGITTA_Driver構造体へのポインタ
 * @param  param_idx          : 同一パラメータID内のidx(0起算)
 * @param  value              : 変更後の値
 * @return DS_CMD_ERR_CODEを参照
 */
DS_CMD_ERR_CODE SAGITTA_change_log_level(SAGITTA_Driver* sagitta_driver, uint8_t param_idx, float value);

/**
 * @brief  SAGITTAのLimitsパラメータを変更する
 * @param  sagitta_driver    : SAGITTA_Driver構造体へのポインタ
 * @param  param_idx          : 同一パラメータID内のidx(0起算)
 * @param  value              : 変更後の値
 * @return DS_CMD_ERR_CODEを参照
 */
DS_CMD_ERR_CODE SAGITTA_change_limits(SAGITTA_Driver* sagitta_driver, uint8_t param_idx, float value);

/**
 * @brief  SAGITTAのMountingパラメータを変更する
 * @param  sagitta_driver    : SAGITTA_Driver構造体へのポインタ
 * @param  param_idx          : 同一パラメータID内のidx(0起算)
 * @param  value              : 変更後の値
 * @return DS_CMD_ERR_CODEを参照
 */
DS_CMD_ERR_CODE SAGITTA_change_mounting(SAGITTA_Driver* sagitta_driver, uint8_t param_idx, float value);

/**
 * @brief  SAGITTAのCameraパラメータを変更する
 * @param  sagitta_driver    : SAGITTA_Driver構造体へのポインタ
 * @param  param_idx          : 同一パラメータID内のidx(0起算)
 * @param  value              : 変更後の値
 * @return DS_CMD_ERR_CODEを参照
 */
DS_CMD_ERR_CODE SAGITTA_change_camera(SAGITTA_Driver* sagitta_driver, uint8_t param_idx, float value);

/**
 * @brief  SAGITTAのImage Processorパラメータを変更する
 * @param  sagitta_driver    : SAGITTA_Driver構造体へのポインタ
 * @param  param_idx          : 同一パラメータID内のidx(0起算)
 * @param  value              : 変更後の値
 * @return DS_CMD_ERR_CODEを参照
 */
DS_CMD_ERR_CODE SAGITTA_change_image_processor(SAGITTA_Driver* sagitta_driver, uint8_t param_idx, float value);

/**
 * @brief  SAGITTAのCentroidingパラメータを変更する
 * @param  sagitta_driver    : SAGITTA_Driver構造体へのポインタ
 * @param  param_idx          : 同一パラメータID内のidx(0起算)
 * @param  value              : 変更後の値
 * @return DS_CMD_ERR_CODEを参照
 */
DS_CMD_ERR_CODE SAGITTA_change_centroiding(SAGITTA_Driver* sagitta_driver, uint8_t param_idx, float value);

/**
 * @brief  SAGITTAのLost in Space Algorithmパラメータを変更する
 * @param  sagitta_driver    : SAGITTA_Driver構造体へのポインタ
 * @param  param_idx          : 同一パラメータID内のidx(0起算)
 * @param  value              : 変更後の値
 * @return DS_CMD_ERR_CODEを参照
 */
DS_CMD_ERR_CODE SAGITTA_change_lisa(SAGITTA_Driver* sagitta_driver, uint8_t param_idx, float value);

/**
 * @brief  SAGITTAのMatchingパラメータを変更する
 * @param  sagitta_driver    : SAGITTA_Driver構造体へのポインタ
 * @param  param_idx          : 同一パラメータID内のidx(0起算)
 * @param  value              : 変更後の値
 * @return DS_CMD_ERR_CODEを参照
 */
DS_CMD_ERR_CODE SAGITTA_change_matching(SAGITTA_Driver* sagitta_driver, uint8_t param_idx, float value);

/**
 * @brief  SAGITTAのTrackingパラメータを変更する
 * @param  sagitta_driver    : SAGITTA_Driver構造体へのポインタ
 * @param  param_idx          : 同一パラメータID内のidx(0起算)
 * @param  value              : 変更後の値
 * @return DS_CMD_ERR_CODEを参照
 */
DS_CMD_ERR_CODE SAGITTA_change_tracking(SAGITTA_Driver* sagitta_driver, uint8_t param_idx, float value);

/**
 * @brief  SAGITTAのValidationパラメータを変更する
 * @param  sagitta_driver    : SAGITTA_Driver構造体へのポインタ
 * @param  param_idx          : 同一パラメータID内のidx(0起算)
 * @param  value              : 変更後の値
 * @return DS_CMD_ERR_CODEを参照
 */
DS_CMD_ERR_CODE SAGITTA_change_validation(SAGITTA_Driver* sagitta_driver, uint8_t param_idx, float value);

/**
 * @brief  SAGITTAのAlgoパラメータを変更する
 * @param  sagitta_driver    : SAGITTA_Driver構造体へのポインタ
 * @param  param_idx          : 同一パラメータID内のidx(0起算)
 * @param  value              : 変更後の値
 * @return DS_CMD_ERR_CODEを参照
 * @note   Algo: Algorithm. ICDでのparameter名称.
 */
DS_CMD_ERR_CODE SAGITTA_change_algo(SAGITTA_Driver* sagitta_driver, uint8_t param_idx, float value);

/**
 * @brief  SAGITTAの非同期テレメパラメータを変更する
 * @param  sagitta_driver    : SAGITTA_Driver構造体へのポインタ
 * @param  param_idx          : 同一パラメータID内のidx(0起算)
 * @param  value              : 変更後の値
 * @return DS_CMD_ERR_CODEを参照
 */
DS_CMD_ERR_CODE SAGITTA_change_subscription(SAGITTA_Driver* sagitta_driver, uint8_t param_idx, float value);

/**
 * @brief  SAGITTAのパラメータを読み取る
 * @param  sagitta_driver    : SAGITTA_Driver構造体へのポインタ
 * @param  parameter_id       : PARAMETER ID
 * @return DS_CMD_ERR_CODEを参照
 */
DS_CMD_ERR_CODE SAGITTA_read_parameter(SAGITTA_Driver* sagitta_driver, const uint8_t parameter_id);

/**
 * @brief  座標変換行列設定関数
 * @param  sagitta_driver  : SAGITTA_Driver構造体へのポインタ
 * @param  q_c2b            : コンポ座標からボディ座標への座標変換Quaternion
 * @return C2A_MATH_ERRORに準じる
 */
C2A_MATH_ERROR SAGITTA_set_frame_transform_c2b(SAGITTA_Driver* sagitta_driver, const Quaternion q_c2b);

#endif
