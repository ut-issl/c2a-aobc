cmake_minimum_required(VERSION 3.13)

project(C2A_USER_APPS)

set(C2A_SRCS
  app_registry.c
  # UserDefined
  UserDefined/AppStatus.c
  # UserDefined/debug_apps.c
  UserDefined/module_test_bench.c
  UserDefined/AOCS/aocs_mode_manager.c
  UserDefined/AOCS/aocs_manager.c
  UserDefined/AOCS/AttitudeControl/bdot.c
  UserDefined/AOCS/AttitudeControl/sun_pointing.c
  UserDefined/AOCS/AttitudeControl/three_axis_control_mtq.c
  UserDefined/AOCS/AttitudeControl/three_axis_control_rw.c
  UserDefined/AOCS/AttitudeControl/unloading.c
  UserDefined/AOCS/AttitudeDetermination/sun_vector_propagator.c
  UserDefined/AOCS/AttitudeDetermination/rough_three_axis_determination.c
  UserDefined/AOCS/AttitudeDetermination/fine_three_axis_determination.c
  UserDefined/AOCS/AttitudeDetermination/stt_gyro_ekf.c
  UserDefined/AOCS/HardwareDependent/ActuatorControllers/mtq_seiren_controller.c
  UserDefined/AOCS/HardwareDependent/ActuatorControllers/rw_controller.c
  UserDefined/AOCS/HardwareDependent/SensorSelectors/magnetometer_selector.c
  UserDefined/AOCS/HardwareDependent/SensorSelectors/gyro_selector.c
  UserDefined/AOCS/HardwareDependent/SensorSelectors/sun_sensor_selector.c
  UserDefined/AOCS/HardwareDependent/SensorSelectors/stt_selector.c
  UserDefined/AOCS/HardwareDependent/SensorSelectors/gpsr_selector.c
  UserDefined/AOCS/HardwareDependent/SensorFilters/mpu9250_filter.c
  UserDefined/AOCS/HardwareDependent/SensorFilters/rm3100_filter.c
  UserDefined/AOCS/HardwareDependent/SensorFilters/stim210_filter.c
  UserDefined/AOCS/HardwareDependent/SensorFilters/oem7600_filter.c
  UserDefined/AOCS/HardwareDependent/SensorFilters/rw0003_filter.c
  UserDefined/AOCS/HardwareDependent/SensorFilters/sagitta_filter.c
  UserDefined/AOCS/HardwareDependent/SensorFilters/nanossoc_d60_filter.c
  UserDefined/AOCS/HardwareDependent/SensorMonitors/current_anomaly.c
  UserDefined/AOCS/OrbitEstimation/orbit_calculator.c
  UserDefined/AOCS/OrbitEstimation/kepler_orbit_propagator.c
  UserDefined/AOCS/OrbitEstimation/sgp4_orbit_propagator.c
  UserDefined/AOCS/OrbitEstimation/gpsr_orbit_propagator.c
  UserDefined/AOCS/InertialReference/time_space_calculator.c
  UserDefined/AOCS/InertialReference/inertial_geomag_calculator.c
  UserDefined/AOCS/InertialReference/inertial_sun_direction_calculator.c
  UserDefined/AOCS/TargetAttitude/quaternion_interpolator.c
  UserDefined/AOCS/TargetAttitude/target_attitude_calculator.c
  UserDefined/AOCS/TargetAttitude/target_attitude_from_orbit.c
  UserDefined/Cdh/non_volatile_memory_manager.c
  UserDefined/Cdh/non_volatile_memory_partition.c
  UserDefined/Cdh/non_volatile_memory_parameter.c
  UserDefined/Cdh/aocs_data_recorder.c
  UserDefined/Power/power_switch_control.c
  UserDefined/Thermal/thermo_sensor.c
  UserDefined/Thermal/temperature_anomaly.c
  # DriverInstances
  DriverInstances/uart_example.c
  DriverInstances/DI_i2c_example.c
  DriverInstances/di_mobc.c
  DriverInstances/di_ina260.c
  DriverInstances/di_mpu9250.c
  DriverInstances/di_mtq_seiren.c
  DriverInstances/di_nanossoc_d60.c
  DriverInstances/di_oem7600.c
  DriverInstances/di_rm3100.c
  DriverInstances/di_rw0003.c
  DriverInstances/di_sagitta.c
  DriverInstances/di_stim210.c
  DriverInstances/di_fm25v10.c
)

add_library(${PROJECT_NAME} OBJECT ${C2A_SRCS})

include(${C2A_USER_DIR}/common.cmake)
