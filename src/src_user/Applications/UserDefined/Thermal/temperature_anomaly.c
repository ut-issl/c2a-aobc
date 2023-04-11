#pragma section REPRO
/**
 * @file
 * @brief 温度異常検知アプリ
 */
#include "temperature_anomaly.h"

#include <src_core/System/EventManager/event_logger.h>
#include <src_core/TlmCmd/common_cmd_packet_util.h>
#include "../../../Library/c2a_math.h"
#include "thermo_sensor.h"
#include "../../DriverInstances/di_mpu9250.h"
#include "../../DriverInstances/di_stim210.h"
#include "../../DriverInstances/di_sagitta.h"
#include "../../DriverInstances/di_rw0003.h"
#include "../../DriverInstances/di_oem7600.h"


static TemperatureAnomaly        temperature_anomaly_;
const  TemperatureAnomaly* const temperature_anomaly = &temperature_anomaly_;

static float APP_TEMPERATURE_ANOMALY_temperature_degC_[APP_TEMPERATURE_ANOMALY_IDX_MAX]; //!< 温度情報をまとめた変数
static const float APP_TEMPERATURE_ANOMALY_kMaxTemperatureDegC_ = 1000.0f; //!< 最大温度 [degC]

static void APP_TEMPERATURE_ANOMALY_init_(void);
static void APP_TEMPERATURE_ANOMALY_exec_(void);

/**
 * @brief 各種温度をまとめて最新値に更新する
 */
static void APP_TEMPERATURE_ANOMALY_update_temperature_(void);

AppInfo APP_TEMPERATURE_ANOMALY_create_app(void)
{
  return AI_create_app_info("Temperature Anomaly", APP_TEMPERATURE_ANOMALY_init_, APP_TEMPERATURE_ANOMALY_exec_);
}

static void APP_TEMPERATURE_ANOMALY_init_(void)
{
  for (uint8_t idx = 0; idx < APP_TEMPERATURE_ANOMALY_IDX_MAX; idx++)
  {
    APP_TEMPERATURE_ANOMALY_temperature_degC_[idx] = 0.0f;
    temperature_anomaly_.threshold_list[idx].upper_degC = 100.0f;
    temperature_anomaly_.threshold_list[idx].lower_degC = -50.0f;
  }
  // TODO_L: 閾値初期値を個別に設定したい場合は個別設定する

  return;
}

static void APP_TEMPERATURE_ANOMALY_exec_(void)
{
  APP_TEMPERATURE_ANOMALY_update_temperature_();

  for (uint8_t idx = 0; idx < APP_TEMPERATURE_ANOMALY_IDX_MAX; idx++)
  {
    C2A_MATH_ERROR ret;
    ret = C2A_MATH_check_range_violation(APP_TEMPERATURE_ANOMALY_temperature_degC_[idx],
                                         temperature_anomaly_.threshold_list[idx].upper_degC,
                                         temperature_anomaly_.threshold_list[idx].lower_degC);
    if (ret == C2A_MATH_ERROR_RANGE_OVER_HIGH)
    {
      EL_record_event(EL_GROUP_TEMPERATURE_HIGH, idx, EL_ERROR_LEVEL_HIGH, 0);
    }
    else if (ret == C2A_MATH_ERROR_RANGE_OVER_LOW)
    {
      EL_record_event(EL_GROUP_TEMPERATURE_LOW, idx, EL_ERROR_LEVEL_HIGH, 0);
    }
    else
    {
      // 何もしない
    }
  }

  return;
}

