/**
 * @file  ina260_parameters.h
 * @brief INA260に関する衛星固有パラメータを管理する
 */

#ifndef INA260_PARAMETERS_H_
#define INA260_PARAMETERS_H_

#include <src_user/Applications/DriverInstances/di_ina260.h>

// PIC
extern const INA260_AVERAGING_MODE  INA260_PARAMETERS_pic_averaging_mode;           //!< Averaging mode for PIC
extern const INA260_CONVERSION_TIME INA260_PARAMETERS_pic_voltage_conversion_time;  //!< Voltage conversion time for PIC
extern const INA260_CONVERSION_TIME INA260_PARAMETERS_pic_current_conversion_time;  //!< Current conversion time for PIC
extern const float INA260_PARAMETERS_pic_hw_over_current_threshold_mA;              //!< H/W over current threshold for PIC [mA]

// STIM210
extern const INA260_AVERAGING_MODE  INA260_PARAMETERS_stim210_averaging_mode;           //!< Averaging mode for STIM210
extern const INA260_CONVERSION_TIME INA260_PARAMETERS_stim210_voltage_conversion_time;  //!< Voltage conversion time for STIM210
extern const INA260_CONVERSION_TIME INA260_PARAMETERS_stim210_current_conversion_time;  //!< Current conversion time for STIM210
extern const float INA260_PARAMETERS_stim210_hw_over_current_threshold_mA;              //!< H/W over current threshold for STIM210 [mA]

// SAGITTA
extern const INA260_AVERAGING_MODE  INA260_PARAMETERS_sagitta_averaging_mode;           //!< Averaging mode for SAGITTA
extern const INA260_CONVERSION_TIME INA260_PARAMETERS_sagitta_voltage_conversion_time;  //!< Voltage conversion time for SAGITTA
extern const INA260_CONVERSION_TIME INA260_PARAMETERS_sagitta_current_conversion_time;  //!< Current conversion time for SAGITTA
extern const float INA260_PARAMETERS_sagitta_hw_over_current_threshold_mA;              //!< H/W over current threshold for SAGITTA [mA]

// OEM7600
extern const INA260_AVERAGING_MODE  INA260_PARAMETERS_oem7600_averaging_mode;           //!< Averaging mode for OEM7600
extern const INA260_CONVERSION_TIME INA260_PARAMETERS_oem7600_voltage_conversion_time;  //!< Voltage conversion time for OEM7600
extern const INA260_CONVERSION_TIME INA260_PARAMETERS_oem7600_current_conversion_time;  //!< Current conversion time for OEM7600
extern const float INA260_PARAMETERS_oem7600_hw_over_current_threshold_mA;              //!< H/W over current threshold for OEM7600 [mA]

// RM3100
extern const INA260_AVERAGING_MODE  INA260_PARAMETERS_rm3100_averaging_mode;           //!< Averaging mode for RM3100
extern const INA260_CONVERSION_TIME INA260_PARAMETERS_rm3100_voltage_conversion_time;  //!< Voltage conversion time for RM3100
extern const INA260_CONVERSION_TIME INA260_PARAMETERS_rm3100_current_conversion_time;  //!< Current conversion time for RM3100
extern const float INA260_PARAMETERS_rm3100_hw_over_current_threshold_mA;              //!< H/W over current threshold for RM3100 [mA]

// NanoSSOC-D60
extern const INA260_AVERAGING_MODE  INA260_PARAMETERS_nanossoc_d60_averaging_mode;           //!< Averaging mode for NanoSSOC-D60
extern const INA260_CONVERSION_TIME INA260_PARAMETERS_nanossoc_d60_voltage_conversion_time;  //!< Voltage conversion time for NanoSSOC-D60
extern const INA260_CONVERSION_TIME INA260_PARAMETERS_nanossoc_d60_current_conversion_time;  //!< Current conversion time for NanoSSOC-D60
extern const float INA260_PARAMETERS_nanossoc_d60_hw_over_current_threshold_mA;              //!< H/W over current threshold for NanoSSOC-D60 [mA]

// MTQ
extern const INA260_AVERAGING_MODE  INA260_PARAMETERS_mtq_averaging_mode;           //!< Averaging mode for MTQ
extern const INA260_CONVERSION_TIME INA260_PARAMETERS_mtq_voltage_conversion_time;  //!< Voltage conversion time for MTQ
extern const INA260_CONVERSION_TIME INA260_PARAMETERS_mtq_current_conversion_time;  //!< Current conversion time for MTQ
extern const float INA260_PARAMETERS_mtq_hw_over_current_threshold_mA;              //!< H/W over current threshold for MTQ [mA]

// RW0003 X
extern const INA260_AVERAGING_MODE  INA260_PARAMETERS_rw0003_x_averaging_mode;           //!< Averaging mode for RW0003 X
extern const INA260_CONVERSION_TIME INA260_PARAMETERS_rw0003_x_voltage_conversion_time;  //!< Voltage conversion time for RW0003 X
extern const INA260_CONVERSION_TIME INA260_PARAMETERS_rw0003_x_current_conversion_time;  //!< Current conversion time for RW0003 X
extern const float INA260_PARAMETERS_rw0003_x_hw_over_current_threshold_mA;              //!< H/W over current threshold for RW0003 X [mA]

// RW0003 Y
extern const INA260_AVERAGING_MODE  INA260_PARAMETERS_rw0003_y_averaging_mode;           //!< Averaging mode for RW0003 Y
extern const INA260_CONVERSION_TIME INA260_PARAMETERS_rw0003_y_voltage_conversion_time;  //!< Voltage conversion time for RW0003 Y
extern const INA260_CONVERSION_TIME INA260_PARAMETERS_rw0003_y_current_conversion_time;  //!< Current conversion time for RW0003 Y
extern const float INA260_PARAMETERS_rw0003_y_hw_over_current_threshold_mA;              //!< H/W over current threshold for RW0003 Y [mA]

// RW0003 Z
extern const INA260_AVERAGING_MODE  INA260_PARAMETERS_rw0003_z_averaging_mode;           //!< Averaging mode for RW0003 Z
extern const INA260_CONVERSION_TIME INA260_PARAMETERS_rw0003_z_voltage_conversion_time;  //!< Voltage conversion time for RW0003 Z
extern const INA260_CONVERSION_TIME INA260_PARAMETERS_rw0003_z_current_conversion_time;  //!< Current conversion time for RW0003 Z
extern const float INA260_PARAMETERS_rw0003_z_hw_over_current_threshold_mA;              //!< H/W over current threshold for RW0003 Z [mA]

#endif // INA260_PARAMETERS_H_
