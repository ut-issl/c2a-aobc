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

#define SAGITTA_TELEMETRY_HISTOGRAM_LENGTH                (9)
#define SAGITTA_TELEMETRY_BLOBS_LENGTH                    (8)
#define SAGITTA_TELEMETRY_CENTROIDS_LENGTH                (16)
#define SAGITTA_TELEMETRY_MATCHED_CENTROIDS_LENGTH        (16)
#define SAGITTA_TELEMETRY_BLOB_STATS_LENGTH               (16)
#define SAGITTA_PARAMETER_LOG_LEVEL_LENGTH                (16)
#define SAGITTA_PARAMETER_LIMITS_LENGTH                   (10)
#define SAGITTA_PARAMETER_DISTORTION_LENGTH               (8)
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
  SAGITTA_PARAMETER_ID_DISTORTION = 8,
  SAGITTA_PARAMETER_ID_CAMERA = 9,
  SAGITTA_PARAMETER_ID_IMAGE_PROCESSOR = 10,
  SAGITTA_PARAMETER_ID_CENTROIDING = 11,
  SAGITTA_PARAMETER_ID_LISA = 12,
  SAGITTA_PARAMETER_ID_MATCHING = 13,
  SAGITTA_PARAMETER_ID_TRACKING = 14,
  SAGITTA_PARAMETER_ID_VALIDATION = 15,
  SAGITTA_PARAMETER_ID_ALGO = 16,
  SAGITTA_PARAMETER_ID_SUBSCRIPTION = 18,
  SAGITTA_PARAMETER_ID_AUTO_THRESHOLD = 23,
  SAGITTA_PARAMETER_ID_FAST_LISA = 25,
  SAGITTA_PARAMETER_ID_NOISE_LIMITS = 30,
  SAGITTA_PARAMETER_ID_BLOB_FILTER = 31
} SAGITTA_PARAMETER_ID;

/**
 * @enum   SAGITTA_TLM_ID
 * @brief  テレメ要求コマンド・テレメ解釈に用いる受信モード
 * @note   uint8_tを想定
 */