static void APP_TEMPERATURE_ANOMALY_update_temperature_(void)
{
  APP_TEMPERATURE_ANOMALY_temperature_degC_[APP_TEMPERATURE_ANOMALY_IDX_PIC] =
    thermo_sensor->temperature_degC[APP_TSNS_IDX_PIC];
  APP_TEMPERATURE_ANOMALY_temperature_degC_[APP_TEMPERATURE_ANOMALY_IDX_RW_DCDC] =
    thermo_sensor->temperature_degC[APP_TSNS_IDX_RW_DCDC];

  APP_TEMPERATURE_ANOMALY_temperature_degC_[APP_TEMPERATURE_ANOMALY_IDX_ROUGH_GYRO] =
    mpu9250_driver[MPU9250_IDX_ON_AOBC]->info.temperature_degC;
  APP_TEMPERATURE_ANOMALY_temperature_degC_[APP_TEMPERATURE_ANOMALY_IDX_FINE_GYRO_X] =
    stim210_driver[STIM210_IDX_IN_UNIT]->info.temperature_compo_degC[0];
  APP_TEMPERATURE_ANOMALY_temperature_degC_[APP_TEMPERATURE_ANOMALY_IDX_FINE_GYRO_Y] =
    stim210_driver[STIM210_IDX_IN_UNIT]->info.temperature_compo_degC[1];
  APP_TEMPERATURE_ANOMALY_temperature_degC_[APP_TEMPERATURE_ANOMALY_IDX_FINE_GYRO_Z] =
    stim210_driver[STIM210_IDX_IN_UNIT]->info.temperature_compo_degC[2];
  APP_TEMPERATURE_ANOMALY_temperature_degC_[APP_TEMPERATURE_ANOMALY_IDX_STT_MCU] =
    sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.temperature_mcu_degC;
  APP_TEMPERATURE_ANOMALY_temperature_degC_[APP_TEMPERATURE_ANOMALY_IDX_STT_FPGA] =
    sagitta_driver[SAGITTA_IDX_IN_UNIT]->info.temperature_fpga_degC;
  APP_TEMPERATURE_ANOMALY_temperature_degC_[APP_TEMPERATURE_ANOMALY_IDX_RW_X] =
    rw0003_driver[RW0003_IDX_ON_X]->info.temperature_degC;
  APP_TEMPERATURE_ANOMALY_temperature_degC_[APP_TEMPERATURE_ANOMALY_IDX_RW_Y] =
    rw0003_driver[RW0003_IDX_ON_Y]->info.temperature_degC;
  APP_TEMPERATURE_ANOMALY_temperature_degC_[APP_TEMPERATURE_ANOMALY_IDX_RW_Z] =
    rw0003_driver[RW0003_IDX_ON_Z]->info.temperature_degC;
  APP_TEMPERATURE_ANOMALY_temperature_degC_[APP_TEMPERATURE_ANOMALY_IDX_GPSR] =
    oem7600_driver[OEM7600_IDX_IN_UNIT]->info.hardware_status.temperature_degC;

  return;
}


CCP_CmdRet Cmd_APP_TEMPERATURE_ANOMALY_SET_THRESHOLD(const CommonCmdPacket* packet)
{
  uint8_t arg_position = 0;

  APP_TEMPERATURE_ANOMALY_IDX idx = (APP_TEMPERATURE_ANOMALY_IDX)CCP_get_param_from_packet(packet, arg_position, uint8_t);
  arg_position++;
  if (idx >= APP_TEMPERATURE_ANOMALY_IDX_MAX) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);

  APP_TEMPERATURE_ANOMALY_TemperatureThreshold threshold;
  threshold.upper_degC = CCP_get_param_from_packet(packet, arg_position, float);
  arg_position++;
  if (threshold.lower_degC >= APP_TEMPERATURE_ANOMALY_kMaxTemperatureDegC_) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);

  threshold.lower_degC = CCP_get_param_from_packet(packet, arg_position, float);
  arg_position++;
  if (threshold.lower_degC <= PHYSICAL_CONST_ABSOLUTE_ZERO_degC) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);

  if (threshold.upper_degC <= threshold.lower_degC) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_CONTEXT);

  temperature_anomaly_.threshold_list[idx] = threshold;

  return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);
}

#pragma section
