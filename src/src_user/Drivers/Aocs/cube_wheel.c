#pragma section REPRO
/**
* @file   cube_wheel.c
* @brief  cube_wheelのDriver
*/

#include "./cube_wheel.h"
#include <src_user/Library/user_utility.h>
#include "string.h" // for memcpy
#include <src_user/IfWrapper/GPIO.h>
#include <src_core/Drivers/Super/driver_super.h>
#include "../../Library/vector.h"

static uint8_t GetStartupStatus(CUBE_WHEEL_Driver* CUBE_WHEEL_driver);
static void WriteRegister(CUBE_WHEEL_Driver* CUBE_WHEEL_driver, uint8_t register_address, uint8_t* send_data, uint8_t length);
static void ReadRegister(CUBE_WHEEL_Driver* CUBE_WHEEL_driver, uint8_t register_address, uint8_t* receive_data, uint8_t length);
static uint8_t ReadID(CUBE_WHEEL_Driver* CUBE_WHEEL_driver, uint8_t* data);
static uint8_t ReadExtendedID(CUBE_WHEEL_Driver* CUBE_WHEEL_driver, uint8_t* data);
static uint8_t ReadErrorFlag(CUBE_WHEEL_Driver* CUBE_WHEEL_driver);

const static uint8_t CUBE_WHEEL_kCmdIdSetReset_ = 1;
const static uint8_t CUBE_WHEEL_kCmdIdSetWheelSpeed_ = 2;
const static uint8_t CUBE_WHEEL_kCmdIdSetWheelTorque_ = 3;
const static uint8_t CUBE_WHEEL_kCmdIdSetMotorPowerState_ = 7;
const static uint8_t CUBE_WHEEL_kCmdIdSetEncoderPowerState_ = 8;
const static uint8_t CUBE_WHEEL_kCmdIdSetHallPowerState_ = 9;
const static uint8_t CUBE_WHEEL_kCmdIdSetControlMode_ = 10;
const static uint8_t CUBE_WHEEL_kCmdIdSetBackupWheelMode_ = 12;
const static uint8_t CUBE_WHEEL_kCmdIdSetClearErrors_ = 20;
const static uint8_t CUBE_WHEEL_kCmdIdSetI2cAddresss_ = 31;
const static uint8_t CUBE_WHEEL_kCmdIdSetPwmGain_ = 33;
const static uint8_t CUBE_WHEEL_kCmdIdSetMainGain_ = 34;
const static uint8_t CUBE_WHEEL_kCmdIdSetBackupGain_ = 35;

const static uint8_t CUBE_WHEEL_kCmdIdGetIdentification_ = 128;
const static uint8_t CUBE_WHEEL_kCmdIdGetExIdentification_ = 129;
const static uint8_t CUBE_WHEEL_kCmdIdGetWheelStatus_ = 130;
const static uint8_t CUBE_WHEEL_kCmdIdGetWheelSpeed_ = 133;
const static uint8_t CUBE_WHEEL_kCmdIdGetWheelReference_ = 134;
const static uint8_t CUBE_WHEEL_kCmdIdGetWheelCurrent_ = 135;
const static uint8_t CUBE_WHEEL_kCmdIdGetWheelData_ = 137;
const static uint8_t CUBE_WHEEL_kCmdIdGetWheelDataAdditional_ = 138;
const static uint8_t CUBE_WHEEL_kCmdIdGetPwmGain_ = 139;
const static uint8_t CUBE_WHEEL_kCmdIdGetMainGain_ = 140;
const static uint8_t CUBE_WHEEL_kCmdIdGetBackupGain_ = 141;
const static uint8_t CUBE_WHEEL_kCmdIdGetStatusErrorFlag_ = 145;

