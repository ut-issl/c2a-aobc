// CubeWheel.h

#ifndef _CUBEWHEEL_h
#define _CUBEWHEEL_h


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
	* @struct CUBEWHEEL_Info
	* @brief  CUBEWHEELのテレメトリ情報
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

} CUBEWHEEL_Info;

/**
	* @struct CUBEWHEEL_Driver
	* @brief  CUBEWHEEL_Driver構造体
	*/
typedef struct
{
	struct
	{
		I2C_Config i2c_write;
		I2C_Config i2c_read;
		I2C_Config i2c_slave;;  //!< I2C class
	} driver;
	CUBEWHEEL_Info info;

} CUBEWHEEL_Driver;


int CUBEWHEEL_init(CUBEWHEEL_Driver* CUBEWHEEL_driver,uint8_t ch, uint8_t device_address, uint8_t enable_port_no, uint8_t axis_id);

// これは必要そう
DS_CMD_ERR_CODE CUBEWHEEL_Disable(CUBEWHEEL_Driver* CUBEWHEEL_driver);
DS_CMD_ERR_CODE CUBEWHEEL_Enable(CUBEWHEEL_Driver* CUBEWHEEL_driver);
DS_CMD_ERR_CODE CUBEWHEEL_Startup(CUBEWHEEL_Driver* CUBEWHEEL_driver);
DS_CMD_ERR_CODE CUBEWHEEL_GetStatus(CUBEWHEEL_Driver* CUBEWHEEL_driver);


uint8_t CUBEWHEEL_SetReferenceSpeed(CUBEWHEEL_Driver* CUBEWHEEL_driver, int16_t speed);
uint8_t CUBEWHEEL_SetDutyCycle(CUBEWHEEL_Driver* CUBEWHEEL_driver, int16_t duty);

uint8_t CUBEWHEEL_SetControlMode(CUBEWHEEL_Driver* CUBEWHEEL_driver, uint8_t value);
uint8_t CUBEWHEEL_SetPwmGain(CUBEWHEEL_Driver* CUBEWHEEL_driver, int Ki, uint8_t KiMultiplier);
uint8_t CUBEWHEEL_SetMainGain(CUBEWHEEL_Driver* CUBEWHEEL_driver, uint16_t Ki, uint8_t KiMultiplier, uint16_t Kd, uint8_t KdMultiplier);

uint8_t CUBEWHEEL_GetWheelData(CUBEWHEEL_Driver* CUBEWHEEL_driver);
uint8_t CUBEWHEEL_GetWheelStatus(CUBEWHEEL_Driver* CUBEWHEEL_driver);
uint8_t CUBEWHEEL_GetWheelSpeed(CUBEWHEEL_Driver* CUBEWHEEL_driver);
uint8_t CUBEWHEEL_GetWheelDuty(CUBEWHEEL_Driver* CUBEWHEEL_driver);
uint8_t CUBEWHEEL_GetPwmGain(CUBEWHEEL_Driver* CUBEWHEEL_driver);
uint8_t CUBEWHEEL_GetMainGain(CUBEWHEEL_Driver* CUBEWHEEL_driver);


uint8_t CUBEWHEEL_SetBackupmode(CUBEWHEEL_Driver* CUBEWHEEL_driver, uint8_t en);
uint8_t CUBEWHEEL_EnableEncoder(CUBEWHEEL_Driver* CUBEWHEEL_driver, uint8_t en);
uint8_t CUBEWHEEL_EnableHallsensor(CUBEWHEEL_Driver* CUBEWHEEL_driver, uint8_t en);

/**
 * @brief  回転方向ベクトル設定関数
 * @param  *CUBEWHEEL_driver       : CUBEWHEEL_Driver構造体へのポインタ
 * @param  rotation_direction_b : 機体固定座標系での回転方向ベクトル
 * @return C2A_MATH_ERRORに準じる
 */
C2A_MATH_ERROR CUBEWHEEL_set_rotation_direction_b(CUBEWHEEL_Driver* CUBEWHEEL_driver,
  const float rotation_direction_b[PHYSICAL_CONST_THREE_DIM]);

#endif

