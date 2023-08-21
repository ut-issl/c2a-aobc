/**
 * @file
 * @brief  EH の Rule 共通ヘッダ
 */
#ifndef EVENT_HANDLER_RULES_H_
#define EVENT_HANDLER_RULES_H_

/**
 * @enum  EH_RULE_ID
 * @brief EH_Rule の ID
 * @note  最大数は EH_RULE_MAX で規定
 * @note  uint16_t を想定
 */
typedef enum
{
  EH_RULE_TLM_ERROR_MPU9250                = 0,
  EH_RULE_TLM_ERROR_RM3100_ON_AOBC         = 1,
  EH_RULE_TLM_ERROR_RM3100_ON_AOBC_BROKEN  = 2,
  EH_RULE_TLM_ERROR_RM3100_EXTERNAL        = 3,
  EH_RULE_TLM_ERROR_RM3100_EXTERNAL_BROKEN = 4,
  EH_RULE_TLM_ERROR_NANOSSOC_D60           = 5,
  EH_RULE_TLM_ERROR_STIM210                = 9,
  EH_RULE_TLM_ERROR_STIM210_BROKEN         = 10,
  EH_RULE_TLM_ERROR_SAGITTA                = 11,
  EH_RULE_TLM_ERROR_OEM7600                = 12,
  EH_RULE_TLM_ERROR_RWX                    = 13,
  EH_RULE_TLM_ERROR_RWY                    = 14,
  EH_RULE_TLM_ERROR_RWZ                    = 15,
  EH_RULE_TLM_ERROR_INA260_PIC             = 16,
  EH_RULE_TLM_ERROR_INA260_STIM210         = 17,
  EH_RULE_TLM_ERROR_INA260_SAGITTA         = 18,
  EH_RULE_TLM_ERROR_INA260_OEM7600         = 19,
  EH_RULE_TLM_ERROR_INA260_RM3100          = 20,
  EH_RULE_TLM_ERROR_INA260_NANOSSOC_D60    = 21,
  EH_RULE_TLM_ERROR_INA260_MTQ             = 22,
  EH_RULE_TLM_ERROR_INA260_RW0003_X        = 23,
  EH_RULE_TLM_ERROR_INA260_RW0003_Y        = 24,
  EH_RULE_TLM_ERROR_INA260_RW0003_Z        = 25,
  EH_RULE_CHECKSUM_ERROR_NANOSSOC_D60      = 26,
  EH_RULE_CRC_ERROR_STIM210                = 30,
  EH_RULE_CRC_ERROR_STIM210_BROKEN         = 31,
  EH_RULE_XXHASH_ERROR_SAGITTA             = 32,
  EH_RULE_CRC_ERROR_OEM7600                = 33,
  EH_RULE_CRC_ERROR_RWX                    = 34,
  EH_RULE_CRC_ERROR_RWY                    = 35,
  EH_RULE_CRC_ERROR_RWZ                    = 36,
  EH_RULE_HW_OC_STIM210                    = 37,
  EH_RULE_HW_OC_STIM210_BROKEN             = 38,
  EH_RULE_HW_OC_SAGITTA                    = 39,
  EH_RULE_HW_OC_SAGITTA_BROKEN             = 40,
  EH_RULE_HW_OC_OEM7600                    = 41,
  EH_RULE_HW_OC_OEM7600_BROKEN             = 42,
  EH_RULE_HW_OC_RM3100                     = 43,
  EH_RULE_HW_OC_RM3100_BROKEN              = 44,
  EH_RULE_HW_OC_NANOSSOC_D60               = 45,
  EH_RULE_HW_OC_NANOSSOC_D60_BROKEN        = 46,
  EH_RULE_HW_OC_MTQ                        = 47,
  EH_RULE_HW_OC_MTQ_BROKEN                 = 48,
  EH_RULE_HW_OC_RW0003X                    = 49,
  EH_RULE_HW_OC_RW0003X_BROKEN             = 50,
  EH_RULE_HW_OC_RW0003Y                    = 51,
  EH_RULE_HW_OC_RW0003Y_BROKEN             = 52,
  EH_RULE_HW_OC_RW0003Z                    = 53,
  EH_RULE_HW_OC_RW0003Z_BROKEN             = 54,
  EH_RULE_SW_OC_STIM210                    = 55,
  EH_RULE_SW_OC_STIM210_BROKEN             = 56,
  EH_RULE_SW_OC_SAGITTA                    = 57,
  EH_RULE_SW_OC_SAGITTA_BROKEN             = 58,
  EH_RULE_SW_OC_OEM7600                    = 59,
  EH_RULE_SW_OC_OEM7600_BROKEN             = 60,
  EH_RULE_SW_OC_RM3100                     = 61,
  EH_RULE_SW_OC_RM3100_BROKEN              = 62,
  EH_RULE_SW_OC_NANOSSOC_D60               = 63,
  EH_RULE_SW_OC_NANOSSOC_D60_BROKEN        = 64,
  EH_RULE_SW_OC_MTQ                        = 65,
  EH_RULE_SW_OC_MTQ_BROKEN                 = 66,
  EH_RULE_SW_OC_RW0003X                    = 67,
  EH_RULE_SW_OC_RW0003X_BROKEN             = 68,
  EH_RULE_SW_OC_RW0003Y                    = 69,
  EH_RULE_SW_OC_RW0003Y_BROKEN             = 70,
  EH_RULE_SW_OC_RW0003Z                    = 71,
  EH_RULE_SW_OC_RW0003Z_BROKEN             = 72
} EH_RULE_ID;


/**
 * @brief  event_handler のデフォルトルールを読み込む
 * @param  void
 * @return void
 */
void EH_load_default_rules(void);

// 以下，ユーザー定義 ルール登録関数宣言
void EH_load_rule_tlmcmd(void);
void EH_load_rule_hw_oc(void);
void EH_load_rule_sw_oc(void);

#endif