const static uint8_t CUBE_WHEEL_kCmdLengthSetReset_ = 1;
const static uint8_t CUBE_WHEEL_kCmdLengthSetWheelSpeed_ = 2;
const static uint8_t CUBE_WHEEL_kCmdLengthSetWheelTorque_ = 2;
const static uint8_t CUBE_WHEEL_kCmdLengthSetMotorPowerState_ = 1;
const static uint8_t CUBE_WHEEL_kCmdLengthSetEncoderPowerState_ = 1;
const static uint8_t CUBE_WHEEL_kCmdLengthSetHallPowerState_ = 1;
const static uint8_t CUBE_WHEEL_kCmdLengthSetControlMode_ = 1;
const static uint8_t CUBE_WHEEL_kCmdLengthSetBackupWheelMode = 1;
const static uint8_t CUBE_WHEEL_kCmdLengthSetClearErrors = 1;
const static uint8_t CUBE_WHEEL_kCmdLengthSetI2cAddress = 1;
const static uint8_t CUBE_WHEEL_kCmdLengthSetPwmGain = 3;
const static uint8_t CUBE_WHEEL_kCmdLengthSetMainGain = 6;
const static uint8_t CUBE_WHEEL_kCmdLengthSetBackupGain = 6;


const static uint8_t CUBE_WHEEL_kTlmLengthGetIdentification_ = 8;
const static uint8_t CUBE_WHEEL_kTlmLengthGetExIdentification_ = 4;
const static uint8_t CUBE_WHEEL_kTlmLengthGetWheelStatus_ = 8;
const static uint8_t CUBE_WHEEL_kTlmLengthGetWheelSpeed_ = 2;
const static uint8_t CUBE_WHEEL_kTlmLengthGetWheelReference_ = 2;
const static uint8_t CUBE_WHEEL_kTlmLengthGetWheelCurrent_ = 2;
const static uint8_t CUBE_WHEEL_kTlmLengthGetWheelData_ = 6;
const static uint8_t CUBE_WHEEL_kTlmLengthGetWheelDataAdditional_ = 4;
const static uint8_t CUBE_WHEEL_kTlmLengthGetPwmGain_ = 4;
const static uint8_t CUBE_WHEEL_kTlmLengthGetMainGain_ = 7;
const static uint8_t CUBE_WHEEL_kTlmLengthGetBackupGain_ = 7;
const static uint8_t CUBE_WHEEL_kTlmLengthGetStatusErrorFlag_ = 1;

const static uint8_t CUBE_WHEEL_kConvertValueOfCurrentFactor_ = 0.48828125;

// Endianがビッグエンディアン

