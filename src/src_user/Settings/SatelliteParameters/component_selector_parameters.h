/**
 * @file  component_selector_parameters.h
 * @brief コンポセレクタに関する衛星固有パラメータを管理する
 */

#ifndef COMPONENT_SELECTOR_PARAMETERS_H_
#define COMPONENT_SELECTOR_PARAMETERS_H_

#include "../../Applications/UserDefined/AOCS/HardwareDependent/SensorSelectors/magnetometer_selector.h"
#include "../../Applications/UserDefined/AOCS/HardwareDependent/SensorSelectors/gyro_selector.h"

extern const APP_MAG_SELECTOR_STATE COMPONENT_SELECTOR_PARAMETERS_initial_selected_magnetometer;  //!< Initial selected magnetometer

#endif // COMPONENT_SELECTOR_PARAMETERS_H_
