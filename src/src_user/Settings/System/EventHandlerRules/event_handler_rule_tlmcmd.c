#pragma section REPRO
/**
 * @file
 * @brief テレコマ通信エラー の EH Rule
 */
#include "event_handler_rules.h"
#include <src_core/System/EventManager/event_handler.h>
#include "../../../TlmCmd/block_command_definitions.h"
#include "../event_logger_group.h"
#include "../../../Applications/DriverInstances/di_mpu9250.h"
#include "../../../Applications/DriverInstances/di_rm3100.h"
#include "../../../Applications/DriverInstances/di_nanossoc_d60.h"
#include "../../../Applications/DriverInstances/di_stim210.h"
#include "../../../Applications/DriverInstances/di_sagitta.h"
#include "../../../Applications/DriverInstances/di_oem7600.h"
#include "../../../Applications/DriverInstances/di_rw0003.h"
#include "../../../Applications/DriverInstances/di_ina260.h"

void EH_load_rule_tlmcmd(void)
{
  EH_RuleSettings settings;

  // MPU9250 TLM ERROR
  settings.event.group = EL_GROUP_TLM_ERROR_MPU9250;
  settings.event.local = MPU9250_IDX_ON_AOBC;
  settings.event.err_level = EL_ERROR_LEVEL_HIGH;
  settings.should_match_err_level = 1;
  settings.condition.type = EH_RESPONSE_CONDITION_CONTINUOUS;
  settings.condition.count_threshold = 100;
  settings.condition.time_threshold_ms = 2000;
  settings.deploy_bct_id = BC_RESET_MPU9250;
  settings.is_active = 0;
  EH_register_rule(EH_RULE_TLM_ERROR_MPU9250, &settings);

  // RM3100 ON AOBC TLM ERROR
  settings.event.group = EL_GROUP_TLM_ERROR_RM3100;
  settings.event.local = RM3100_IDX_ON_AOBC;
  settings.event.err_level = EL_ERROR_LEVEL_HIGH;
  settings.should_match_err_level = 1;
  settings.condition.type = EH_RESPONSE_CONDITION_CONTINUOUS;
  settings.condition.count_threshold = 100;
  settings.condition.time_threshold_ms = 2000;
  settings.deploy_bct_id = BC_RESET_RM3100;
  settings.is_active = 0;
  EH_register_rule(EH_RULE_TLM_ERROR_RM3100_ON_AOBC, &settings);

  // RM3100 ON AOBC TLM ERROR 多段
  settings.event.group = (EL_GROUP)EL_CORE_GROUP_EH_MATCH_RULE;
  settings.event.local = EH_RULE_TLM_ERROR_RM3100_ON_AOBC;
  settings.event.err_level = EL_ERROR_LEVEL_EH;
  settings.should_match_err_level = 1;
  settings.condition.type = EH_RESPONSE_CONDITION_CUMULATIVE;
  settings.condition.count_threshold = 10;
  settings.condition.time_threshold_ms = 0;
  settings.deploy_bct_id = BC_SELECT_RM3100_EXTERNAL;
  settings.is_active = 0;
  EH_register_rule(EH_RULE_TLM_ERROR_RM3100_ON_AOBC_BROKEN, &settings);

  // RM3100 EXTERNAL TLM ERROR
  settings.event.group = EL_GROUP_TLM_ERROR_RM3100;
  settings.event.local = RM3100_IDX_EXTERNAL;
  settings.event.err_level = EL_ERROR_LEVEL_HIGH;
  settings.should_match_err_level = 1;
  settings.condition.type = EH_RESPONSE_CONDITION_CONTINUOUS;
  settings.condition.count_threshold = 1000;
  settings.condition.time_threshold_ms = 2000;
  settings.deploy_bct_id = BC_RESET_RM3100;
  settings.is_active = 0;
  EH_register_rule(EH_RULE_TLM_ERROR_RM3100_EXTERNAL, &settings);

  // RM3100 EXTERNAL TLM ERROR 多段
  settings.event.group = (EL_GROUP)EL_CORE_GROUP_EH_MATCH_RULE;
  settings.event.local = EH_RULE_TLM_ERROR_RM3100_EXTERNAL;
  settings.event.err_level = EL_ERROR_LEVEL_EH;
  settings.should_match_err_level = 1;
  settings.condition.type = EH_RESPONSE_CONDITION_CUMULATIVE;
  settings.condition.count_threshold = 10;
  settings.condition.time_threshold_ms = 0;
  settings.deploy_bct_id = BC_SELECT_RM3100_ON_AOBC;
  settings.is_active = 0;
  EH_register_rule(EH_RULE_TLM_ERROR_RM3100_EXTERNAL_BROKEN, &settings);

  // NANOSSOC D60 PY TLM ERROR
  settings.event.group = EL_GROUP_TLM_ERROR_NANOSSOC;
  settings.event.local = NANOSSOC_D60_IDX_ON_PY;
  settings.event.err_level = EL_ERROR_LEVEL_HIGH;
  settings.should_match_err_level = 1;
  settings.condition.type = EH_RESPONSE_CONDITION_CONTINUOUS;
  settings.condition.count_threshold = 250;
  settings.condition.time_threshold_ms = 2000;
  settings.deploy_bct_id = BC_RESET_NANOSSOC_D60;
  settings.is_active = 0;
  EH_register_rule(EH_RULE_TLM_ERROR_NANOSSOC_D60_PY, &settings);

  // NANOSSOC D60 MY TLM ERROR
  settings.event.group = EL_GROUP_TLM_ERROR_NANOSSOC;
  settings.event.local = NANOSSOC_D60_IDX_ON_MY;
  settings.event.err_level = EL_ERROR_LEVEL_HIGH;
  settings.should_match_err_level = 1;
  settings.condition.type = EH_RESPONSE_CONDITION_CONTINUOUS;
  settings.condition.count_threshold = 250;
  settings.condition.time_threshold_ms = 2000;
  settings.deploy_bct_id = BC_RESET_NANOSSOC_D60;
  settings.is_active = 0;
  EH_register_rule(EH_RULE_TLM_ERROR_NANOSSOC_D60_MY, &settings);

  // NANOSSOC D60 PZ TLM ERROR
  settings.event.group = EL_GROUP_TLM_ERROR_NANOSSOC;
  settings.event.local = NANOSSOC_D60_IDX_ON_PZ;
  settings.event.err_level = EL_ERROR_LEVEL_HIGH;
  settings.should_match_err_level = 1;
  settings.condition.type = EH_RESPONSE_CONDITION_CONTINUOUS;
  settings.condition.count_threshold = 250;
  settings.condition.time_threshold_ms = 2000;
  settings.deploy_bct_id = BC_RESET_NANOSSOC_D60;
  settings.is_active = 0;
  EH_register_rule(EH_RULE_TLM_ERROR_NANOSSOC_D60_PZ, &settings);

  // NANOSSOC D60 MZ TLM ERROR
  settings.event.group = EL_GROUP_TLM_ERROR_NANOSSOC;
  settings.event.local = NANOSSOC_D60_IDX_ON_MZ;
  settings.event.err_level = EL_ERROR_LEVEL_HIGH;
  settings.should_match_err_level = 1;
  settings.condition.type = EH_RESPONSE_CONDITION_CONTINUOUS;
  settings.condition.count_threshold = 250;
  settings.condition.time_threshold_ms = 2000;
  settings.deploy_bct_id = BC_RESET_NANOSSOC_D60;
  settings.is_active = 0;
  EH_register_rule(EH_RULE_TLM_ERROR_NANOSSOC_D60_MZ, &settings);

  // STIM210 TLM ERROR
  settings.event.group = EL_GROUP_TLM_ERROR_STIM210;
  settings.event.local = STIM210_IDX_IN_UNIT;
  settings.event.err_level = EL_ERROR_LEVEL_HIGH;
  settings.should_match_err_level = 1;
  settings.condition.type = EH_RESPONSE_CONDITION_CONTINUOUS;
  settings.condition.count_threshold = 100;
  settings.condition.time_threshold_ms = 2000;
  settings.deploy_bct_id = BC_RESET_STIM210;
  settings.is_active = 0;
  EH_register_rule(EH_RULE_TLM_ERROR_STIM210, &settings);

  // STIM210 TLM ERROR 多段
  settings.event.group = (EL_GROUP)EL_CORE_GROUP_EH_MATCH_RULE;
  settings.event.local = EH_RULE_TLM_ERROR_STIM210;
  settings.event.err_level = EL_ERROR_LEVEL_EH;
  settings.should_match_err_level = 1;
  settings.condition.type = EH_RESPONSE_CONDITION_CUMULATIVE;
  settings.condition.count_threshold = 5;
  settings.condition.time_threshold_ms = 0;
  settings.deploy_bct_id = BC_POWER_OFF_STIM210;
  settings.is_active = 0;
  EH_register_rule(EH_RULE_TLM_ERROR_STIM210_BROKEN, &settings);

  // SAGITTA TLM ERROR
  settings.event.group = EL_GROUP_TLM_ERROR_SAGITTA;
  settings.event.local = SAGITTA_IDX_IN_UNIT;
  settings.event.err_level = EL_ERROR_LEVEL_HIGH;
  settings.should_match_err_level = 1;
  settings.condition.type = EH_RESPONSE_CONDITION_CONTINUOUS;
  settings.condition.count_threshold = 100;
  settings.condition.time_threshold_ms = 2000;
  settings.deploy_bct_id = BC_RESET_SAGITTA;
  settings.is_active = 0;
  EH_register_rule(EH_RULE_TLM_ERROR_SAGITTA, &settings);

  // OEM7600 TLM ERROR
  settings.event.group = EL_GROUP_TLM_ERROR_OEM7600;
  settings.event.local = OEM7600_IDX_IN_UNIT;
  settings.event.err_level = EL_ERROR_LEVEL_HIGH;
  settings.should_match_err_level = 1;
  settings.condition.type = EH_RESPONSE_CONDITION_CONTINUOUS;
  settings.condition.count_threshold = 100;
  settings.condition.time_threshold_ms = 2000;
  settings.deploy_bct_id = BC_RESET_OEM7600;
  settings.is_active = 0;
  EH_register_rule(EH_RULE_TLM_ERROR_OEM7600, &settings);

  // RWX TLM ERROR
  settings.event.group = EL_GROUP_TLM_ERROR_RW0003X;
  settings.event.local = RW0003_IDX_ON_X;
  settings.event.err_level = EL_ERROR_LEVEL_HIGH;
  settings.should_match_err_level = 1;
  settings.condition.type = EH_RESPONSE_CONDITION_CONTINUOUS;
  settings.condition.count_threshold = 10;
  settings.condition.time_threshold_ms = 2000;
  settings.deploy_bct_id = BC_RESET_RWX;
  settings.is_active = 0;
  EH_register_rule(EH_RULE_TLM_ERROR_RWX, &settings);

  // RWY TLM ERROR
  settings.event.group = EL_GROUP_TLM_ERROR_RW0003Y;
  settings.event.local = RW0003_IDX_ON_Y;
  settings.event.err_level = EL_ERROR_LEVEL_HIGH;
  settings.should_match_err_level = 1;
  settings.condition.type = EH_RESPONSE_CONDITION_CONTINUOUS;
  settings.condition.count_threshold = 10;
  settings.condition.time_threshold_ms = 2000;
  settings.deploy_bct_id = BC_RESET_RWY;
  settings.is_active = 0;
  EH_register_rule(EH_RULE_TLM_ERROR_RWY, &settings);

  // RWZ TLM ERROR
  settings.event.group = EL_GROUP_TLM_ERROR_RW0003Z;
  settings.event.local = RW0003_IDX_ON_Z;
  settings.event.err_level = EL_ERROR_LEVEL_HIGH;
  settings.should_match_err_level = 1;
  settings.condition.type = EH_RESPONSE_CONDITION_CONTINUOUS;
  settings.condition.count_threshold = 10;
  settings.condition.time_threshold_ms = 2000;
  settings.deploy_bct_id = BC_RESET_RWZ;
  settings.is_active = 0;
  EH_register_rule(EH_RULE_TLM_ERROR_RWZ, &settings);

  // INA260 TLM ERROR
  settings.event.group = EL_GROUP_TLM_ERROR_INA260;
  settings.event.local = INA260_IDX_PIC;
  settings.event.err_level = EL_ERROR_LEVEL_HIGH;
  settings.should_match_err_level = 1;
  settings.condition.type = EH_RESPONSE_CONDITION_CONTINUOUS;
  settings.condition.count_threshold = 100;
  settings.condition.time_threshold_ms = 5000;
  settings.deploy_bct_id = BC_RESET_INA260;
  settings.is_active = 0;
  EH_register_rule(EH_RULE_TLM_ERROR_INA260_PIC, &settings);

  settings.event.group = EL_GROUP_TLM_ERROR_INA260;
  settings.event.local = INA260_IDX_STIM210;
  settings.event.err_level = EL_ERROR_LEVEL_HIGH;
  settings.should_match_err_level = 1;
  settings.condition.type = EH_RESPONSE_CONDITION_CONTINUOUS;
  settings.condition.count_threshold = 100;
  settings.condition.time_threshold_ms = 5000;
  settings.deploy_bct_id = BC_RESET_INA260;
  settings.is_active = 0;
  EH_register_rule(EH_RULE_TLM_ERROR_INA260_STIM210, &settings);

  settings.event.group = EL_GROUP_TLM_ERROR_INA260;
  settings.event.local = INA260_IDX_SAGITTA;
  settings.event.err_level = EL_ERROR_LEVEL_HIGH;
  settings.should_match_err_level = 1;
  settings.condition.type = EH_RESPONSE_CONDITION_CONTINUOUS;
  settings.condition.count_threshold = 100;
  settings.condition.time_threshold_ms = 5000;
  settings.deploy_bct_id = BC_RESET_INA260;
  settings.is_active = 0;
  EH_register_rule(EH_RULE_TLM_ERROR_INA260_SAGITTA, &settings);

  settings.event.group = EL_GROUP_TLM_ERROR_INA260;
  settings.event.local = INA260_IDX_OEM7600;
  settings.event.err_level = EL_ERROR_LEVEL_HIGH;
  settings.should_match_err_level = 1;
  settings.condition.type = EH_RESPONSE_CONDITION_CONTINUOUS;
  settings.condition.count_threshold = 100;
  settings.condition.time_threshold_ms = 5000;
  settings.deploy_bct_id = BC_RESET_INA260;
  settings.is_active = 0;
  EH_register_rule(EH_RULE_TLM_ERROR_INA260_OEM7600, &settings);

  settings.event.group = EL_GROUP_TLM_ERROR_INA260;
  settings.event.local = INA260_IDX_RM3100;
  settings.event.err_level = EL_ERROR_LEVEL_HIGH;
  settings.should_match_err_level = 1;
  settings.condition.type = EH_RESPONSE_CONDITION_CONTINUOUS;
  settings.condition.count_threshold = 100;
  settings.condition.time_threshold_ms = 5000;
  settings.deploy_bct_id = BC_RESET_INA260;
  settings.is_active = 0;
  EH_register_rule(EH_RULE_TLM_ERROR_INA260_RM3100, &settings);

  settings.event.group = EL_GROUP_TLM_ERROR_INA260;
  settings.event.local = INA260_IDX_NANOSSOC_D60;
  settings.event.err_level = EL_ERROR_LEVEL_HIGH;
  settings.should_match_err_level = 1;
  settings.condition.type = EH_RESPONSE_CONDITION_CONTINUOUS;
  settings.condition.count_threshold = 100;
  settings.condition.time_threshold_ms = 5000;
  settings.deploy_bct_id = BC_RESET_INA260;
  settings.is_active = 0;
  EH_register_rule(EH_RULE_TLM_ERROR_INA260_NANOSSOC_D60, &settings);

  settings.event.group = EL_GROUP_TLM_ERROR_INA260;
  settings.event.local = INA260_IDX_MTQ;
  settings.event.err_level = EL_ERROR_LEVEL_HIGH;
  settings.should_match_err_level = 1;
  settings.condition.type = EH_RESPONSE_CONDITION_CONTINUOUS;
  settings.condition.count_threshold = 100;
  settings.condition.time_threshold_ms = 5000;
  settings.deploy_bct_id = BC_RESET_INA260;
  settings.is_active = 0;
  EH_register_rule(EH_RULE_TLM_ERROR_INA260_MTQ, &settings);

  settings.event.group = EL_GROUP_TLM_ERROR_INA260;
  settings.event.local = INA260_IDX_RW0003_X;
  settings.event.err_level = EL_ERROR_LEVEL_HIGH;
  settings.should_match_err_level = 1;
  settings.condition.type = EH_RESPONSE_CONDITION_CONTINUOUS;
  settings.condition.count_threshold = 100;
  settings.condition.time_threshold_ms = 5000;
  settings.deploy_bct_id = BC_RESET_INA260;
  settings.is_active = 0;
  EH_register_rule(EH_RULE_TLM_ERROR_INA260_RW0003_X, &settings);

  settings.event.group = EL_GROUP_TLM_ERROR_INA260;
  settings.event.local = INA260_IDX_RW0003_Y;
  settings.event.err_level = EL_ERROR_LEVEL_HIGH;
  settings.should_match_err_level = 1;
  settings.condition.type = EH_RESPONSE_CONDITION_CONTINUOUS;
  settings.condition.count_threshold = 100;
  settings.condition.time_threshold_ms = 5000;
  settings.deploy_bct_id = BC_RESET_INA260;
  settings.is_active = 0;
  EH_register_rule(EH_RULE_TLM_ERROR_INA260_RW0003_Y, &settings);

  settings.event.group = EL_GROUP_TLM_ERROR_INA260;
  settings.event.local = INA260_IDX_RW0003_Z;
  settings.event.err_level = EL_ERROR_LEVEL_HIGH;
  settings.should_match_err_level = 1;
  settings.condition.type = EH_RESPONSE_CONDITION_CONTINUOUS;
  settings.condition.count_threshold = 100;
  settings.condition.time_threshold_ms = 5000;
  settings.deploy_bct_id = BC_RESET_INA260;
  settings.is_active = 0;
  EH_register_rule(EH_RULE_TLM_ERROR_INA260_RW0003_Z, &settings);

  // NANOSSOC D60 PY CHECKSUM ERROR
  settings.event.group = EL_GROUP_ERROR_NANOSSOC;
  settings.event.local = NANOSSOC_D60_IDX_ON_PY;
  settings.event.err_level = EL_ERROR_LEVEL_HIGH;
  settings.should_match_err_level = 1;
  settings.condition.type = EH_RESPONSE_CONDITION_CONTINUOUS;
  settings.condition.count_threshold = 250;
  settings.condition.time_threshold_ms = 2000;
  settings.deploy_bct_id = BC_RESET_NANOSSOC_D60;
  settings.is_active = 0;
  EH_register_rule(EH_RULE_CHECKSUM_ERROR_NANOSSOC_D60_PY, &settings);

  // NANOSSOC D60 MY CHECKSUM ERROR
  settings.event.group = EL_GROUP_ERROR_NANOSSOC;
  settings.event.local = NANOSSOC_D60_IDX_ON_MY;
  settings.event.err_level = EL_ERROR_LEVEL_HIGH;
  settings.should_match_err_level = 1;
  settings.condition.type = EH_RESPONSE_CONDITION_CONTINUOUS;
  settings.condition.count_threshold = 250;
  settings.condition.time_threshold_ms = 2000;
  settings.deploy_bct_id = BC_RESET_NANOSSOC_D60;
  settings.is_active = 0;
  EH_register_rule(EH_RULE_CHECKSUM_ERROR_NANOSSOC_D60_MY, &settings);

  // NANOSSOC D60 PZ CHECKSUM ERROR
  settings.event.group = EL_GROUP_ERROR_NANOSSOC;
  settings.event.local = NANOSSOC_D60_IDX_ON_PZ;
  settings.event.err_level = EL_ERROR_LEVEL_HIGH;
  settings.should_match_err_level = 1;
  settings.condition.type = EH_RESPONSE_CONDITION_CONTINUOUS;
  settings.condition.count_threshold = 250;
  settings.condition.time_threshold_ms = 2000;
  settings.deploy_bct_id = BC_RESET_NANOSSOC_D60;
  settings.is_active = 0;
  EH_register_rule(EH_RULE_CHECKSUM_ERROR_NANOSSOC_D60_PZ, &settings);

  // NANOSSOC D60 MZ CHECKSUM ERROR
  settings.event.group = EL_GROUP_ERROR_NANOSSOC;
  settings.event.local = NANOSSOC_D60_IDX_ON_MZ;
  settings.event.err_level = EL_ERROR_LEVEL_HIGH;
  settings.should_match_err_level = 1;
  settings.condition.type = EH_RESPONSE_CONDITION_CONTINUOUS;
  settings.condition.count_threshold = 250;
  settings.condition.time_threshold_ms = 2000;
  settings.deploy_bct_id = BC_RESET_NANOSSOC_D60;
  settings.is_active = 0;
  EH_register_rule(EH_RULE_CHECKSUM_ERROR_NANOSSOC_D60_MZ, &settings);

  // STIM210 CRC ERROR
  settings.event.group = EL_GROUP_ERROR_STIM210;
  settings.event.local = STIM210_IDX_IN_UNIT;
  settings.event.err_level = EL_ERROR_LEVEL_HIGH;
  settings.should_match_err_level = 1;
  settings.condition.type = EH_RESPONSE_CONDITION_CONTINUOUS;
  settings.condition.count_threshold = 100;
  settings.condition.time_threshold_ms = 2000;
  settings.deploy_bct_id = BC_RESET_STIM210;
  settings.is_active = 0;
  EH_register_rule(EH_RULE_CRC_ERROR_STIM210, &settings);

  // STIM210 CRC ERROR 多段
  settings.event.group = (EL_GROUP)EL_CORE_GROUP_EH_MATCH_RULE;
  settings.event.local = EH_RULE_CRC_ERROR_STIM210;
  settings.event.err_level = EL_ERROR_LEVEL_EH;
  settings.should_match_err_level = 1;
  settings.condition.type = EH_RESPONSE_CONDITION_CUMULATIVE;
  settings.condition.count_threshold = 5;
  settings.condition.time_threshold_ms = 0;
  settings.deploy_bct_id = BC_POWER_OFF_STIM210;
  settings.is_active = 0;
  EH_register_rule(EH_RULE_CRC_ERROR_STIM210_BROKEN, &settings);

  // SAGITTA XXHASH ERROR
  settings.event.group = EL_GROUP_ERROR_SAGITTA;
  settings.event.local = SAGITTA_IDX_IN_UNIT;
  settings.event.err_level = EL_ERROR_LEVEL_HIGH;
  settings.should_match_err_level = 1;
  settings.condition.type = EH_RESPONSE_CONDITION_CONTINUOUS;
  settings.condition.count_threshold = 100;
  settings.condition.time_threshold_ms = 2000;
  settings.deploy_bct_id = BC_RESET_SAGITTA;
  settings.is_active = 0;
  EH_register_rule(EH_RULE_XXHASH_ERROR_SAGITTA, &settings);

  // OEM7600 CRC ERROR
  settings.event.group = EL_GROUP_ERROR_OEM7600;
  settings.event.local = OEM7600_IDX_IN_UNIT;
  settings.event.err_level = EL_ERROR_LEVEL_HIGH;
  settings.should_match_err_level = 1;
  settings.condition.type = EH_RESPONSE_CONDITION_CONTINUOUS;
  settings.condition.count_threshold = 100;
  settings.condition.time_threshold_ms = 2000;
  settings.deploy_bct_id = BC_RESET_OEM7600;
  settings.is_active = 0;
  EH_register_rule(EH_RULE_CRC_ERROR_OEM7600, &settings);

  // RWX CRC ERROR
  settings.event.group = EL_GROUP_ERROR_RW0003X;
  settings.event.local = RW0003_IDX_ON_X;
  settings.event.err_level = EL_ERROR_LEVEL_HIGH;
  settings.should_match_err_level = 1;
  settings.condition.type = EH_RESPONSE_CONDITION_CONTINUOUS;
  settings.condition.count_threshold = 100;
  settings.condition.time_threshold_ms = 2000;
  settings.deploy_bct_id = BC_RESET_RWX;
  settings.is_active = 0;
  EH_register_rule(EH_RULE_CRC_ERROR_RWX, &settings);

  // RWY CRC ERROR
  settings.event.group = EL_GROUP_ERROR_RW0003Y;
  settings.event.local = RW0003_IDX_ON_Y;
  settings.event.err_level = EL_ERROR_LEVEL_HIGH;
  settings.should_match_err_level = 1;
  settings.condition.type = EH_RESPONSE_CONDITION_CONTINUOUS;
  settings.condition.count_threshold = 100;
  settings.condition.time_threshold_ms = 2000;
  settings.deploy_bct_id = BC_RESET_RWY;
  settings.is_active = 0;
  EH_register_rule(EH_RULE_CRC_ERROR_RWY, &settings);

  // RWZ CRC ERROR
  settings.event.group = EL_GROUP_ERROR_RW0003Z;
  settings.event.local = RW0003_IDX_ON_Z;
  settings.event.err_level = EL_ERROR_LEVEL_HIGH;
  settings.should_match_err_level = 1;
  settings.condition.type = EH_RESPONSE_CONDITION_CONTINUOUS;
  settings.condition.count_threshold = 100;
  settings.condition.time_threshold_ms = 2000;
  settings.deploy_bct_id = BC_RESET_RWZ;
  settings.is_active = 0;
  EH_register_rule(EH_RULE_CRC_ERROR_RWZ, &settings);
}