int CUBE_WHEEL_init(CUBE_WHEEL_Driver* CUBE_WHEEL_driver, uint8_t ch, uint8_t device_address, uint8_t enable_port_no, uint8_t axis_id)
{

  CUBE_WHEEL_driver->info.enable_port_no = enable_port_no;
  GPIO_set_direction(CUBE_WHEEL_driver->info.enable_port_no, GPIO_OUTPUT);

  CUBE_WHEEL_driver->info.axis_id = axis_id;
  CUBE_WHEEL_driver->info.maxspeed_in_rpm = 8000; // rpm
  CUBE_WHEEL_driver->info.max_torque = 0.23; // mNm

  CUBE_WHEEL_driver->info.rotation_direction_b[0] = 1.0f;
  CUBE_WHEEL_driver->info.rotation_direction_b[1] = 0.0f;
  CUBE_WHEEL_driver->info.rotation_direction_b[2] = 0.0f;

  CUBE_WHEEL_driver->driver.i2c_write.ch = ch;
  CUBE_WHEEL_driver->driver.i2c_write.frequency_khz = 100;
  CUBE_WHEEL_driver->driver.i2c_write.device_address = (device_address << 1); // write
  CUBE_WHEEL_driver->driver.i2c_write.stop_flag = 1;
  CUBE_WHEEL_driver->driver.i2c_write.timeout_threshold = 500;
  CUBE_WHEEL_driver->driver.i2c_write.rx_length = 8;

  // todo: ここはreopen?
  if (I2C_init(&CUBE_WHEEL_driver->driver.i2c_write) != 0)
  {
    Printf("Error: I2C_init in i2c_rw_write_.\n");
  }

  CUBE_WHEEL_driver->driver.i2c_read.ch = ch;
  CUBE_WHEEL_driver->driver.i2c_read.frequency_khz = 100;
  CUBE_WHEEL_driver->driver.i2c_read.device_address = (device_address << 1) + 1; // read
  CUBE_WHEEL_driver->driver.i2c_read.stop_flag = 1;
  CUBE_WHEEL_driver->driver.i2c_read.timeout_threshold = 500;
  CUBE_WHEEL_driver->driver.i2c_read.rx_length = 8;

  // todo: ここはreopen?
  if (I2C_init(&CUBE_WHEEL_driver->driver.i2c_read) != 0)
  {
    Printf("Error: I2C_init in i2c_rw_read_.\n");
  }

  CUBE_WHEEL_driver->driver.i2c_slave.ch = ch;
  CUBE_WHEEL_driver->driver.i2c_slave.frequency_khz = 100;
  CUBE_WHEEL_driver->driver.i2c_slave.device_address = device_address; // slave
  CUBE_WHEEL_driver->driver.i2c_slave.stop_flag = 1;
  CUBE_WHEEL_driver->driver.i2c_slave.timeout_threshold = 500;
  CUBE_WHEEL_driver->driver.i2c_slave.rx_length = 8;

  // todo: ここはreopen?
  if (I2C_init(&CUBE_WHEEL_driver->driver.i2c_slave) != 0)
  {
    Printf("Error: I2C_init in i2c_rw_slave_.\n");
  }

  return 0;
}

DS_CMD_ERR_CODE CUBE_WHEEL_Disable(CUBE_WHEEL_Driver* CUBE_WHEEL_driver)
{
  GPIO_set_output(CUBE_WHEEL_driver->info.enable_port_no, GPIO_LOW);

  return DS_CMD_OK;
}

DS_CMD_ERR_CODE CUBE_WHEEL_Enable(CUBE_WHEEL_Driver* CUBE_WHEEL_driver)
{
  int i_ret = 0;

  GPIO_set_output(CUBE_WHEEL_driver->info.enable_port_no, GPIO_HIGH);

  if (I2C_reopen(&CUBE_WHEEL_driver->driver.i2c_slave, 0) != 0)
  {
    Printf("Error: I2C_init in i2c_rw_slave_.\n");
    return DS_CMD_UNKNOWN_ERR;
  }
  else
  {
    return DS_CMD_OK;
  }

  return DS_CMD_UNKNOWN_ERR;
}


DS_CMD_ERR_CODE CUBE_WHEEL_Startup(CUBE_WHEEL_Driver* CUBE_WHEEL_driver)
{
  if (GetStartupStatus(CUBE_WHEEL_driver) == 0)
  {
    // set speed mode in initial state
    CUBE_WHEEL_SetControlMode(CUBE_WHEEL_driver, 3);

    Printf("Success Startup: RW id= %d\n", CUBE_WHEEL_driver->info.axis_id);
    CUBE_WHEEL_driver->info.valid = true;
    return DS_CMD_OK;
  }
  else
  {
    Printf("Error Startup: RW id = %d\n", CUBE_WHEEL_driver->info.axis_id);
    CUBE_WHEEL_driver->info.valid = false;
    return DS_CMD_UNKNOWN_ERR;
  }

  // GetStartupStatusからの返り値が戻り値になっている．
  return DS_CMD_OK;
}

