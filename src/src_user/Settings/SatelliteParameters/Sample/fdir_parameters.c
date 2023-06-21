/**
 * @file  fdir_parameters.c
 * @brief FDIRに関する衛星固有パラメータを管理する
 */

#include <src_user/Settings/SatelliteParameters/fdir_parameters.h>

// Mode Manager
const float FDIR_PARAMETERS_ang_vel_conv_limit_rad_s = PHYSICAL_CONST_degree_to_radian(0.5f);
const float FDIR_PARAMETERS_ang_vel_conv_time_limit_s = 5.0f * 60.0f;

const float FDIR_PARAMETERS_ang_vel_norm_increase_limit_rad_s = PHYSICAL_CONST_degree_to_radian(0.5f);
const float FDIR_PARAMETERS_ang_vel_anomaly_detection_period_s = 200.0f;

const float FDIR_PARAMETERS_sun_angle_div_limit_rad = PHYSICAL_CONST_degree_to_radian(45.0f);
const float FDIR_PARAMETERS_sun_angle_div_time_limit_s = 50.0f * 60.0f;

const float FDIR_PARAMETERS_three_axis_div_limit_rad = PHYSICAL_CONST_degree_to_radian(30.0f);
const float FDIR_PARAMETERS_three_axis_div_time_limit_s = 5.0f * 60.0f;

const float FDIR_PARAMETERS_sun_invisible_time_limit_s = 50.0f * 60.0f;
const float FDIR_PARAMETERS_stt_invisible_time_limit_s = 10.0f * 60.0f;

// S/W over current detection
const uint16_t FDIR_PARAMETERS_sw_oc_threshold_pic_mA = 200;
const uint16_t FDIR_PARAMETERS_sw_oc_threshold_stim210_mA = 1000;
const uint16_t FDIR_PARAMETERS_sw_oc_threshold_sagitta_mA = 500;
const uint16_t FDIR_PARAMETERS_sw_oc_threshold_oem7600_mA = 1000;
const uint16_t FDIR_PARAMETERS_sw_oc_threshold_rm3100_mA = 200;
const uint16_t FDIR_PARAMETERS_sw_oc_threshold_nanossoc_d60_mA = 150;
const uint16_t FDIR_PARAMETERS_sw_oc_threshold_mtq_mA = 2000;
const uint16_t FDIR_PARAMETERS_sw_oc_threshold_rw0003_x_mA = 2000;
const uint16_t FDIR_PARAMETERS_sw_oc_threshold_rw0003_y_mA = 2000;
const uint16_t FDIR_PARAMETERS_sw_oc_threshold_rw0003_z_mA = 2000;

// H/W over current detection
// If the measured voltage is smaller than the following values, we assume that the H/W OC protection was executed by the INA260.
const float FDIR_PARAMETERS_hw_oc_detection_threshold_pic_V = 0.5f;
const float FDIR_PARAMETERS_hw_oc_detection_threshold_stim210_V = 0.5f;
const float FDIR_PARAMETERS_hw_oc_detection_threshold_sagitta_V = 0.5f;
const float FDIR_PARAMETERS_hw_oc_detection_threshold_oem7600_V = 1.0f;
const float FDIR_PARAMETERS_hw_oc_detection_threshold_rm3100_V = 0.5f;
const float FDIR_PARAMETERS_hw_oc_detection_threshold_nanossoc_d60_V = 0.5f;
const float FDIR_PARAMETERS_hw_oc_detection_threshold_mtq_V = 0.5f;
const float FDIR_PARAMETERS_hw_oc_detection_threshold_rw0003_x_V = 0.5f;
const float FDIR_PARAMETERS_hw_oc_detection_threshold_rw0003_y_V = 0.5f;
const float FDIR_PARAMETERS_hw_oc_detection_threshold_rw0003_z_V = 0.5f;