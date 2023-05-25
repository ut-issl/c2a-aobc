/*
 Name:  c2a_aobc_example.ino
*/

extern "C" 
{
#include <src_core/c2a_core_main.h>
#include <src_core/System/TimeManager/time_manager.h>
#include <src_core/Library/print.h>
}

// C2A Private global variables and functions
static void C2A_init_(void);
static void C2A_main_(void);

// Timer interrupt functions
static void C2A_timer_setting_(void);
static void __USER_ISR C2A_timer1_handler_(void);

// Arduino functions
void setup() 
{
  // TODO: ここでいいのか場所は要検討
  Serial.begin(115200);   // Debugポート

  C2A_init_();

  return;
}

void loop() 
{
  // Printf("AOBC Main loop Started.\n");
  C2A_main_();

  return;
}

// C2A Main functions
static void C2A_main_(void)
{
  while (1)
  {
    C2A_core_main();
    // ユーザー定義loop処理はここに入れる
  }

  return;
}

// C2A関連の初期化
// HW関連部分（タイマー，割り込み設定など）のみ独自に初期化
// Printf内部で WDT_clear_wdt(); が呼ばれてることに注意！
static void C2A_init_(void)
{
  TMGR_init();                // Time Manager
                              // AM_initialize_all_apps での時間計測のためにここで初期化
  Printf("C2A_init: TMGR_init done.\n");

  // タイマ割込み関連
  C2A_timer_setting_();
  Printf("C2A_init: timer_setting_ done.\n");

  C2A_core_init();

  TMGR_clear(); // TODO: user_ossと揃えて再度初期化しているが，この時割り込みを止めるべきかなど要検討

  return;
}

static void C2A_timer_setting_(void)
{
  setIntVector(_TIMER_1_VECTOR, C2A_timer1_handler_);
  clearIntFlag(_TIMER_1_IRQ);
  setIntPriority(_TIMER_1_VECTOR, _T1_IPL_IPC, _T1_SPL_IPC);
  setIntEnable(_TIMER_1_IRQ);
  TMR1 = 0; // clear timer
  // 次の値は1msのクロックアップになりかつ、時刻誤差蓄積が最小になるように実験を行う手動調整した値
  T1CON = TACON_PS_8; // set prescaler
  PR1 = (unsigned int)(10001);

  T1CONSET = TACON_ON;

  return;
}

//static unsigned int counter_ = 0;
//static unsigned int prev_counter_ = 0;

static void __USER_ISR C2A_timer1_handler_(void)
{
  // Debug out to check interrupt frequency
  //counter_ = micros();
  //Printf("TI=%d [us]\n", counter_ - prev_counter_);
  //prev_counter_ = counter_;

  TMGR_count_up_master_clock();
  clearIntFlag(_TIMER_1_IRQ);

  return;
}