DS_CMD_ERR_CODE CUBE_WHEEL_GetStatus(CUBE_WHEEL_Driver* CUBE_WHEEL_driver)
{
  if (CUBE_WHEEL_driver->info.valid)
  {
    CUBE_WHEEL_GetWheelStatus(CUBE_WHEEL_driver);
    CUBE_WHEEL_GetWheelDuty(CUBE_WHEEL_driver);
    CUBE_WHEEL_GetWheelSpeed(CUBE_WHEEL_driver);
    CUBE_WHEEL_GetPwmGain(CUBE_WHEEL_driver);
    CUBE_WHEEL_GetMainGain(CUBE_WHEEL_driver);

  }
  return DS_CMD_OK;
}

uint8_t CUBE_WHEEL_SetDutyCycle(CUBE_WHEEL_Driver* CUBE_WHEEL_driver, int16_t duty)
{
  int i_ret = 0;

  uint8_t data[2];
  data[1] = (duty >> 8) & 0xFF;
  data[0] = duty & 0xFF;
  WriteRegister(CUBE_WHEEL_driver, CUBE_WHEEL_kCmdIdSetWheelTorque_, data, sizeof(data));

  return i_ret;
}

uint8_t CUBE_WHEEL_SetReferenceSpeed(CUBE_WHEEL_Driver* CUBE_WHEEL_driver, int16_t speed)
{
  int i_ret = 0;
  int16_t raw_value = speed * 2;

  uint8_t data[2] = { 0 };
  data[1] = (raw_value >> 8) & 0xFF;
  data[0] = raw_value & 0xFF;
  WriteRegister(CUBE_WHEEL_driver, CUBE_WHEEL_kCmdIdSetWheelSpeed_, data, sizeof(data));

  return i_ret;
}

uint8_t CUBE_WHEEL_SetControlMode(CUBE_WHEEL_Driver* CUBE_WHEEL_driver, uint8_t value)
{
  int i_ret = 0;

  uint8_t data[1] = { 0 };
  data[0] = value & 0xFF;
  WriteRegister(CUBE_WHEEL_driver, CUBE_WHEEL_kCmdIdSetControlMode_, data, sizeof(data));

  return i_ret;
}
uint8_t CUBE_WHEEL_SetPwmGain(CUBE_WHEEL_Driver* CUBE_WHEEL_driver, int Ki, uint8_t KiMultiplier)
{
  int i_ret = 0;

  uint8_t data[3] = { 0 };
  data[1] = (Ki >> 8) & 0xFF;
  data[0] = Ki & 0xFF;

  data[2] = KiMultiplier & 0xFF;

  WriteRegister(CUBE_WHEEL_driver, CUBE_WHEEL_kCmdIdSetPwmGain_, data, sizeof(data));

  return i_ret;
}

uint8_t CUBE_WHEEL_SetMainGain(CUBE_WHEEL_Driver* CUBE_WHEEL_driver, uint16_t Ki, uint8_t KiMultiplier, uint16_t Kd, uint8_t KdMultiplier)
{

  int i_ret = 0;

  uint8_t data[6] = { 0 };
  data[1] = (Ki >> 8) & 0xFF;
  data[0] = Ki & 0xFF;
  data[2] = KiMultiplier & 0xFF;

  data[4] = (Kd >> 8) & 0xFF;
  data[3] = Kd & 0xFF;
  data[5] = KdMultiplier & 0xFF;

  WriteRegister(CUBE_WHEEL_driver, CUBE_WHEEL_kCmdIdSetMainGain_, data, sizeof(data));

  return i_ret;
}

