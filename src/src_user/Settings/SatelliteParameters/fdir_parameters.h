/**
 * @file  structure_parameters.h
 * @brief 衛星構造に関する衛星固有パラメータを管理する
 */

#ifndef FDIR_PARAMETERS_H_
#define FDIR_PARAMETERS_H_

#include <src_user/Library/physical_constants.h>
#include <stdint.h>

// Mode Manager
// Bdot
extern const float FDIR_PARAMETERS_ang_vel_conv_limit_rad_s;            //!< Angular velocity convergence detection limit [rad/s]
extern const float FDIR_PARAMETERS_ang_vel_conv_time_limit_s;           //!< Angular velocity convergence detection time limit [s]
extern const float FDIR_PARAMETERS_ang_vel_norm_increase_limit_rad_s;   //!< Angular velocity increase anomaly detection limit [rad/s]
extern const float FDIR_PARAMETERS_ang_vel_anomaly_detection_period_s;  //!< Angular velocity anomaly detection period [s]
extern const uint32_t FDIR_PARAMETERS_bdot_start_mode_manager_time_s;   //!< Bdot start auto mode transition time [s]
// Sun pointing control divergence anomaly
extern const float FDIR_PARAMETERS_sun_angle_div_limit_rad;     //!< Sun pointing divergence detection limit [rad]
extern const float FDIR_PARAMETERS_sun_angle_div_time_limit_s;  //!< Sun pointing divergence detection time limit [s]
extern const uint32_t FDIR_PARAMETERS_sun_pointing_start_mode_manager_time_s;             //!< Sun pointing start auto mode transition time [s]
// Rough three axis control with MTQ divergence anomaly
extern const float FDIR_PARAMETERS_rough_three_axis_mtq_div_limit_rad;    //!< Three axis control divergence detection limit [rad]
extern const float FDIR_PARAMETERS_rough_three_axis_mtq_div_time_limit_s; //!< Three axis control divergence detection time limit [s]
extern const uint32_t FDIR_PARAMETERS_rough_three_axis_mtq_start_mode_manager_time_s;     //!< Rough Three Axis with MTQ start auto mode transition time [s]
// Rough three axis control with RW divergence anomaly
extern const float FDIR_PARAMETERS_rough_three_axis_rw_div_limit_rad;    //!< Three axis control divergence detection limit [rad]
extern const float FDIR_PARAMETERS_rough_three_axis_rw_div_time_limit_s; //!< Three axis control divergence detection time limit [s]
extern const uint32_t FDIR_PARAMETERS_rough_three_axis_rw_start_mode_manager_time_s; //!< Rough Three Axis with RW start auto mode transition time [s]
// Fine three axis control divergence anomaly
extern const float FDIR_PARAMETERS_fine_three_axis_div_limit_rad;    //!< Three axis control divergence detection limit [rad]
extern const float FDIR_PARAMETERS_fine_three_axis_div_time_limit_s; //!< Three axis control divergence detection time limit [s]
extern const uint32_t FDIR_PARAMETERS_fine_three_axis_start_mode_manager_time_s;     //!< Fine Three Axis with RW start auto mode transition time [s]
// Sensor invisible anomaly
extern const float FDIR_PARAMETERS_sun_invisible_time_limit_s;  //!< Sun invisible anomaly detection time limit [s]
extern const float FDIR_PARAMETERS_stt_invisible_time_limit_s;  //!< STT invisible anomaly detection time limit [s]


