#ifndef APP_HEADERS_H_
#define APP_HEADERS_H_

// nop
#include <src_core/Applications/nop.h>

// Core
#include <src_core/Applications/gs_command_dispatcher.h>
#include <src_core/Applications/realtime_command_dispatcher.h>
#include <src_core/Applications/timeline_command_dispatcher.h>
#include <src_core/Applications/event_utility.h>
#include <src_core/Applications/memory_dump.h>
#include <src_core/Applications/divided_cmd_utility.h>

// MiddleWare

// DI
// Common
#include "DriverInstances/uart_example.h"
#include "DriverInstances/DI_i2c_example.h"
#include "DriverInstances/di_mobc.h"
// AOCS
#include "DriverInstances/di_mpu9250.h"
#include "DriverInstances/di_rm3100.h"
#include "DriverInstances/di_nanossoc_d60.h"
#include "DriverInstances/di_stim210.h"
#include "DriverInstances/di_sagitta.h"
#include "DriverInstances/di_oem7600.h"
#include "DriverInstances/di_mtq_seiren.h"
#include "DriverInstances/di_rw0003.h"
#include "UserDefined/module_test_bench.h"  // 登録はしておくが、消しても良い
// Power
#include "DriverInstances/di_ina260.h"
// CDH
#include "DriverInstances/di_fm25v10.h"

// UserDefined
// AOCS
#include "UserDefined/AOCS/aocs_mode_manager.h"
#include "UserDefined/AOCS/aocs_manager.h"
#include "UserDefined/AOCS/AttitudeControl/bdot.h"
#include "UserDefined/AOCS/AttitudeControl/sun_pointing.h"
#include "UserDefined/AOCS/AttitudeControl/three_axis_control_mtq.h"
#include "UserDefined/AOCS/AttitudeControl/three_axis_control_rw.h"
#include "UserDefined/AOCS/AttitudeControl/unloading.h"
#include "UserDefined/AOCS/AttitudeDetermination/sun_vector_propagator.h"
#include "UserDefined/AOCS/AttitudeDetermination/rough_three_axis_determination.h"
#include "UserDefined/AOCS/AttitudeDetermination/fine_three_axis_determination.h"
#include "UserDefined/AOCS/AttitudeDetermination/stt_gyro_ekf.h"
#include "UserDefined/AOCS/OrbitEstimation/orbit_calculator.h"
#include "UserDefined/AOCS/OrbitEstimation/kepler_orbit_propagator.h"
#include "UserDefined/AOCS/OrbitEstimation/sgp4_orbit_propagator.h"
#include "UserDefined/AOCS/OrbitEstimation/gpsr_orbit_propagator.h"
#include "UserDefined/AOCS/InertialReference/time_space_calculator.h"
#include "UserDefined/AOCS/InertialReference/inertial_geomag_calculator.h"
#include "UserDefined/AOCS/InertialReference/inertial_sun_direction_calculator.h"
#include "UserDefined/AOCS/TargetAttitude/quaternion_interpolator.h"
#include "UserDefined/AOCS/TargetAttitude/target_attitude_calculator.h"
#include "UserDefined/AOCS/TargetAttitude/target_attitude_from_orbit.h"

// HardwareDependent: 搭載するコンポに強く依存するアプリ
#include "UserDefined/AOCS/HardwareDependent/ActuatorControllers/mtq_seiren_controller.h"
#include "UserDefined/AOCS/HardwareDependent/ActuatorControllers/rw_controller.h"
#include "UserDefined/AOCS/HardwareDependent/SensorSelectors/magnetometer_selector.h"
#include "UserDefined/AOCS/HardwareDependent/SensorSelectors/gyro_selector.h"
#include "UserDefined/AOCS/HardwareDependent/SensorSelectors/sun_sensor_selector.h"
#include "UserDefined/AOCS/HardwareDependent/SensorSelectors/stt_selector.h"
#include "UserDefined/AOCS/HardwareDependent/SensorSelectors/gpsr_selector.h"
#include "UserDefined/AOCS/HardwareDependent/SensorFilters/mpu9250_filter.h"
#include "UserDefined/AOCS/HardwareDependent/SensorFilters/rm3100_filter.h"
#include "UserDefined/AOCS/HardwareDependent/SensorFilters/stim210_filter.h"
#include "UserDefined/AOCS/HardwareDependent/SensorFilters/oem7600_filter.h"
#include "UserDefined/AOCS/HardwareDependent/SensorFilters/rw0003_filter.h"
#include "UserDefined/AOCS/HardwareDependent/SensorFilters/sagitta_filter.h"
#include "UserDefined/AOCS/HardwareDependent/SensorFilters/nanossoc_d60_filter.h"
#include "UserDefined/AOCS/HardwareDependent/SensorMonitors/current_anomaly.h"

// Power
#include "UserDefined/Power/power_switch_control.h"

// CDH
#include "UserDefined/Cdh/non_volatile_memory_manager.h"
#include "UserDefined/Cdh/non_volatile_memory_partition.h"
#include "UserDefined/Cdh/non_volatile_memory_parameter.h"
#include "UserDefined/Cdh/aocs_data_recorder.h"

// Thermal
#include "UserDefined/Thermal/thermo_sensor.h"
#include "UserDefined/Thermal/temperature_anomaly.h"

#endif // APP_HEADERS_H_