uint8_t CUBE_WHEEL_GetWheelData(CUBE_WHEEL_Driver* CUBE_WHEEL_driver)
{
  int i_ret = 0;
  uint8_t data[CUBE_WHEEL_kTlmLengthGetWheelData_];
  uint8_t s_data[2] = { 0 };
  ReadRegister(CUBE_WHEEL_driver, CUBE_WHEEL_kCmdIdGetWheelData_, data, CUBE_WHEEL_kTlmLengthGetWheelData_);

  short raw_value = 0;

  s_data[0] = data[0];
  s_data[1] = data[1];
  raw_value = raw2signedshort_little(s_data);
  CUBE_WHEEL_driver->info.speed_in_rpm = (float)(raw_value / 2);

  s_data[0] = data[2];
  s_data[1] = data[3];
  raw_value = raw2signedshort_little(s_data);
  CUBE_WHEEL_driver->info.reference_in_rpm = (float)(raw_value / 2);

  uint16_t uraw_value = 0;
  uraw_value = data[5] << 8;
  uraw_value += data[4];
  CUBE_WHEEL_driver->info.current = (float)(uraw_value * CUBE_WHEEL_kConvertValueOfCurrentFactor_);

  return i_ret;
}

uint8_t CUBE_WHEEL_GetWheelStatus(CUBE_WHEEL_Driver* CUBE_WHEEL_driver)
{
  int i_ret = 0;
  uint8_t data[CUBE_WHEEL_kTlmLengthGetWheelStatus_];
  ReadRegister(CUBE_WHEEL_driver, CUBE_WHEEL_kCmdIdGetWheelStatus_, data, CUBE_WHEEL_kTlmLengthGetWheelStatus_);

  uint16_t raw_value = 0;
  raw_value = data[1] << 8;
  raw_value += data[0];
  CUBE_WHEEL_driver->info.runtime = (uint16_t)raw_value;

  raw_value = data[3] << 8;
  raw_value += data[2];
  CUBE_WHEEL_driver->info.runtime_in_msec = (uint16_t)raw_value;

  raw_value = data[5] << 8; // reserve
  raw_value += data[4];  // reserve

  raw_value = data[6];
  CUBE_WHEEL_driver->info.motor_mode = (uint8_t)raw_value;

  raw_value = data[7];
  CUBE_WHEEL_driver->info.backup_mode_state = raw_value & 0x01;
  CUBE_WHEEL_driver->info.motor_state = (raw_value >> 1) & 0x01;
  CUBE_WHEEL_driver->info.hall_sensor_state = (raw_value >> 2) & 0x01;
  CUBE_WHEEL_driver->info.encoder_state = (raw_value >> 3) & 0x01;
  CUBE_WHEEL_driver->info.error_flag = (raw_value >> 4) & 0x01;

  return i_ret;
}

uint8_t CUBE_WHEEL_GetWheelSpeed(CUBE_WHEEL_Driver* CUBE_WHEEL_driver)
{
  int i_ret = 0;

  uint8_t data[CUBE_WHEEL_kTlmLengthGetWheelSpeed_];
  ReadRegister(CUBE_WHEEL_driver, CUBE_WHEEL_kCmdIdGetWheelSpeed_, data, CUBE_WHEEL_kTlmLengthGetWheelSpeed_);

  int raw_value = 0;
  uint8_t s_data[2] = { 0 };

  s_data[0] = data[0];
  s_data[1] = data[1];
  raw_value = raw2signedshort_little(s_data);
  CUBE_WHEEL_driver->info.speed_in_rpm = (float)(raw_value / 2);

  return i_ret;
}

uint8_t CUBE_WHEEL_GetWheelDuty(CUBE_WHEEL_Driver* CUBE_WHEEL_driver)
{
  uint8_t data[CUBE_WHEEL_kTlmLengthGetWheelDataAdditional_];
  ReadRegister(CUBE_WHEEL_driver, CUBE_WHEEL_kCmdIdGetWheelDataAdditional_, data, CUBE_WHEEL_kTlmLengthGetWheelDataAdditional_);

  int raw_value = 0;
  uint8_t s_data[2] = { 0 };

  s_data[0] = data[0];
  s_data[1] = data[1];
  raw_value = raw2signedshort_little(s_data);
  CUBE_WHEEL_driver->info.duty = (float)(raw_value);

  s_data[0] = data[2];
  s_data[1] = data[3];
  raw_value = raw2signedshort_little(s_data);
  CUBE_WHEEL_driver->info.speed_backup_in_rpm = (float)(raw_value / 2);

  return 0;
}

