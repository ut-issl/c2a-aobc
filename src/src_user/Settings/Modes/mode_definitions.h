/**
 * @file
 * @brief モード定義とモード遷移定義
 */
#ifndef MODE_DEFINITIONS_H_
#define MODE_DEFINITIONS_H_

typedef enum
{
  MD_MODEID_START_UP              = 0,  //!< 起動直後
  MD_MODEID_INITIAL               = 1,  //!< 起動後、及びモード切り替えの始点、地上試験用
  MD_MODEID_BDOT                  = 2,  //!< Bdot制御モード
  MD_MODEID_ROUGH_SUN_POINTING    = 3,  //!< 粗太陽捕捉制御モード
  MD_MODEID_ROUGH_THREE_AXIS      = 4,  //!< 粗三軸制御モード
  MD_MODEID_ROUGH_THREE_AXIS_RW   = 5,  //!< RWを使った粗三軸捕捉制御モード
  MD_MODEID_FINE_THREE_AXIS       = 6,  //!< 精三軸制御モード
  MD_MODEID_MODE_MAX                    // NOTE: tlmサイズ調整のため，最大モード数を8とする．増やすときはそちらとも調整する．
} MD_MODEID;

void MD_load_mode_list(void);
void MD_load_transition_table(void);

#endif
