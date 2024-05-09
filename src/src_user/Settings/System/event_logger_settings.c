#pragma section REPRO
/**
 * @file
 * @brief event_logger の設定
 */

#include <src_core/System/EventManager/event_logger.h>
#include "event_logger_settings.h"
#include "event_logger_group.h"

void EL_load_default_settings(void)
{
  EL_disable_logging(EL_GROUP_TLM_ERROR_INA260);
  EL_disable_logging(EL_GROUP_TLM_ERROR_MPU9250);
  EL_disable_logging(EL_GROUP_TLM_ERROR_RM3100);
  EL_disable_logging(EL_GROUP_TLM_ERROR_NANOSSOC);
  EL_disable_logging(EL_GROUP_TLM_ERROR_STIM210);
  EL_disable_logging(EL_GROUP_TLM_ERROR_SAGITTA);
  EL_disable_logging(EL_GROUP_TLM_ERROR_OEM7600);
  EL_disable_logging(EL_GROUP_TLM_ERROR_RW0003X);
  EL_disable_logging(EL_GROUP_TLM_ERROR_RW0003Y);
  EL_disable_logging(EL_GROUP_TLM_ERROR_RW0003Z);
  EL_disable_logging(EL_GROUP_CHECKSUM_ERROR_NANOSSOC);
  EL_disable_logging(EL_GROUP_CRC_ERROR_STIM210);
  EL_disable_logging(EL_GROUP_XXHASH_ERROR_SAGITTA);
  EL_disable_logging(EL_GROUP_CRC_ERROR_OEM7600);
  EL_disable_logging(EL_GROUP_CRC_ERROR_RW0003X);
  EL_disable_logging(EL_GROUP_CRC_ERROR_RW0003Y);
  EL_disable_logging(EL_GROUP_CRC_ERROR_RW0003Z);
  EL_disable_logging(EL_GROUP_ERROR_MPU9250);
  EL_disable_logging(EL_GROUP_ERROR_RM3100);
  EL_disable_logging(EL_GROUP_ERROR_NANOSSOC);
  EL_disable_logging(EL_GROUP_ERROR_STIM210);
  EL_disable_logging(EL_GROUP_ERROR_SAGITTA);
  EL_disable_logging(EL_GROUP_ERROR_OEM7600);
  EL_disable_logging(EL_GROUP_ERROR_RW0003X);
  EL_disable_logging(EL_GROUP_ERROR_RW0003Y);
  EL_disable_logging(EL_GROUP_ERROR_RW0003Z);
  EL_disable_logging(EL_GROUP_SW_OC);
  EL_disable_logging(EL_GROUP_HW_OC);
}

#pragma section