uint8_t CUBE_WHEEL_GetPwmGain(CUBE_WHEEL_Driver* CUBE_WHEEL_driver)
{
  int i_ret = 0;
  uint8_t data[CUBE_WHEEL_kTlmLengthGetPwmGain_];
  ReadRegister(CUBE_WHEEL_driver, CUBE_WHEEL_kCmdIdGetPwmGain_, data, CUBE_WHEEL_kTlmLengthGetPwmGain_);

  int raw_value = 0;
  uint8_t s_data[2] = { 0 };

  s_data[0] = data[0];
  s_data[1] = data[1];
  raw_value = raw2signedshort_little(s_data);
  CUBE_WHEEL_driver->info.gain_pwm = (float)raw_value;


  raw_value = data[2];
  CUBE_WHEEL_driver->info.gain_pwm_multiplier = (uint8_t)raw_value;

  return i_ret;
}

uint8_t CUBE_WHEEL_GetMainGain(CUBE_WHEEL_Driver* CUBE_WHEEL_driver)
{
  int i_ret = 0;
  uint8_t data[CUBE_WHEEL_kTlmLengthGetMainGain_];
  ReadRegister(CUBE_WHEEL_driver, CUBE_WHEEL_kCmdIdGetMainGain_, data, CUBE_WHEEL_kTlmLengthGetMainGain_);

  uint16_t raw_value = 0;
  raw_value = data[1] << 8;
  raw_value += data[0];
  CUBE_WHEEL_driver->info.feedback_gain = raw_value;

  raw_value = data[2];
  CUBE_WHEEL_driver->info.feedback_gain_multiplier = (uint8_t)raw_value;

  raw_value = data[4] << 8;
  raw_value += data[3];
  CUBE_WHEEL_driver->info.integrator_gain = raw_value;

  raw_value = data[5];
  CUBE_WHEEL_driver->info.integrator_gain_multiplier = (uint8_t)raw_value;

  return i_ret;

}

uint8_t CUBE_WHEEL_SetBackupmode(CUBE_WHEEL_Driver* CUBE_WHEEL_driver, uint8_t enable_status)
{
  int i_ret = 0;
  uint8_t data[1] = { 0 };
  data[0] = enable_status & 0xFF;
  WriteRegister(CUBE_WHEEL_driver, CUBE_WHEEL_kCmdIdSetBackupWheelMode_, data, sizeof(data));

  return i_ret;
}

uint8_t CUBE_WHEEL_EnableEncoder(CUBE_WHEEL_Driver* CUBE_WHEEL_driver, uint8_t enable_status)
{
  int i_ret = 0;
  uint8_t data[1] = { 0 };
  data[0] = enable_status & 0xFF;
  WriteRegister(CUBE_WHEEL_driver, CUBE_WHEEL_kCmdIdSetEncoderPowerState_, data, sizeof(data));

  return i_ret;
}

uint8_t CUBE_WHEEL_EnableHallsensor(CUBE_WHEEL_Driver* CUBE_WHEEL_driver, uint8_t enable_status)
{
  int i_ret = 0;
  uint8_t data[1] = { 0 };
  data[0] = enable_status & 0xFF;
  WriteRegister(CUBE_WHEEL_driver, CUBE_WHEEL_kCmdIdSetHallPowerState_, data, sizeof(data));

  return i_ret;
}

static uint8_t GetStartupStatus(CUBE_WHEEL_Driver* CUBE_WHEEL_driver)
{
  uint8_t i_ret = 0;
  uint8_t data[CUBE_WHEEL_kTlmLengthGetIdentification_];
  i_ret = ReadID(CUBE_WHEEL_driver, data);
  if (i_ret != 0)
  {
    return i_ret;
  }
  i_ret = ReadExtendedID(CUBE_WHEEL_driver, data);
  return i_ret;
}