typedef enum
{
  SAGITTA_TLM_ID_POWER = 11,
  SAGITTA_TLM_ID_SOLUTION = 24,
  SAGITTA_TLM_ID_TEMPERATURE = 27,
  SAGITTA_TLM_ID_HISTOGRAM = 28,
  SAGITTA_TLM_ID_BLOBS = 36,
  SAGITTA_TLM_ID_CENTROIDS = 37,
  SAGITTA_TLM_ID_AUTO_BLOB = 39,
  SAGITTA_TLM_ID_MATCHED_CENTROIDS = 40,
  SAGITTA_TLM_ID_BLOB_STATS = 49
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
 * @struct SAGITTA_TELEMETRY_POWER
 * @brief  SagittaのPowerテレメトリを格納する
 */
typedef struct
{
  float mcu_current_A;       //!< Current used by the MCU [A]
  float mcu_voltage_V;       //!< Voltage over the MCU [V]
  float fpga_core_current_A; //!< Current used by the FPGA core [A]
  float fpga_core_voltage_V; //!< Voltage over the FPGA core [V]
  float fpga_18_current_A;   //!< Current used by the FPGA 1.8 V line [A]
  float fpga_18_voltage_V;   //!< Voltage over the FPGA 1.8 V line [V]
  float fpga_25_current_A;   //!< Current used by the FPGA 2.5 V line [A]
  float fpga_25_voltage_V;   //!< Voltage over the FPGA 2.5 V line [V]
  float cmv_21_current_A;    //!< Current used by the CMV 2.1 V line [A]
  float cmv_21_voltage_V;    //!< Voltage over the CMV 2.1 V line [V]
  float cmv_pix_current_A;   //!< Current used by the CMV VDDPix [A]
  float cmv_pix_voltage_V;   //!< Voltage over the CMV VDDPix [V]
  float cmv_33_current_A;    //!< Current used by the CMV VDD 3.3 V line [A]
  float cmv_33_voltage_V;    //!< Voltage over the CMV VDD 3.3 V line [V]
  float cmv_res_current_A;   //!< Current used by the CMV VResh [A]
  float cmv_res_voltage_V;   //!< Voltage over the CMV Resh [V]
} SAGITTA_TELEMETRY_POWER;

/**
 * @struct SAGITTA_TELEMETRY_SOLUTION
 * @brief  SagittaのSolutionテレメトリを格納する
 */
typedef struct
{
  Quaternion quaternion_i2c;       //!< Quaternion ECI -> Component Frame
  float track_confidence;          //!< Tracker Confidence Value
  Quaternion track_quaternion_i2c; //!< Quaternion tracking solution
  uint8_t num_stars_removed;       //!< Number of stars removed from trackgin solution
  uint8_t num_stars_centroided;    //!< Number of stars centroided
  uint8_t num_stars_matched;       //!< Number of stars matched with db image
  Quaternion lisa_quaternion_i2c;  //!< Quaternion LISA solution
  float lisa_percentage_close;     //!< Percentage of close stars in LISA solution
  uint8_t num_stars_lisa_close;    //!< Number of close stars in LISA solution
  uint8_t star_tracker_mode;       //!< Star tracker state: LISA = 1, Tracking = 0
  uint8_t is_valid_quaternion;     //!< Status of attitude determination (0: not valid, 1: valid)
  uint32_t stable_count;           //!< Number of consecutive stable solutions
  SAGITTA_SOLUTION_STRATEGY solution_strategy; //!< Algorithm path that was followed to determine the attitude
} SAGITTA_TELEMETRY_SOLUTION;

/**
 * @struct SAGITTA_TELEMETRY_TEMPERATURE
 * @brief  SagittaのTemperatureテレメトリを格納する
 */
typedef struct
{
  float mcu_degC;  //!< Temperature of the MCU
  float cmos_degC; //!< Temperature of the image sensor
  float fpga_degC; //!< Temperature of the FPGA
} SAGITTA_TELEMETRY_TEMPERATURE;

/**
 * @struct SAGITTA_TELEMETRY_BLOBS
 * @brief  SagittaのBlobsテレメトリを格納する
 * @note   blob: a collection of pixels with a value higher than a specified threshold level
 */
typedef struct
{
  uint16_t count;              //!< Number of detected blobs
  uint16_t count_used;         //!< Number of detected blobs excluding duplicates
  uint16_t four_lines_skipped; //!< Number of 4lines that were skipped in the image to speed up the blob algorithm
  uint16_t x_coordinate[SAGITTA_TELEMETRY_BLOBS_LENGTH]; //!< x coordinate of blob 1-8
  uint16_t y_coordinate[SAGITTA_TELEMETRY_BLOBS_LENGTH]; //!< y coordinate of blob 1-8
} SAGITTA_TELEMETRY_BLOBS;

/**
 * @struct SAGITTA_TELEMETRY_CENTROIDS
 * @brief  SagittaのCentroidsテレメトリを格納する
 */
typedef struct
{
  uint16_t count;
  float x_coordinate[SAGITTA_TELEMETRY_CENTROIDS_LENGTH];
  float y_coordinate[SAGITTA_TELEMETRY_CENTROIDS_LENGTH];
  uint8_t magnitude[SAGITTA_TELEMETRY_CENTROIDS_LENGTH];
} SAGITTA_TELEMETRY_CENTROIDS;

/**
 * @struct SAGITTA_TELEMETRY_MATCHED_CENTROIDS
 * @brief  SagittaのMatchedCentroidsテレメトリを格納する
 */
typedef struct
{
  uint8_t count; //!< Number of matched centroids
  uint32_t id[SAGITTA_TELEMETRY_MATCHED_CENTROIDS_LENGTH];              //!< Star ID number of centroid
  float x_coordinate[SAGITTA_TELEMETRY_MATCHED_CENTROIDS_LENGTH];       //!< x-coordinate of matched centroid
  float y_coordinate[SAGITTA_TELEMETRY_MATCHED_CENTROIDS_LENGTH];       //!< y-coordinate of matched centroid
  float error_x_coordinate[SAGITTA_TELEMETRY_MATCHED_CENTROIDS_LENGTH]; //!< x-coordinate error of matched centroid
  float error_y_coordinate[SAGITTA_TELEMETRY_MATCHED_CENTROIDS_LENGTH]; //!< y-coordinate error of matched centroid
} SAGITTA_TELEMETRY_MATCHED_CENTROIDS;

/**
 * @struct SAGITTA_TELEMETRY_BLOB_STATS
 * @brief  SagittaのBlobStatsテレメトリを格納する
 * @note   region: 4x4 grid of the image (00, 01, 02, 03, 10, 11, 12, 13, 20, 21, 22, 23, 30, 31, 32, 33)
 */
typedef struct
{
  uint8_t noise_limit[SAGITTA_TELEMETRY_BLOB_STATS_LENGTH];          //!< Noise limit in region [0-3][0-3]
  uint8_t brightness_threshold[SAGITTA_TELEMETRY_BLOB_STATS_LENGTH]; //!< Brightness threshold in region [0-3][0-3]
  uint8_t num_valid_blobs[SAGITTA_TELEMETRY_BLOB_STATS_LENGTH];      //!< Number of of valid blobs in region [0-3][0-3]
  uint8_t num_overflows[SAGITTA_TELEMETRY_BLOB_STATS_LENGTH];        //!< Number of overflows in region [0-3][0-3]
} SAGITTA_TELEMETRY_BLOB_STATS;

/**
 * @struct SAGITTA_TELEMETRY
 * @brief  Sagittaのテレメトリを格納する
 */
typedef struct
{
  SAGITTA_TELEMETRY_POWER power;
  SAGITTA_TELEMETRY_SOLUTION solution;
  SAGITTA_TELEMETRY_TEMPERATURE temperature;
  uint32_t histogram_a_pix[SAGITTA_TELEMETRY_HISTOGRAM_LENGTH]; //!< Number of pixel values in section A
  uint32_t histogram_b_pix[SAGITTA_TELEMETRY_HISTOGRAM_LENGTH]; //!< Number of pixel values in section B
  uint32_t histogram_c_pix[SAGITTA_TELEMETRY_HISTOGRAM_LENGTH]; //!< Number of pixel values in section C
  uint32_t histogram_d_pix[SAGITTA_TELEMETRY_HISTOGRAM_LENGTH]; //!< Number of pixel values in section D
  SAGITTA_TELEMETRY_BLOBS blobs;
  SAGITTA_TELEMETRY_CENTROIDS centroids;
  float auto_blob_threshold; //!< Automatically determined signal threshold value used in the blob algorithm
  SAGITTA_TELEMETRY_MATCHED_CENTROIDS matched_centroids;
  SAGITTA_TELEMETRY_BLOB_STATS blob_stats;
} SAGITTA_TELEMETRY;

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
 * @struct SAGITTA_PARAMETER_DISTORTION
 * @brief  SagittaのDISTORTIONパラメータを格納する
 */
typedef struct
{
  uint8_t mode; //!< 0: No compensation, 1: Hershel polynomial function, 2: Radial polynomial function
  float k0_coefficient[SAGITTA_PARAMETER_DISTORTION_LENGTH]; //!< coefficient of the polynomial function for x coordinate
  float h0_coefficient[SAGITTA_PARAMETER_DISTORTION_LENGTH]; //!< coefficient of the polynomial function for y coordinate
} SAGITTA_PARAMETER_DISTORTION;

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
 * @struct SAGITTA_PARAMETER_AUTO_THRESHOLD
 * @brief  SagittaのAUTO_THRESHOLDパラメータを格納する
 */
typedef struct
{
  uint8_t mode;
  uint8_t desired_blobs_count;
  uint16_t min_threshold;
  uint16_t max_threshold;
  float threshold_kp;
} SAGITTA_PARAMETER_AUTO_THRESHOLD;

/**
 * @struct SAGITTA_PARAMETER_FAST_LISA
 * @brief  SagittaのFAST_LISAパラメータを格納する
 */
typedef struct
{
  float limit_angle;    //!< Limit on angle of the triplets for fastLISA
  float limit_distance; //!< Limit on angle of the triplets for fastLISA
} SAGITTA_PARAMETER_FAST_LISA;

/**
 * @struct SAGITTA_PARAMETER_NOISE_LIMITS
 * @brief  SagittaのNoiseLimitsパラメータを格納する
 */
typedef struct
{
  uint16_t limit1; //!< Noise threshold 1 for blob detection
  uint16_t limit2; //!< Noise threshold 2 for blob detection
  uint16_t min;    //!< Minimum value
  uint16_t max;    //!< Maximum value
} SAGITTA_PARAMETER_NOISE_LIMITS;

/**
 * @struct SAGITTA_PARAMETER_BLOB_FILTER
 * @brief  SagittaのBlobFilterパラメータを格納する
 */
typedef struct
  {
  uint64_t max_oflow;   //!< Maximum number of blobs for all regions combined in one value
  uint64_t max_open;    //!< Maximum number of open blobs for all regions combined in one value
  uint64_t max_simopen; //!< Maximum number of simultaneous open blobs for all regions combined in one value
  uint64_t max_valid;   //!< Maximum number of valid blobs for all regions combined in one value
  uint64_t desired;     //!< Desired number of blobs for all regions combined in one value
  uint8_t max_width;    //!< Maximum width of a blob
  uint8_t max_height;   //!< Maximum height of a blob
  uint8_t max_count;    //!< Maximum pixel counter value of a blob
} SAGITTA_PARAMETER_BLOB_FILTER;

/**
 * @struct SAGITTA_Parameter
 * @brief  Sagittaの内部パラメータを格納する
 */
typedef struct
{
  uint8_t log_level[SAGITTA_PARAMETER_LOG_LEVEL_LENGTH];
  SAGITTA_PARAMETER_LIMITS limits;
  Quaternion mounting;
  SAGITTA_PARAMETER_DISTORTION distortion;
  SAGITTA_PARAMETER_CAMERA camera;
  SAGITTA_PARAMETER_IMAGE_PROCESSOR image_processor;
  SAGITTA_PARAMETER_CENTROIDING centroiding;
  SAGITTA_PARAMETER_LISA lisa;
  SAGITTA_PARAMETER_MATCHING matching;
  SAGITTA_PARAMETER_TRACKING tracking;
  SAGITTA_PARAMETER_VALIDATION validation;
  SAGITTA_PARAMETER_ALGO algo;
  uint8_t subscription[SAGITTA_PARAMETER_SUBSCRIPTION_LENGTH];
  SAGITTA_PARAMETER_AUTO_THRESHOLD auto_threshold;
  SAGITTA_PARAMETER_FAST_LISA fast_lisa;
  SAGITTA_PARAMETER_NOISE_LIMITS noise_limits;
  SAGITTA_PARAMETER_BLOB_FILTER blob_filter;
} SAGITTA_PARAMETER;

/**
 * @struct SAGITTA_Info
 * @brief  Sagittaのモードや変数などを格納する
 */
typedef struct
{
  Quaternion quaternion_i2b;      //!< Quaternion ECI -> Body
  Quaternion frame_transform_c2b; //!< frame transformation quaternion from component frame to body frame

  SAGITTA_TLM_TYPE tlm_type;         //!< テレメのタイプ
  uint8_t tlm_id;                    //!< テレメに含まれるparameter, action, telemetryのID
  uint8_t tlm_status;                //!< テレメに含まれるset parameter, action, telemetryのstatus
  uint32_t unix_time_ms;             //!< Unix time [ms]
  SAGITTA_REC_ERR_CODE err_status;   //!< Receive error status
  uint32_t xxhash;                   //!< 受信したxxhash
  SAGITTA_XXHASH_STATE xxhash_state; //!< 受信したxxhashが正しいか
  SAGITTA_TELEMETRY telemetry;       //!< テレメ
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
 * @brief  SAGITTAのDriver_Super初期化
 * @param  sagitta_driver    : 初期化するSAGITTA_Driver構造体へのポインタ
 * @param  rx_buffer: 受信バッファ
 * @return DS_INIT_ERR_CODE
 */
DS_ERR_CODE SAGITTA_DS_init(SAGITTA_Driver* sagitta_driver, DS_StreamRecBuffer* rx_buffer);
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
 * @brief  SAGITTAのDistortionパラメータを設定する
 * @param  sagitta_driver    : SAGITTA_Driver構造体へのポインタ
 * @return DS_CMD_ERR_CODEを参照
 */
DS_CMD_ERR_CODE SAGITTA_set_distortion(SAGITTA_Driver* sagitta_driver);

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
 * @brief  SAGITTAのAuto Thresholdパラメータを設定する
 * @param  sagitta_driver    : SAGITTA_Driver構造体へのポインタ
 * @return DS_CMD_ERR_CODEを参照
 */
DS_CMD_ERR_CODE SAGITTA_set_auto_threshold(SAGITTA_Driver* sagitta_driver);

/**
 * @brief  SAGITTAのFast LISAパラメータを設定する
 * @param  sagitta_driver    : SAGITTA_Driver構造体へのポインタ
 * @return DS_CMD_ERR_CODEを参照
 */
DS_CMD_ERR_CODE SAGITTA_set_fast_lisa(SAGITTA_Driver* sagitta_driver);

/**
 * @brief  SAGITTAのNoiseLimitsパラメータを設定する
 * @param  sagitta_driver    : SAGITTA_Driver構造体へのポインタ
 * @return DS_CMD_ERR_CODEを参照
 */
DS_CMD_ERR_CODE SAGITTA_set_noise_limits(SAGITTA_Driver* sagitta_driver);

/**
 * @brief  SAGITTAのBlobFilterパラメータを設定する
 * @param  sagitta_driver    : SAGITTA_Driver構造体へのポインタ
 * @return DS_CMD_ERR_CODEを参照
 */
DS_CMD_ERR_CODE SAGITTA_set_blob_filter(SAGITTA_Driver* sagitta_driver);

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
 * @brief  SAGITTAのDistortionパラメータを変更する
 * @param  sagitta_driver    : SAGITTA_Driver構造体へのポインタ
 * @param  param_idx          : 同一パラメータID内のidx(0起算)
 * @param  value              : 変更後の値
 * @return DS_CMD_ERR_CODEを参照
 */
DS_CMD_ERR_CODE SAGITTA_change_distortion(SAGITTA_Driver* sagitta_driver, uint8_t param_idx, float value);

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
 * @brief  SAGITTAのAuto Thresholdパラメータを変更する
 * @param  sagitta_driver    : SAGITTA_Driver構造体へのポインタ
 * @param  param_idx          : 同一パラメータID内のidx(0起算)
 * @param  value              : 変更後の値
 * @return DS_CMD_ERR_CODEを参照
 */
DS_CMD_ERR_CODE SAGITTA_change_auto_threshold(SAGITTA_Driver* sagitta_driver, uint8_t param_idx, float value);

/**
 * @brief  SAGITTAのFast LISAパラメータを変更する
 * @param  sagitta_driver    : SAGITTA_Driver構造体へのポインタ
 * @param  param_idx          : 同一パラメータID内のidx(0起算)
 * @param  value              : 変更後の値
 * @return DS_CMD_ERR_CODEを参照
 */
DS_CMD_ERR_CODE SAGITTA_change_fast_lisa(SAGITTA_Driver* sagitta_driver, uint8_t param_idx, float value);

/**
 * @brief  SAGITTAのNoiseLimitsパラメータを変更する
 * @param  sagitta_driver    : SAGITTA_Driver構造体へのポインタ
 * @param  param_idx          : 同一パラメータID内のidx(0起算)
 * @param  value              : 変更後の値
 * @return DS_CMD_ERR_CODEを参照
 */
DS_CMD_ERR_CODE SAGITTA_change_noise_limits(SAGITTA_Driver* sagitta_driver, uint8_t param_idx, float value);

/**
 * @brief  SAGITTAのBlobFilterパラメータを変更する
 * @param  sagitta_driver    : SAGITTA_Driver構造体へのポインタ
 * @param  param_idx          : 同一パラメータID内のidx(0起算)
 * @param  value              : 変更後の値
 * @return DS_CMD_ERR_CODEを参照
 */
DS_CMD_ERR_CODE SAGITTA_change_blob_filter(SAGITTA_Driver* sagitta_driver, uint8_t param_idx, float value);

/**
 * @brief  SAGITTAのパラメータを読み取る
 * @param  sagitta_driver    : SAGITTA_Driver構造体へのポインタ
 * @param  parameter_id       : PARAMETER ID
 * @return DS_CMD_ERR_CODEを参照
 */
DS_CMD_ERR_CODE SAGITTA_read_parameter(SAGITTA_Driver* sagitta_driver, const uint8_t parameter_id);

/**
 * @brief  同期式でテレメトリを受け取る
 * @param  sagitta_driver    : SAGITTA_Driver構造体へのポインタ
 * @param  telemetry_id       : TELEMETRY ID
 * @return DS_CMD_ERR_CODEを参照
 */
DS_CMD_ERR_CODE SAGITTA_request_tlm(SAGITTA_Driver* sagitta_driver, uint8_t tlm_id);

/**
 * @brief  座標変換行列設定関数
 * @param  sagitta_driver  : SAGITTA_Driver構造体へのポインタ
 * @param  q_c2b            : コンポ座標からボディ座標への座標変換Quaternion
 * @return C2A_MATH_ERRORに準じる
 */
C2A_MATH_ERROR SAGITTA_set_frame_transform_c2b(SAGITTA_Driver* sagitta_driver, const Quaternion q_c2b);

#endif