// S/W over current detection
extern const uint16_t FDIR_PARAMETERS_sw_oc_threshold_pic_mA;          //!< S/W over current detection threshold for PIC [mA]
extern const uint16_t FDIR_PARAMETERS_sw_oc_threshold_stim210_mA;      //!< S/W over current detection threshold for STIM210 [mA]
extern const uint16_t FDIR_PARAMETERS_sw_oc_threshold_sagitta_mA;      //!< S/W over current detection threshold for SAGITTA [mA]
extern const uint16_t FDIR_PARAMETERS_sw_oc_threshold_oem7600_mA;      //!< S/W over current detection threshold for OEM7600 [mA]
extern const uint16_t FDIR_PARAMETERS_sw_oc_threshold_rm3100_mA;       //!< S/W over current detection threshold for RM3100 [mA]
extern const uint16_t FDIR_PARAMETERS_sw_oc_threshold_nanossoc_d60_mA; //!< S/W over current detection threshold for nanoSSOC D60 [mA]
extern const uint16_t FDIR_PARAMETERS_sw_oc_threshold_mtq_mA;          //!< S/W over current detection threshold for MTQ [mA]
extern const uint16_t FDIR_PARAMETERS_sw_oc_threshold_rw0003_x_mA;     //!< S/W over current detection threshold for RW003 X-axis [mA]
extern const uint16_t FDIR_PARAMETERS_sw_oc_threshold_rw0003_y_mA;     //!< S/W over current detection threshold for RW003 Y-axis [mA]
extern const uint16_t FDIR_PARAMETERS_sw_oc_threshold_rw0003_z_mA;     //!< S/W over current detection threshold for RW003 Z-axis [mA]
// Event handler settings
// STIM210
extern const uint16_t FDIR_PARAMETERS_sw_oc_eh_reset_count_threshold_stim210;        //!< S/W over current detection event handler reset count threshold for STIM210
extern const uint32_t FDIR_PARAMETERS_sw_oc_eh_reset_time_threshold_stim210_ms;      //!< S/W over current detection event handler reset time threshold for STIM210 [ms]
extern const uint16_t FDIR_PARAMETERS_sw_oc_eh_power_off_count_threshold_stim210;    //!< S/W over current detection event handler power off count threshold for STIM210
extern const uint32_t FDIR_PARAMETERS_sw_oc_eh_power_off_time_threshold_stim210_ms;  //!< S/W over current detection event handler power off time threshold for STIM210 [ms]
// Sagitta
extern const uint16_t FDIR_PARAMETERS_sw_oc_eh_reset_count_threshold_sagitta;        //!< S/W over current detection event handler reset count threshold for SAGITTA
extern const uint32_t FDIR_PARAMETERS_sw_oc_eh_reset_time_threshold_sagitta_ms;      //!< S/W over current detection event handler reset time threshold for SAGITTA [ms]
extern const uint16_t FDIR_PARAMETERS_sw_oc_eh_power_off_count_threshold_sagitta;    //!< S/W over current detection event handler power off count threshold for SAGITTA
extern const uint32_t FDIR_PARAMETERS_sw_oc_eh_power_off_time_threshold_sagitta_ms;  //!< S/W over current detection event handler power off time threshold for SAGITTA [ms]
// OEM7600
extern const uint16_t FDIR_PARAMETERS_sw_oc_eh_reset_count_threshold_oem7600;        //!< S/W over current detection event handler reset count threshold for OEM7600
extern const uint32_t FDIR_PARAMETERS_sw_oc_eh_reset_time_threshold_oem7600_ms;      //!< S/W over current detection event handler reset time threshold for OEM7600 [ms]
extern const uint16_t FDIR_PARAMETERS_sw_oc_eh_power_off_count_threshold_oem7600;    //!< S/W over current detection event handler power off count threshold for OEM7600
extern const uint32_t FDIR_PARAMETERS_sw_oc_eh_power_off_time_threshold_oem7600_ms;  //!< S/W over current detection event handler power off time threshold for OEM7600 [ms]
// RM3100
extern const uint16_t FDIR_PARAMETERS_sw_oc_eh_reset_count_threshold_rm3100;       //!< S/W over current detection event handler reset count threshold for RM3100
extern const uint32_t FDIR_PARAMETERS_sw_oc_eh_reset_time_threshold_rm3100_ms;     //!< S/W over current detection event handler reset time threshold for RM3100 [ms]
extern const uint16_t FDIR_PARAMETERS_sw_oc_eh_power_off_count_threshold_rm3100;   //!< S/W over current detection event handler power off count threshold for RM3100
extern const uint32_t FDIR_PARAMETERS_sw_oc_eh_power_off_time_threshold_rm3100_ms; //!< S/W over current detection event handler power off time threshold for RM3100 [ms]
// nanoSSOC D60
extern const uint16_t FDIR_PARAMETERS_sw_oc_eh_reset_count_threshold_nanossoc_d60;       //!< S/W over current detection event handler reset count threshold for nanoSSOC D60
extern const uint32_t FDIR_PARAMETERS_sw_oc_eh_reset_time_threshold_nanossoc_d60_ms;     //!< S/W over current detection event handler reset time threshold for nanoSSOC D60 [ms]
extern const uint16_t FDIR_PARAMETERS_sw_oc_eh_power_off_count_threshold_nanossoc_d60;   //!< S/W over current detection event handler power off count threshold for nanoSSOC D60
extern const uint32_t FDIR_PARAMETERS_sw_oc_eh_power_off_time_threshold_nanossoc_d60_ms; //!< S/W over current detection event handler power off time threshold for nanoSSOC D60 [ms]
// MTQ
extern const uint16_t FDIR_PARAMETERS_sw_oc_eh_reset_count_threshold_mtq;            //!< S/W over current detection event handler reset count threshold for MTQ
extern const uint32_t FDIR_PARAMETERS_sw_oc_eh_reset_time_threshold_mtq_ms;          //!< S/W over current detection event handler reset time threshold for MTQ [ms]
extern const uint16_t FDIR_PARAMETERS_sw_oc_eh_power_off_count_threshold_mtq;            //!< S/W over current detection event handler power off count threshold for MTQ
extern const uint32_t FDIR_PARAMETERS_sw_oc_eh_power_off_time_threshold_mtq_ms;          //!< S/W over current detection event handler power off time threshold for MTQ [ms]
// RW
extern const uint16_t FDIR_PARAMETERS_sw_oc_eh_reset_count_threshold_rw0003_x;       //!< S/W over current detection event handler reset count threshold for RW0003 X
extern const uint32_t FDIR_PARAMETERS_sw_oc_eh_reset_time_threshold_rw0003_x_ms;     //!< S/W over current detection event handler reset time threshold for RW0003 X [ms]
extern const uint16_t FDIR_PARAMETERS_sw_oc_eh_power_off_count_threshold_rw0003_x;   //!< S/W over current detection event handler power off count threshold for RW0003 X
extern const uint32_t FDIR_PARAMETERS_sw_oc_eh_power_off_time_threshold_rw0003_x_ms; //!< S/W over current detection event handler power off time threshold for RW0003 X [ms]
extern const uint16_t FDIR_PARAMETERS_sw_oc_eh_reset_count_threshold_rw0003_y;       //!< S/W over current detection event handler reset count threshold for RW0003 Y
extern const uint32_t FDIR_PARAMETERS_sw_oc_eh_reset_time_threshold_rw0003_y_ms;     //!< S/W over current detection event handler reset time threshold for RW0003 Y [ms]
extern const uint16_t FDIR_PARAMETERS_sw_oc_eh_power_off_count_threshold_rw0003_y;   //!< S/W over current detection event handler power off count threshold for RW0003 Y
extern const uint32_t FDIR_PARAMETERS_sw_oc_eh_power_off_time_threshold_rw0003_y_ms; //!< S/W over current detection event handler power off time threshold for RW0003 Y [ms]
extern const uint16_t FDIR_PARAMETERS_sw_oc_eh_reset_count_threshold_rw0003_z;       //!< S/W over current detection event handler reset count threshold for RW0003 Z
extern const uint32_t FDIR_PARAMETERS_sw_oc_eh_reset_time_threshold_rw0003_z_ms;     //!< S/W over current detection event handler reset time threshold for RW0003 Z [ms]
extern const uint16_t FDIR_PARAMETERS_sw_oc_eh_power_off_count_threshold_rw0003_z;   //!< S/W over current detection event handler power off count threshold for RW0003 Z
extern const uint32_t FDIR_PARAMETERS_sw_oc_eh_power_off_time_threshold_rw0003_z_ms; //!< S/W over current detection event handler power off time threshold for RW0003 Z [ms]

