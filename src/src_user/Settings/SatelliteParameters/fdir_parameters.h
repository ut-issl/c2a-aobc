/**
 * @file  structure_parameters.h
 * @brief 衛星構造に関する衛星固有パラメータを管理する
 */

#ifndef FDIR_PARAMETERS_H_
#define FDIR_PARAMETERS_H_

#include <src_user/Library/physical_constants.h>
#include <stdint.h>

// Mode Manager
extern const float FDIR_PARAMETERS_ang_vel_conv_limit_rad_s;    //!< Angular velocity convergence detection limit [rad/s]
extern const float FDIR_PARAMETERS_ang_vel_conv_time_limit_s;   //!< Angular velocity convergence detection time limit [s]

extern const float FDIR_PARAMETERS_ang_vel_norm_increase_limit_rad_s;   //!< Angular velocity increase anomaly detection limit [rad/s]
extern const float FDIR_PARAMETERS_ang_vel_anomaly_detection_period_s;  //!< Angular velocity anomaly detection period [s]

extern const float FDIR_PARAMETERS_sun_angle_div_limit_rad;     //!< Sun pointing divergence detection limit [rad]
extern const float FDIR_PARAMETERS_sun_angle_div_time_limit_s;  //!< Sun pointing divergence detection time limit [s]

extern const float FDIR_PARAMETERS_three_axis_div_limit_rad;    //!< Three axis control divergence detection limit [rad]
extern const float FDIR_PARAMETERS_three_axis_div_time_limit_s; //!< Three axis control divergence detection time limit [s]

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


// Temperature anomaly
// TBI

#endif // FDIR_PARAMETERS_H_
