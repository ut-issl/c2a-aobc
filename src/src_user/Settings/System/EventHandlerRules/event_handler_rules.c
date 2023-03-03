#pragma section REPRO
/**
 * @file
 * @brief  EH の Rule 共通コード
 */
#include "event_handler_rules.h"

void EH_load_default_rules(void)
{
  EH_load_rule_tlmcmd();
  EH_load_rule_hw_oc();
  EH_load_rule_sw_oc();
}

#pragma section