// H/W over current detection
// If the measured voltage is smaller than the following values, we assume that the H/W OC protection was executed by the INA260.
extern const float FDIR_PARAMETERS_hw_oc_detection_threshold_pic_V;          //!< H/W over current detection threshold for PIC [V]
extern const float FDIR_PARAMETERS_hw_oc_detection_threshold_stim210_V;      //!< H/W over current detection threshold for STIM210 [V]
extern const float FDIR_PARAMETERS_hw_oc_detection_threshold_sagitta_V;      //!< H/W over current detection threshold for SAGITTA [V]
extern const float FDIR_PARAMETERS_hw_oc_detection_threshold_oem7600_V;      //!< H/W over current detection threshold for OEM7600 [V]
extern const float FDIR_PARAMETERS_hw_oc_detection_threshold_rm3100_V;       //!< H/W over current detection threshold for RM3100 [V]
extern const float FDIR_PARAMETERS_hw_oc_detection_threshold_nanossoc_d60_V; //!< H/W over current detection threshold for nanoSSOC D60 [V]
extern const float FDIR_PARAMETERS_hw_oc_detection_threshold_mtq_V;          //!< H/W over current detection threshold for MTQ [V]
extern const float FDIR_PARAMETERS_hw_oc_detection_threshold_rw0003_x_V;     //!< H/W over current detection threshold for RW003 X-axis [V]
extern const float FDIR_PARAMETERS_hw_oc_detection_threshold_rw0003_y_V;     //!< H/W over current detection threshold for RW003 Y-axis [V]
extern const float FDIR_PARAMETERS_hw_oc_detection_threshold_rw0003_z_V;     //!< H/W over current detection threshold for RW003 Z-axis [V]
// Event handler settings
// STIM210
extern const uint16_t FDIR_PARAMETERS_hw_oc_eh_reset_count_threshold_stim210;        //!< H/W over current detection event handler reset count threshold for STIM210
extern const uint32_t FDIR_PARAMETERS_hw_oc_eh_reset_time_threshold_stim210_ms;      //!< H/W over current detection event handler reset time threshold for STIM210 [ms]
extern const uint16_t FDIR_PARAMETERS_hw_oc_eh_power_off_count_threshold_stim210;    //!< H/W over current detection event handler power off count threshold for STIM210
extern const uint32_t FDIR_PARAMETERS_hw_oc_eh_power_off_time_threshold_stim210_ms;  //!< H/W over current detection event handler power off time threshold for STIM210 [ms]
// Sagitta
extern const uint16_t FDIR_PARAMETERS_hw_oc_eh_reset_count_threshold_sagitta;        //!< H/W over current detection event handler reset count threshold for SAGITTA
extern const uint32_t FDIR_PARAMETERS_hw_oc_eh_reset_time_threshold_sagitta_ms;      //!< H/W over current detection event handler reset time threshold for SAGITTA [ms]
extern const uint16_t FDIR_PARAMETERS_hw_oc_eh_power_off_count_threshold_sagitta;    //!< H/W over current detection event handler power off count threshold for SAGITTA
extern const uint32_t FDIR_PARAMETERS_hw_oc_eh_power_off_time_threshold_sagitta_ms;  //!< H/W over current detection event handler power off time threshold for SAGITTA [ms]
// OEM7600
extern const uint16_t FDIR_PARAMETERS_hw_oc_eh_reset_count_threshold_oem7600;        //!< H/W over current detection event handler reset count threshold for OEM7600
extern const uint32_t FDIR_PARAMETERS_hw_oc_eh_reset_time_threshold_oem7600_ms;      //!< H/W over current detection event handler reset time threshold for OEM7600 [ms]
extern const uint16_t FDIR_PARAMETERS_hw_oc_eh_power_off_count_threshold_oem7600;    //!< H/W over current detection event handler power off count threshold for OEM7600
extern const uint32_t FDIR_PARAMETERS_hw_oc_eh_power_off_time_threshold_oem7600_ms;  //!< H/W over current detection event handler power off time threshold for OEM7600 [ms]
// RM3100
extern const uint16_t FDIR_PARAMETERS_hw_oc_eh_reset_count_threshold_rm3100;       //!< H/W over current detection event handler reset count threshold for RM3100
extern const uint32_t FDIR_PARAMETERS_hw_oc_eh_reset_time_threshold_rm3100_ms;     //!< H/W over current detection event handler reset time threshold for RM3100 [ms]
extern const uint16_t FDIR_PARAMETERS_hw_oc_eh_power_off_count_threshold_rm3100;   //!< H/W over current detection event handler power off count threshold for RM3100
extern const uint32_t FDIR_PARAMETERS_hw_oc_eh_power_off_time_threshold_rm3100_ms; //!< H/W over current detection event handler power off time threshold for RM3100 [ms]
// nanoSSOC D60
extern const uint16_t FDIR_PARAMETERS_hw_oc_eh_reset_count_threshold_nanossoc_d60;       //!< H/W over current detection event handler reset count threshold for nanoSSOC D60
extern const uint32_t FDIR_PARAMETERS_hw_oc_eh_reset_time_threshold_nanossoc_d60_ms;     //!< H/W over current detection event handler reset time threshold for nanoSSOC D60 [ms]
extern const uint16_t FDIR_PARAMETERS_hw_oc_eh_power_off_count_threshold_nanossoc_d60;   //!< H/W over current detection event handler power off count threshold for nanoSSOC D60
extern const uint32_t FDIR_PARAMETERS_hw_oc_eh_power_off_time_threshold_nanossoc_d60_ms; //!< H/W over current detection event handler power off time threshold for nanoSSOC D60 [ms]
// MTQ
extern const uint16_t FDIR_PARAMETERS_hw_oc_eh_reset_count_threshold_mtq;            //!< H/W over current detection event handler reset count threshold for MTQ
extern const uint32_t FDIR_PARAMETERS_hw_oc_eh_reset_time_threshold_mtq_ms;          //!< H/W over current detection event handler reset time threshold for MTQ [ms]
extern const uint16_t FDIR_PARAMETERS_hw_oc_eh_power_off_count_threshold_mtq;            //!< H/W over current detection event handler power off count threshold for MTQ
extern const uint32_t FDIR_PARAMETERS_hw_oc_eh_power_off_time_threshold_mtq_ms;          //!< H/W over current detection event handler power off time threshold for MTQ [ms]
// RW
extern const uint16_t FDIR_PARAMETERS_hw_oc_eh_reset_count_threshold_rw0003_x;       //!< H/W over current detection event handler reset count threshold for RW0003 X
extern const uint32_t FDIR_PARAMETERS_hw_oc_eh_reset_time_threshold_rw0003_x_ms;     //!< H/W over current detection event handler reset time threshold for RW0003 X [ms]
extern const uint16_t FDIR_PARAMETERS_hw_oc_eh_power_off_count_threshold_rw0003_x;   //!< H/W over current detection event handler power off count threshold for RW0003 X
extern const uint32_t FDIR_PARAMETERS_hw_oc_eh_power_off_time_threshold_rw0003_x_ms; //!< H/W over current detection event handler power off time threshold for RW0003 X [ms]
extern const uint16_t FDIR_PARAMETERS_hw_oc_eh_reset_count_threshold_rw0003_y;       //!< H/W over current detection event handler reset count threshold for RW0003 Y
extern const uint32_t FDIR_PARAMETERS_hw_oc_eh_reset_time_threshold_rw0003_y_ms;     //!< H/W over current detection event handler reset time threshold for RW0003 Y [ms]
extern const uint16_t FDIR_PARAMETERS_hw_oc_eh_power_off_count_threshold_rw0003_y;   //!< H/W over current detection event handler power off count threshold for RW0003 Y
extern const uint32_t FDIR_PARAMETERS_hw_oc_eh_power_off_time_threshold_rw0003_y_ms; //!< H/W over current detection event handler power off time threshold for RW0003 Y [ms]
extern const uint16_t FDIR_PARAMETERS_hw_oc_eh_reset_count_threshold_rw0003_z;       //!< H/W over current detection event handler reset count threshold for RW0003 Z
extern const uint32_t FDIR_PARAMETERS_hw_oc_eh_reset_time_threshold_rw0003_z_ms;     //!< H/W over current detection event handler reset time threshold for RW0003 Z [ms]
extern const uint16_t FDIR_PARAMETERS_hw_oc_eh_power_off_count_threshold_rw0003_z;   //!< H/W over current detection event handler power off count threshold for RW0003 Z
extern const uint32_t FDIR_PARAMETERS_hw_oc_eh_power_off_time_threshold_rw0003_z_ms; //!< H/W over current detection event handler power off time threshold for RW0003 Z [ms]

// Temperature anomaly
// TBI

#endif // FDIR_PARAMETERS_H_
