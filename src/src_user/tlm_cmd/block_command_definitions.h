/**
 * @file
 * @brief  ブロックコマンド定義
 * @note   このコードは自動生成されています！
 */
#ifndef BLOCK_COMMAND_DEFINITIONS_H_
#define BLOCK_COMMAND_DEFINITIONS_H_

// 登録されるBlockCommandTableのblock番号を規定
typedef enum
{
  BC_AR_DRIVERS_UPDATE_INI = 0,
  BC_AR_DEBUG_DISPLAY_INI = 1,
  BC_AR_GS_RELATED_PROCESS = 2,
  BC_AC_TLM_CMD_HIRATE = 3,
  BC_HK_CYCLIC_TLM = 4,
  BC_TL_START_UP = 5,    // STARTUPモード
  BC_TL_INITIAL = 6,    // INITIALモード
  BC_SL_START_UP_TO_INITIAL = 7,
  BC_SL_NOP = 8,

  BC_ID_MAX    // BCT 自体のサイズは BCT_MAX_BLOCKS で規定
} BC_DEFAULT_ID;

void BC_load_defaults(void);

#endif
