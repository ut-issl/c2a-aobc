#pragma section REPRO
/**
 * @file
 * @brief H/W過電流の EH Rule
 */
#include "event_handler_rules.h"
#include <src_core/System/EventManager/event_handler.h>
#include "../../../TlmCmd/block_command_definitions.h"
#include "../event_logger_group.h"
#include "../../../Applications/DriverInstances/di_ina260.h"

void EH_load_rule_hw_oc(void)
{
  EH_RuleSettings settings;

  settings.event.group = EL_GROUP_HW_OC;
  settings.event.local = INA260_IDX_STIM210;
  settings.event.err_level = EL_ERROR_LEVEL_LOW;
  settings.should_match_err_level = 1;
  settings.condition.type = EH_RESPONSE_CONDITION_CONTINUOUS;
  settings.condition.count_threshold = 10;
  settings.condition.time_threshold_ms = 5000;
  settings.deploy_bct_id = BC_RESET_STIM210;
  settings.is_active = 0;
  EH_register_rule(EH_RULE_HW_OC_STIM210, &settings);

  // STIM210 多段
  settings.event.group = (EL_GROUP)EL_CORE_GROUP_EH_MATCH_RULE;
  settings.event.local = EH_RULE_HW_OC_STIM210;
  settings.event.err_level = EL_ERROR_LEVEL_EH;
  settings.should_match_err_level = 1;
  settings.condition.type = EH_RESPONSE_CONDITION_CUMULATIVE;
  settings.condition.count_threshold = 5;
  settings.condition.time_threshold_ms = 0;
  settings.deploy_bct_id = BC_POWER_OFF_STIM210;
  settings.is_active = 0;
  EH_register_rule(EH_RULE_HW_OC_STIM210_BROKEN, &settings);

  settings.event.group = EL_GROUP_HW_OC;
  settings.event.local = INA260_IDX_SAGITTA;
  settings.event.err_level = EL_ERROR_LEVEL_LOW;
  settings.should_match_err_level = 1;
  settings.condition.type = EH_RESPONSE_CONDITION_CONTINUOUS;
  settings.condition.count_threshold = 10;
  settings.condition.time_threshold_ms = 5000;
  settings.deploy_bct_id = BC_RESET_SAGITTA;
  settings.is_active = 0;
  EH_register_rule(EH_RULE_HW_OC_SAGITTA, &settings);

  // SAGITTA 多段
  settings.event.group = (EL_GROUP)EL_CORE_GROUP_EH_MATCH_RULE;
  settings.event.local = EH_RULE_HW_OC_SAGITTA;
  settings.event.err_level = EL_ERROR_LEVEL_EH;
  settings.should_match_err_level = 1;
  settings.condition.type = EH_RESPONSE_CONDITION_CUMULATIVE;
  settings.condition.count_threshold = 5;
  settings.condition.time_threshold_ms = 0;
  settings.deploy_bct_id = BC_POWER_OFF_SAGITTA;
  settings.is_active = 0;
  EH_register_rule(EH_RULE_HW_OC_SAGITTA_BROKEN, &settings);

  settings.event.group = EL_GROUP_HW_OC;
  settings.event.local = INA260_IDX_OEM7600;
  settings.event.err_level = EL_ERROR_LEVEL_LOW;
  settings.should_match_err_level = 1;
  settings.condition.type = EH_RESPONSE_CONDITION_CONTINUOUS;
  settings.condition.count_threshold = 10;
  settings.condition.time_threshold_ms = 5000;
  settings.deploy_bct_id = BC_RESET_OEM7600;
  settings.is_active = 0;
  EH_register_rule(EH_RULE_HW_OC_OEM7600, &settings);

  // OEM7600 多段
  settings.event.group = (EL_GROUP)EL_CORE_GROUP_EH_MATCH_RULE;
  settings.event.local = EH_RULE_HW_OC_OEM7600;
  settings.event.err_level = EL_ERROR_LEVEL_EH;
  settings.should_match_err_level = 1;
  settings.condition.type = EH_RESPONSE_CONDITION_CUMULATIVE;
  settings.condition.count_threshold = 5;
  settings.condition.time_threshold_ms = 0;
  settings.deploy_bct_id = BC_POWER_OFF_OEM7600;
  settings.is_active = 0;
  EH_register_rule(EH_RULE_HW_OC_OEM7600_BROKEN, &settings);

  settings.event.group = EL_GROUP_HW_OC;
  settings.event.local = INA260_IDX_RM3100;
  settings.event.err_level = EL_ERROR_LEVEL_LOW;
  settings.should_match_err_level = 1;
  settings.condition.type = EH_RESPONSE_CONDITION_CONTINUOUS;
  settings.condition.count_threshold = 10;
  settings.condition.time_threshold_ms = 5000;
  settings.deploy_bct_id = BC_RESET_RM3100;
  settings.is_active = 0;
  EH_register_rule(EH_RULE_HW_OC_RM3100, &settings);

  // RM3100 多段
  settings.event.group = (EL_GROUP)EL_CORE_GROUP_EH_MATCH_RULE;
  settings.event.local = EH_RULE_HW_OC_RM3100;
  settings.event.err_level = EL_ERROR_LEVEL_EH;
  settings.should_match_err_level = 1;
  settings.condition.type = EH_RESPONSE_CONDITION_CUMULATIVE;
  settings.condition.count_threshold = 5;
  settings.condition.time_threshold_ms = 0;
  settings.deploy_bct_id = BC_POWER_OFF_RM3100;
  settings.is_active = 0;
  EH_register_rule(EH_RULE_HW_OC_RM3100_BROKEN, &settings);

  settings.event.group = EL_GROUP_HW_OC;
  settings.event.local = INA260_IDX_NANOSSOC_D60;
  settings.event.err_level = EL_ERROR_LEVEL_LOW;
  settings.should_match_err_level = 1;
  settings.condition.type = EH_RESPONSE_CONDITION_CONTINUOUS;
  settings.condition.count_threshold = 10;
  settings.condition.time_threshold_ms = 5000;
  settings.deploy_bct_id = BC_RESET_NANOSSOC_D60;
  settings.is_active = 0;
  EH_register_rule(EH_RULE_HW_OC_NANOSSOC_D60, &settings);

  // NANOSSOC_D60 多段
  settings.event.group = (EL_GROUP)EL_CORE_GROUP_EH_MATCH_RULE;
  settings.event.local = EH_RULE_HW_OC_NANOSSOC_D60;
  settings.event.err_level = EL_ERROR_LEVEL_EH;
  settings.should_match_err_level = 1;
  settings.condition.type = EH_RESPONSE_CONDITION_CUMULATIVE;
  settings.condition.count_threshold = 5;
  settings.condition.time_threshold_ms = 0;
  settings.deploy_bct_id = BC_POWER_OFF_NANOSSOC_D60;
  settings.is_active = 0;
  EH_register_rule(EH_RULE_HW_OC_NANOSSOC_D60_BROKEN, &settings);

  settings.event.group = EL_GROUP_HW_OC;
  settings.event.local = INA260_IDX_MTQ;
  settings.event.err_level = EL_ERROR_LEVEL_LOW;
  settings.should_match_err_level = 1;
  settings.condition.type = EH_RESPONSE_CONDITION_CONTINUOUS;
  settings.condition.count_threshold = 10;
  settings.condition.time_threshold_ms = 5000;
  settings.deploy_bct_id = BC_RESET_MTQ;
  settings.is_active = 0;
  EH_register_rule(EH_RULE_HW_OC_MTQ, &settings);

  // MTQ 多段
  settings.event.group = (EL_GROUP)EL_CORE_GROUP_EH_MATCH_RULE;
  settings.event.local = EH_RULE_HW_OC_MTQ;
  settings.event.err_level = EL_ERROR_LEVEL_EH;
  settings.should_match_err_level = 1;
  settings.condition.type = EH_RESPONSE_CONDITION_CUMULATIVE;
  settings.condition.count_threshold = 5;
  settings.condition.time_threshold_ms = 0;
  settings.deploy_bct_id = BC_POWER_OFF_MTQ;
  settings.is_active = 0;
  EH_register_rule(EH_RULE_HW_OC_MTQ_BROKEN, &settings);

  settings.event.group = EL_GROUP_HW_OC;
  settings.event.local = INA260_IDX_RW0003_X;
  settings.event.err_level = EL_ERROR_LEVEL_LOW;
  settings.should_match_err_level = 1;
  settings.condition.type = EH_RESPONSE_CONDITION_CONTINUOUS;
  settings.condition.count_threshold = 10;
  settings.condition.time_threshold_ms = 5000;
  settings.deploy_bct_id = BC_RESET_RWX;
  settings.is_active = 0;
  EH_register_rule(EH_RULE_HW_OC_RW0003X, &settings);

  // RW0003X 多段
  settings.event.group = (EL_GROUP)EL_CORE_GROUP_EH_MATCH_RULE;
  settings.event.local = EH_RULE_HW_OC_RW0003X;
  settings.event.err_level = EL_ERROR_LEVEL_EH;
  settings.should_match_err_level = 1;
  settings.condition.type = EH_RESPONSE_CONDITION_CUMULATIVE;
  settings.condition.count_threshold = 5;
  settings.condition.time_threshold_ms = 0;
  settings.deploy_bct_id = BC_POWER_OFF_RWX;
  settings.is_active = 0;
  EH_register_rule(EH_RULE_HW_OC_RW0003X_BROKEN, &settings);

  settings.event.group = EL_GROUP_HW_OC;
  settings.event.local = INA260_IDX_RW0003_Y;
  settings.event.err_level = EL_ERROR_LEVEL_LOW;
  settings.should_match_err_level = 1;
  settings.condition.type = EH_RESPONSE_CONDITION_CONTINUOUS;
  settings.condition.count_threshold = 10;
  settings.condition.time_threshold_ms = 5000;
  settings.deploy_bct_id = BC_RESET_RWY;
  settings.is_active = 0;
  EH_register_rule(EH_RULE_HW_OC_RW0003Y, &settings);

  // RW0003Y 多段
  settings.event.group = (EL_GROUP)EL_CORE_GROUP_EH_MATCH_RULE;
  settings.event.local = EH_RULE_HW_OC_RW0003Y;
  settings.event.err_level = EL_ERROR_LEVEL_EH;
  settings.should_match_err_level = 1;
  settings.condition.type = EH_RESPONSE_CONDITION_CUMULATIVE;
  settings.condition.count_threshold = 5;
  settings.condition.time_threshold_ms = 0;
  settings.deploy_bct_id = BC_POWER_OFF_RWY;
  settings.is_active = 0;
  EH_register_rule(EH_RULE_HW_OC_RW0003Y_BROKEN, &settings);

  settings.event.group = EL_GROUP_HW_OC;
  settings.event.local = INA260_IDX_RW0003_Z;
  settings.event.err_level = EL_ERROR_LEVEL_LOW;
  settings.should_match_err_level = 1;
  settings.condition.type = EH_RESPONSE_CONDITION_CONTINUOUS;
  settings.condition.count_threshold = 10;
  settings.condition.time_threshold_ms = 5000;
  settings.deploy_bct_id = BC_RESET_RWZ;
  settings.is_active = 0;
  EH_register_rule(EH_RULE_HW_OC_RW0003Z, &settings);

  // RW0003Z 多段
  settings.event.group = (EL_GROUP)EL_CORE_GROUP_EH_MATCH_RULE;
  settings.event.local = EH_RULE_HW_OC_RW0003Z;
  settings.event.err_level = EL_ERROR_LEVEL_EH;
  settings.should_match_err_level = 1;
  settings.condition.type = EH_RESPONSE_CONDITION_CUMULATIVE;
  settings.condition.count_threshold = 5;
  settings.condition.time_threshold_ms = 0;
  settings.deploy_bct_id = BC_POWER_OFF_RWZ;
  settings.is_active = 0;
  EH_register_rule(EH_RULE_HW_OC_RW0003Z_BROKEN, &settings);
}
