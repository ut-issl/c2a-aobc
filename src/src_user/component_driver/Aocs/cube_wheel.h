#ifndef CUBE_WHEEL_H_
#define CUBE_WHEEL_H_


#include <src_user/Settings/port_config.h>
#include <src_core/Library/print.h>
#include <src_core/Drivers/Super/driver_super.h>
#include "../../Library/physical_constants.h"
#include "../../Library/c2a_math.h"
#include <stdbool.h>
#include <src_user/Library/quaternion.h>

#define RW_AXIS_ID_X 0
#define RW_AXIS_ID_Y 1
#define RW_AXIS_ID_Z 2

#define RW_BACKUP_MODE_OFF 0
#define RW_BACKUP_MODE_ON 1

/**
 * @struct CUBE_WHEEL_Info
 * @brief  CUBE_WHEELのテレメトリ情報
 */
typedef struct
{
  uint8_t enable_port_no;
  uint8_t axis_id;
  bool valid;
  uint16_t runtime;
  uint16_t runtime_in_msec;
  float speed_in_rpm;
  float speed_backup_in_rpm;
  float maxspeed_in_rpm;
  float reference_in_rpm;
  float max_torque;
  float current;
  float duty;
  uint8_t motor_mode;
  bool backup_mode_state;
  bool motor_state;
  bool hall_sensor_state;
  bool encoder_state;
  bool error_flag;
  float gain_pwm;
  uint8_t gain_pwm_multiplier;
  uint16_t feedback_gain;
  uint8_t feedback_gain_multiplier;
  uint16_t integrator_gain;
  uint8_t integrator_gain_multiplier;
  float rotation_direction_b[3];

} CUBE_WHEEL_Info;

/**
* @struct CUBE_WHEEL_Driver
* @brief  CUBE_WHEEL_Driver構造体
*/
typedef struct
{
  struct
  {
    I2C_Config i2c_write;
    I2C_Config i2c_read;
    I2C_Config i2c_slave;  //!< I2C class
  } driver;
  CUBE_WHEEL_Info info;

} CUBE_WHEEL_Driver;


int CUBE_WHEEL_init(CUBE_WHEEL_Driver* CUBE_WHEEL_driver, uint8_t ch, uint8_t device_address, uint8_t enable_port_no, uint8_t axis_id);

// これは必要そう
DS_CMD_ERR_CODE CUBE_WHEEL_Disable(CUBE_WHEEL_Driver* CUBE_WHEEL_driver);
DS_CMD_ERR_CODE CUBE_WHEEL_Enable(CUBE_WHEEL_Driver* CUBE_WHEEL_driver);
DS_CMD_ERR_CODE CUBE_WHEEL_Startup(CUBE_WHEEL_Driver* CUBE_WHEEL_driver);
DS_CMD_ERR_CODE CUBE_WHEEL_GetStatus(CUBE_WHEEL_Driver* CUBE_WHEEL_driver);


uint8_t CUBE_WHEEL_SetReferenceSpeed(CUBE_WHEEL_Driver* CUBE_WHEEL_driver, int16_t speed);
uint8_t CUBE_WHEEL_SetDutyCycle(CUBE_WHEEL_Driver* CUBE_WHEEL_driver, int16_t duty);

uint8_t CUBE_WHEEL_SetControlMode(CUBE_WHEEL_Driver* CUBE_WHEEL_driver, uint8_t value);
uint8_t CUBE_WHEEL_SetPwmGain(CUBE_WHEEL_Driver* CUBE_WHEEL_driver, int Ki, uint8_t KiMultiplier);
uint8_t CUBE_WHEEL_SetMainGain(CUBE_WHEEL_Driver* CUBE_WHEEL_driver, uint16_t Ki, uint8_t KiMultiplier, uint16_t Kd, uint8_t KdMultiplier);

uint8_t CUBE_WHEEL_GetWheelData(CUBE_WHEEL_Driver* CUBE_WHEEL_driver);
uint8_t CUBE_WHEEL_GetWheelStatus(CUBE_WHEEL_Driver* CUBE_WHEEL_driver);
uint8_t CUBE_WHEEL_GetWheelSpeed(CUBE_WHEEL_Driver* CUBE_WHEEL_driver);
uint8_t CUBE_WHEEL_GetWheelDuty(CUBE_WHEEL_Driver* CUBE_WHEEL_driver);
uint8_t CUBE_WHEEL_GetPwmGain(CUBE_WHEEL_Driver* CUBE_WHEEL_driver);
uint8_t CUBE_WHEEL_GetMainGain(CUBE_WHEEL_Driver* CUBE_WHEEL_driver);


uint8_t CUBE_WHEEL_SetBackupmode(CUBE_WHEEL_Driver* CUBE_WHEEL_driver, uint8_t en);
uint8_t CUBE_WHEEL_EnableEncoder(CUBE_WHEEL_Driver* CUBE_WHEEL_driver, uint8_t en);
uint8_t CUBE_WHEEL_EnableHallsensor(CUBE_WHEEL_Driver* CUBE_WHEEL_driver, uint8_t en);

/**
 * @brief  回転方向ベクトル設定関数
 * @param  *CUBE_WHEEL_driver       : CUBE_WHEEL_Driver構造体へのポインタ
 * @param  rotation_direction_b : 機体固定座標系での回転方向ベクトル
 * @return C2A_MATH_ERRORに準じる
 */
C2A_MATH_ERROR CUBE_WHEEL_set_rotation_direction_b(CUBE_WHEEL_Driver* CUBE_WHEEL_driver,
  const float rotation_direction_b[PHYSICAL_CONST_THREE_DIM]);

#endif
