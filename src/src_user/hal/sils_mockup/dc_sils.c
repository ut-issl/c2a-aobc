#pragma section REPRO
#include <src_core/IfWrapper/DC.h>


int DC_init(void* my_dc_v)
{
  return 0;
}

int DC_RX(void* my_dc_v, void* data_v, int count)
{
  return 0;
}

int DC_TX(void* my_dc_v, void* data_v, int count)
{
  return 0;
}

int DC_all_init(void)
{
  return 0;
}

int DC_RX_all()
{
  return 0;
}


int DC_reopen_dummy(void* my_dc_v, int reason)
{
  return 0;
}


unsigned int dc_val_out; /**< @public @brief 最新のDC出力値データを格納する変数です(テレメトリ用)。*/
unsigned int dc_val_in;  /**< @public @brief 最新のDC入力値データを格納する変数です(テレメトリ用)。*/

#pragma section
