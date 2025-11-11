#pragma section REPRO
/**
 * @file
 * @brief  EH の Rule 共通コード
 */

#include "event_handler_rules.h"
#include <src_core/system/event_manager/event_handler.h>
#include "../../../tlm_cmd/block_command_definitions.h"
#include "../../../tlm_cmd/block_command_definitions.h"


void EH_load_default_rules(void)
{
  EH_RuleSettings settings;

  // single
  settings.event.group = EL_GROUP_TEST_EH;
  settings.event.local = 0;
  settings.event.err_level = EL_ERROR_LEVEL_LOW;
  settings.should_match_err_level = 1;
  settings.condition.type = EH_RESPONSE_CONDITION_SINGLE;
  settings.condition.count_threshold = 0;
  settings.condition.time_threshold_ms = 0;
  settings.deploy_bct_id = BC_ID_MAX;
  settings.is_active = 1;
  EH_register_rule(EH_RULE_TEST0, &settings);


}

#pragma section
