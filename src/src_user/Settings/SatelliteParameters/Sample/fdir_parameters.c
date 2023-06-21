/**
 * @file  fdir_parameters.c
 * @brief FDIRに関する衛星固有パラメータを管理する
 */

#include <src_user/Settings/SatelliteParameters/fdir_parameters.h>

// Mode Manager
// Bdot
const float FDIR_PARAMETERS_ang_vel_conv_limit_rad_s = PHYSICAL_CONST_degree_to_radian(0.5f);
const float FDIR_PARAMETERS_ang_vel_conv_time_limit_s = 5.0f * 60.0f;
const float FDIR_PARAMETERS_ang_vel_norm_increase_limit_rad_s = PHYSICAL_CONST_degree_to_radian(0.5f);
const float FDIR_PARAMETERS_ang_vel_anomaly_detection_period_s = 200.0f;
const uint32_t FDIR_PARAMETERS_bdot_start_mode_manager_time_s = 5 * 60;
// Sun pointing control divergence anomaly
const float FDIR_PARAMETERS_sun_angle_div_limit_rad = PHYSICAL_CONST_degree_to_radian(45.0f);
const float FDIR_PARAMETERS_sun_angle_div_time_limit_s = 50.0f * 60.0f;
extern const uint32_t FDIR_PARAMETERS_sun_pointing_start_mode_manager_time_s = 25 * 60;
// Rough three axis control with MTQ divergence anomaly
const float FDIR_PARAMETERS_rough_three_axis_mtq_div_limit_rad = PHYSICAL_CONST_degree_to_radian(30.0f);
const float FDIR_PARAMETERS_rough_three_axis_mtq_div_time_limit_s = 5.0f * 60.0f;
extern const uint32_t FDIR_PARAMETERS_rough_three_axis_mtq_start_mode_manager_time_s = 35 * 60;
// Rough three axis control with RW divergence anomaly
const float FDIR_PARAMETERS_rough_three_axis_rw_div_limit_rad = PHYSICAL_CONST_degree_to_radian(10.0f);
const float FDIR_PARAMETERS_rough_three_axis_rw_div_time_limit_s = 1.0f * 60.0f;
extern const uint32_t FDIR_PARAMETERS_rough_three_axis_rw_start_mode_manager_time_s = 5 * 60;
// Fine three axis control divergence anomaly
const float FDIR_PARAMETERS_fine_three_axis_div_limit_rad = PHYSICAL_CONST_degree_to_radian(5.0f);
const float FDIR_PARAMETERS_fine_three_axis_div_time_limit_s = 1.0f * 60.0f;
extern const uint32_t FDIR_PARAMETERS_fine_three_axis_start_mode_manager_time_s = 5 * 60;
// Sensor invisible anomaly
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
// Event handler settings
// STIM210
const uint16_t FDIR_PARAMETERS_sw_oc_eh_reset_count_threshold_stim210 = 10;
const uint32_t FDIR_PARAMETERS_sw_oc_eh_reset_time_threshold_stim210_ms = 5000;
const uint16_t FDIR_PARAMETERS_sw_oc_eh_power_off_count_threshold_stim210 = 5;
const uint32_t FDIR_PARAMETERS_sw_oc_eh_power_off_time_threshold_stim210_ms = 0;
// Sagitta
const uint16_t FDIR_PARAMETERS_sw_oc_eh_reset_count_threshold_sagitta = 10;
const uint32_t FDIR_PARAMETERS_sw_oc_eh_reset_time_threshold_sagitta_ms = 5000;
const uint16_t FDIR_PARAMETERS_sw_oc_eh_power_off_count_threshold_sagitta = 5;
const uint32_t FDIR_PARAMETERS_sw_oc_eh_power_off_time_threshold_sagitta_ms = 0;
// OEM7600
const uint16_t FDIR_PARAMETERS_sw_oc_eh_reset_count_threshold_oem7600 = 10;
const uint32_t FDIR_PARAMETERS_sw_oc_eh_reset_time_threshold_oem7600_ms = 5000;
const uint16_t FDIR_PARAMETERS_sw_oc_eh_power_off_count_threshold_oem7600 = 5;
const uint32_t FDIR_PARAMETERS_sw_oc_eh_power_off_time_threshold_oem7600_ms = 0;
// RM3100
const uint16_t FDIR_PARAMETERS_sw_oc_eh_reset_count_threshold_rm3100 = 10;
const uint32_t FDIR_PARAMETERS_sw_oc_eh_reset_time_threshold_rm3100_ms = 5000;
const uint16_t FDIR_PARAMETERS_sw_oc_eh_power_off_count_threshold_rm3100 = 5;
const uint32_t FDIR_PARAMETERS_sw_oc_eh_power_off_time_threshold_rm3100_ms = 0;
// nanoSSOC D60
const uint16_t FDIR_PARAMETERS_sw_oc_eh_reset_count_threshold_nanossoc_d60 = 10;
const uint32_t FDIR_PARAMETERS_sw_oc_eh_reset_time_threshold_nanossoc_d60_ms = 5000;
const uint16_t FDIR_PARAMETERS_sw_oc_eh_power_off_count_threshold_nanossoc_d60 = 5;
const uint32_t FDIR_PARAMETERS_sw_oc_eh_power_off_time_threshold_nanossoc_d60_ms = 0;
// MTQ
const uint16_t FDIR_PARAMETERS_sw_oc_eh_reset_count_threshold_mtq = 10;
const uint32_t FDIR_PARAMETERS_sw_oc_eh_reset_time_threshold_mtq_ms = 5000;
const uint16_t FDIR_PARAMETERS_sw_oc_eh_power_off_count_threshold_mtq = 5;
const uint32_t FDIR_PARAMETERS_sw_oc_eh_power_off_time_threshold_mtq_ms = 0;
// RW
const uint16_t FDIR_PARAMETERS_sw_oc_eh_reset_count_threshold_rw0003_x = 10;
const uint32_t FDIR_PARAMETERS_sw_oc_eh_reset_time_threshold_rw0003_x_ms = 5000;
const uint16_t FDIR_PARAMETERS_sw_oc_eh_power_off_count_threshold_rw0003_x = 5;
const uint32_t FDIR_PARAMETERS_sw_oc_eh_power_off_time_threshold_rw0003_x_ms = 0;
const uint16_t FDIR_PARAMETERS_sw_oc_eh_reset_count_threshold_rw0003_y = 10;
const uint32_t FDIR_PARAMETERS_sw_oc_eh_reset_time_threshold_rw0003_y_ms = 5000;
const uint16_t FDIR_PARAMETERS_sw_oc_eh_power_off_count_threshold_rw0003_y = 5;
const uint32_t FDIR_PARAMETERS_sw_oc_eh_power_off_time_threshold_rw0003_y_ms = 0;
const uint16_t FDIR_PARAMETERS_sw_oc_eh_reset_count_threshold_rw0003_z = 10;
const uint32_t FDIR_PARAMETERS_sw_oc_eh_reset_time_threshold_rw0003_z_ms = 5000;
const uint16_t FDIR_PARAMETERS_sw_oc_eh_power_off_count_threshold_rw0003_z = 5;
const uint32_t FDIR_PARAMETERS_sw_oc_eh_power_off_time_threshold_rw0003_z_ms = 0;

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
// Event handler settings
// STIM210
const uint16_t FDIR_PARAMETERS_hw_oc_eh_reset_count_threshold_stim210 = 10;
const uint32_t FDIR_PARAMETERS_hw_oc_eh_reset_time_threshold_stim210_ms = 5000;
const uint16_t FDIR_PARAMETERS_hw_oc_eh_power_off_count_threshold_stim210 = 5;
const uint32_t FDIR_PARAMETERS_hw_oc_eh_power_off_time_threshold_stim210_ms = 0;
// Sagitta
const uint16_t FDIR_PARAMETERS_hw_oc_eh_reset_count_threshold_sagitta = 10;
const uint32_t FDIR_PARAMETERS_hw_oc_eh_reset_time_threshold_sagitta_ms = 5000;
const uint16_t FDIR_PARAMETERS_hw_oc_eh_power_off_count_threshold_sagitta = 5;
const uint32_t FDIR_PARAMETERS_hw_oc_eh_power_off_time_threshold_sagitta_ms = 0;
// OEM7600
const uint16_t FDIR_PARAMETERS_hw_oc_eh_reset_count_threshold_oem7600 = 10;
const uint32_t FDIR_PARAMETERS_hw_oc_eh_reset_time_threshold_oem7600_ms = 5000;
const uint16_t FDIR_PARAMETERS_hw_oc_eh_power_off_count_threshold_oem7600 = 5;
const uint32_t FDIR_PARAMETERS_hw_oc_eh_power_off_time_threshold_oem7600_ms = 0;
// RM3100
const uint16_t FDIR_PARAMETERS_hw_oc_eh_reset_count_threshold_rm3100 = 10;
const uint32_t FDIR_PARAMETERS_hw_oc_eh_reset_time_threshold_rm3100_ms = 5000;
const uint16_t FDIR_PARAMETERS_hw_oc_eh_power_off_count_threshold_rm3100 = 5;
const uint32_t FDIR_PARAMETERS_hw_oc_eh_power_off_time_threshold_rm3100_ms = 0;
// nanoSSOC D60
const uint16_t FDIR_PARAMETERS_hw_oc_eh_reset_count_threshold_nanossoc_d60 = 10;
const uint32_t FDIR_PARAMETERS_hw_oc_eh_reset_time_threshold_nanossoc_d60_ms = 5000;
const uint16_t FDIR_PARAMETERS_hw_oc_eh_power_off_count_threshold_nanossoc_d60 = 5;
const uint32_t FDIR_PARAMETERS_hw_oc_eh_power_off_time_threshold_nanossoc_d60_ms = 0;
// MTQ
const uint16_t FDIR_PARAMETERS_hw_oc_eh_reset_count_threshold_mtq = 10;
const uint32_t FDIR_PARAMETERS_hw_oc_eh_reset_time_threshold_mtq_ms = 5000;
const uint16_t FDIR_PARAMETERS_hw_oc_eh_power_off_count_threshold_mtq = 5;
const uint32_t FDIR_PARAMETERS_hw_oc_eh_power_off_time_threshold_mtq_ms = 0;
// RW
const uint16_t FDIR_PARAMETERS_hw_oc_eh_reset_count_threshold_rw0003_x = 10;
const uint32_t FDIR_PARAMETERS_hw_oc_eh_reset_time_threshold_rw0003_x_ms = 5000;
const uint16_t FDIR_PARAMETERS_hw_oc_eh_power_off_count_threshold_rw0003_x = 5;
const uint32_t FDIR_PARAMETERS_hw_oc_eh_power_off_time_threshold_rw0003_x_ms = 0;
const uint16_t FDIR_PARAMETERS_hw_oc_eh_reset_count_threshold_rw0003_y = 10;
const uint32_t FDIR_PARAMETERS_hw_oc_eh_reset_time_threshold_rw0003_y_ms = 5000;
const uint16_t FDIR_PARAMETERS_hw_oc_eh_power_off_count_threshold_rw0003_y = 5;
const uint32_t FDIR_PARAMETERS_hw_oc_eh_power_off_time_threshold_rw0003_y_ms = 0;
const uint16_t FDIR_PARAMETERS_hw_oc_eh_reset_count_threshold_rw0003_z = 10;
const uint32_t FDIR_PARAMETERS_hw_oc_eh_reset_time_threshold_rw0003_z_ms = 5000;
const uint16_t FDIR_PARAMETERS_hw_oc_eh_power_off_count_threshold_rw0003_z = 5;
const uint32_t FDIR_PARAMETERS_hw_oc_eh_power_off_time_threshold_rw0003_z_ms = 0;
