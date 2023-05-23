/*
 Name:		C2A_AOBC_EXAMPLE.ino
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
  // TODO: �����ł����̂��ꏊ�͗v����
  Serial.begin(115200);   // Debug�|�[�g

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
    // ���[�U�[��`loop�����͂����ɓ����
  }

  return;
}

// C2A�֘A�̏�����
// HW�֘A�����i�^�C�}�[�C���荞�ݐݒ�Ȃǁj�̂ݓƎ��ɏ�����
// Printf������ WDT_clear_wdt(); ���Ă΂�Ă邱�Ƃɒ��ӁI
static void C2A_init_(void)
{
  TMGR_init();                // Time Manager
                              // AM_initialize_all_apps �ł̎��Ԍv���̂��߂ɂ����ŏ�����
  Printf("C2A_init: TMGR_init done.\n");

  // �^�C�}�����݊֘A
  C2A_timer_setting_();
  Printf("C2A_init: timer_setting_ done.\n");

  C2A_core_init();

  TMGR_clear(); // TODO: user_oss�Ƒ����čēx���������Ă��邪�C���̎����荞�݂��~�߂�ׂ����ȂǗv����

  return;
}

static void C2A_timer_setting_(void)
{
  setIntVector(_TIMER_1_VECTOR, C2A_timer1_handler_);
  clearIntFlag(_TIMER_1_IRQ);
  setIntPriority(_TIMER_1_VECTOR, _T1_IPL_IPC, _T1_SPL_IPC);
  setIntEnable(_TIMER_1_IRQ);
  TMR1 = 0; // clear timer
  // ���̒l��1ms�̃N���b�N�A�b�v�ɂȂ肩�A�����덷�~�ς��ŏ��ɂȂ�悤�Ɏ������s���蓮���������l
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