static void WriteRegister(CUBE_WHEEL_Driver* CUBE_WHEEL_driver, uint8_t register_address, uint8_t* send_data, uint8_t length)
{
  uint8_t send_data_buffer[10] = { 0 };

  // Make send data
  send_data_buffer[0] = register_address;

  // todo error処理
  if (length > 10)
  {
    return;
  }
  memcpy(&send_data_buffer[1], &send_data[0], length);

  // Send data
  I2C_tx(&CUBE_WHEEL_driver->driver.i2c_slave, &send_data_buffer[0], length + 1);

  return;
}

static void ReadRegister(CUBE_WHEEL_Driver* CUBE_WHEEL_driver, uint8_t register_address, uint8_t* receive_data, uint8_t length)
{
  // Resister set
  uint8_t send_data_buffer[1] = { 0 };
  send_data_buffer[0] = register_address;
  I2C_tx(&CUBE_WHEEL_driver->driver.i2c_slave, &send_data_buffer[0], 1);

  // Read data
  CUBE_WHEEL_driver->driver.i2c_slave.rx_length = length;
  I2C_rx(&CUBE_WHEEL_driver->driver.i2c_slave, &receive_data[0], length);
  return;
}

static uint8_t ReadID(CUBE_WHEEL_Driver* CUBE_WHEEL_driver, uint8_t* data)
{
  ReadRegister(CUBE_WHEEL_driver, CUBE_WHEEL_kCmdIdGetIdentification_, data, CUBE_WHEEL_kTlmLengthGetIdentification_);

  // TLM128のdata[0]は8固定。
  // Printf("ReadID = %d\n", data[0]);
  if (data[0] != 0x08)
  {
    return(1);
  }

  return 0;
}


static uint8_t ReadExtendedID(CUBE_WHEEL_Driver* CUBE_WHEEL_driver, uint8_t* data)
{
  ReadRegister(CUBE_WHEEL_driver, CUBE_WHEEL_kCmdIdGetExIdentification_, data, CUBE_WHEEL_kTlmLengthGetExIdentification_);

  // TLM129のdata[2]はI2Cアドレス
  // Printf("ReadExtendedID = %d : %d", data[2], CUBE_WHEEL_driver->driver.i2c_write.device_address);
  if (data[2] != CUBE_WHEEL_driver->driver.i2c_write.device_address)
  {
    return(1);
  }

  return 0;
}

uint8_t ReadErrorFlag(CUBE_WHEEL_Driver* CUBE_WHEEL_driver)
{
  int i_ret = 0;
  uint8_t data[CUBE_WHEEL_kTlmLengthGetStatusErrorFlag_];

  ReadRegister(CUBE_WHEEL_driver, CUBE_WHEEL_kCmdIdGetStatusErrorFlag_, data, CUBE_WHEEL_kTlmLengthGetStatusErrorFlag_);


  // Printf("Cube Wheel Error flag:");
  int i;
  for (i = 0; i < CUBE_WHEEL_kTlmLengthGetStatusErrorFlag_; i++)
  {
    // Printf(data[i], HEX); Printf(" ");
  }
  // Printf("");

  return i_ret;
}

C2A_MATH_ERROR CUBE_WHEEL_set_rotation_direction_b(CUBE_WHEEL_Driver* CUBE_WHEEL_driver,
  const float rotation_direction_b[PHYSICAL_CONST_THREE_DIM])
{
  C2A_MATH_ERROR is_normalized = VECTOR_is_normalized(rotation_direction_b);
  if (is_normalized != C2A_MATH_ERROR_OK) return is_normalized;
  VECTOR_copy(CUBE_WHEEL_driver->info.rotation_direction_b,
    rotation_direction_b);
  return C2A_MATH_ERROR_OK;
}

#pragma section
