#pragma section REPRO
#include "app_registry.h"

#include <src_core/System/ApplicationManager/app_manager.h>

#include "app_headers.h"

static AM_ACK add_application_(size_t id,
                               AppInfo (*app_creator)(void));
void AR_load_initial_settings(void)
{
  add_application_(AR_NOP, NOP_create_app);
  // Driver Instances
  // Common
  // add_application_(AR_DI_UART_EXAMPLE, DI_uart_example);
  // add_application_(AR_DI_I2C_EXAMPLE,  I2C_EXAMPLE_update);
  add_application_(AR_DI_MOBC,         DI_MOBC_update);
  add_application_(AR_DI_MOBC_MST_PH,  DI_MOBC_ms_tlm_packet_handler);
  // AOCS
  add_application_(AR_DI_MPU9250,      DI_MPU9250_update);
  add_application_(AR_DI_RM3100,       DI_RM3100_update);
  add_application_(AR_DI_NANOSSOC_D60, DI_NANOSSOC_D60_update);
  add_application_(AR_DI_STIM210,      DI_STIM210_update);
  add_application_(AR_DI_SAGITTA,      DI_SAGITTA_update);
  add_application_(AR_DI_OEM7600,      DI_OEM7600_update);
  add_application_(AR_DI_MTQ_SEIREN,   DI_MTQ_SEIREN_update);
  add_application_(AR_DI_RW0003,       DI_RW0003_update);
  add_application_(AR_APP_MODULE_TEST_BENCH, APP_MODULE_TEST_BENCH_create_app); // 登録はしておくが、消しても良い
  // Power
  add_application_(AR_DI_INA260, DI_INA260_update);
  // CDH
  add_application_(AR_DI_FM25V10, DI_FM25V10_update);

  // Core
  add_application_(AR_GSC_DISPATCHER, GSCD_create_app);
  add_application_(AR_RTC_DISPATCHER, RTCD_create_app);
  add_application_(AR_TLC_DISPATCHER_GS, TLCD_gs_create_app);
  add_application_(AR_TLC_DISPATCHER_BC, TLCD_bc_create_app);
  add_application_(AR_TLC_DISPATCHER_TLM, TLCD_tlm_create_app);
  add_application_(AR_EVENT_UTILITY, EVENT_UTIL_create_app);
  // add_application_(AR_DBG_FLUSH_SCREEN, flush_screen);
  // add_application_(AR_DBG_PRINT_TIMESTAMP, print_time_stamp);
  // add_application_(AR_DBG_PRINT_CMD_STATUS, print_cmd_status);

  // User Defined
  // AOCS
  add_application_(AR_APP_AOCS_MM_INITIAL, APP_AOCS_MM_initial_create_app);
  add_application_(AR_APP_AOCS_MM_BDOT, APP_AOCS_MM_bdot_create_app);
  add_application_(AR_APP_AOCS_MM_SUN_POINTING, APP_AOCS_MM_sun_pointing_create_app);
  add_application_(AR_APP_AOCS_MM_ROUGH_THREE_AXIS, APP_AOCS_MM_rough_three_axis_create_app);
  add_application_(AR_APP_AOCS_MM_FINE_THREE_AXIS, APP_AOCS_MM_fine_three_axis_create_app);
  add_application_(AR_APP_AOCS_MANAGER, APP_AOCS_MANAGER_create_app);
  // Control
  add_application_(AR_APP_BDOT, APP_BDOT_create_app);
  add_application_(AR_APP_SUN_POINTING, APP_SUN_POINTING_create_app);
  add_application_(AR_APP_SUN_VEC_PROPAGATOR, APP_SUN_VEC_PROPAGATOR_create_app);
  add_application_(AR_APP_THREE_AXIS_CONTROL_MTQ, APP_TAC_MTQ_create_app);
  add_application_(AR_APP_THREE_AXIS_CONTROL_RW, APP_TAC_RW_create_app);
  add_application_(AR_APP_UNLOADING, APP_UNLOADING_create_app);
  // Determination
  add_application_(AR_APP_ROUGH_THREE_AXIS_DETERMINATION, APP_RTAD_create_app);
  add_application_(AR_APP_FINE_THREE_AXIS_DETERMINATION, APP_FTAD_create_app);
  add_application_(AR_APP_STT_GYRO_EKF, APP_STT_GYRO_EKF_create_app);
  // Hardware Dependent
  add_application_(AR_APP_AOCS_MTQ_SEIREN_CONTROLLER, APP_MTQ_SEIREN_CONTROLLER_create_app);
  add_application_(AR_APP_AOCS_RW_CONTROLLER, APP_RW_CONTROLLER_create_app);
  add_application_(AR_APP_MAG_SELECTOR, APP_MAG_SELECTOR_create_app);
  add_application_(AR_APP_GYRO_SELECTOR, APP_GYRO_SELECTOR_create_app);
  add_application_(AR_APP_SUN_SENSOR_SELECTOR, APP_SS_SELECTOR_create_app);
  add_application_(AR_APP_STT_SELECTOR, APP_STT_SELECTOR_create_app);
  add_application_(AR_APP_GPSR_SELECTOR, APP_GPSR_SELECTOR_create_app);
  add_application_(AR_APP_MPU9250_FILTER, APP_MPU9250_FILTER_create_app);
  add_application_(AR_APP_RM3100_FILTER, APP_RM3100_FILTER_create_app);
  add_application_(AR_APP_STIM210_FILTER, APP_STIM210_FILTER_create_app);
  add_application_(AR_APP_OEM7600_FILTER, APP_OEM7600_FILTER_create_app);
  add_application_(AR_APP_RW0003_FILTER, APP_RW0003_FILTER_create_app);
  add_application_(AR_APP_SAGITTA_FILTER, APP_SAGITTA_FILTER_create_app);
  add_application_(AR_APP_NANOSSOC_D60_FILTER, APP_NANOSSOC_D60_FILTER_create_app);
  add_application_(AR_APP_CURRENT_ANOMALY, APP_CURRENT_ANOMALY_create_app);
  // Orbit
  add_application_(AR_APP_ORBIT_CALCULATOR, APP_ORBIT_CALC_create_app);
  add_application_(AR_APP_KEPLER_ORBIT_PROPAGATOR, APP_KOP_create_app);
  add_application_(AR_APP_SGP4_ORBIT_PROPAGATOR, APP_SGP4_create_app);
  add_application_(AR_APP_GPSR_ORBIT_PROPAGATOR, APP_GPSROP_create_app);
  // Environment
  add_application_(AR_APP_TIME_SPACE_CALCULATOR, APP_TIME_SPACE_CALC_create_app);
  add_application_(AR_APP_SUN_DIR_ECI_CALCULATOR, APP_SUN_DIR_ECI_CALC_create_app);
  add_application_(AR_APP_GEOMAG_ECI_CALCULATOR, APP_GEOMAG_ECI_CALC_create_app);
  // Target
  add_application_(AR_APP_QUATERNION_INTERPOLATOR, APP_QI_create_app);
  add_application_(AR_APP_TARGET_ATTITUDE_CALCULATOR, APP_TARGET_ATT_CALC_create_app);
  add_application_(AR_APP_TARGET_ATTITUDE_FROM_ORBIT, APP_TAFO_create_app);

  // Power
  add_application_(AR_APP_POWER_SWITCH_CONTROL, APP_PSC_create_app);

  // CDH
  add_application_(AR_APP_NVM_MANAGER, APP_NVM_MANAGER_create_app);
  add_application_(AR_APP_NVM_PARTITION, APP_NVM_PARTITION_create_app);
  add_application_(AR_APP_NVM_PARAMETER, APP_NVM_PARAMETER_create_app);
  add_application_(AR_APP_AOCS_DATA_RECORDER, APP_AOCS_DR_create_app);

  // Thermal
  add_application_(AR_APP_THERMO_SENSOR, APP_TSNS_create_app);
  add_application_(AR_APP_TEMPERATURE_ANOMALY, APP_TEMPERATURE_ANOMALY_create_app);
}

static AM_ACK add_application_(size_t id,
                               AppInfo (*app_creator)(void))
{
  AppInfo ai = app_creator();
  return AM_register_ai(id, &ai);
}
#pragma section
