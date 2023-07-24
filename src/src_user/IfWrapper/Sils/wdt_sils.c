#pragma section REPRO
#include <src_core/IfWrapper/wdt.h>

// WDTの初期化
int WDT_initialize(void* my_wdt_v)
{
  return 0; // 100msの設定
}

// WDTのカウンタクリア
int WDT_clear(void* my_wdt_v)
{
  return 0;
}

// WDTの有効化
int WDT_enable(void* my_wdt_v)
{
  return 0;
}

// WDTの無効化
int WDT_disable(void* my_wdt_v)
{
  return 0;
}

// WDTの時間設定
int WDT_set_timer(void* my_wdt_v, int time)
{
  return 0;
}

#pragma section
