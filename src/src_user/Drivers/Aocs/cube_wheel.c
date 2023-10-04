#pragma section REPRO
/**
* @file   cubewheel.c
* @brief  cubewheelのDriver
*/

#include "./cubewheel.h"
#include <src_user/Library/user_utility.h>
#include "string.h" // for memcpy
#include <src_user/IfWrapper/GPIO.h>
#include <src_core/Drivers/Super/driver_super.h>
#include "../../Library/vector.h"

static uint8_t GetStartupStatus(CUBEWHEEL_Driver* CUBEWHEEL_driver);
static void WriteRegister(CUBEWHEEL_Driver* CUBEWHEEL_driver, uint8_t register_address, uint8_t* send_data, uint8_t length);
static void ReadRegister(CUBEWHEEL_Driver* CUBEWHEEL_driver, uint8_t register_address, uint8_t* receive_data, uint8_t length);
static uint8_t ReadID(CUBEWHEEL_Driver* CUBEWHEEL_driver, uint8_t* data);
static uint8_t ReadExtendedID(CUBEWHEEL_Driver* CUBEWHEEL_driver, uint8_t* data);
static uint8_t ReadErrorFlag(CUBEWHEEL_Driver* CUBEWHEEL_driver);

const static uint8_t CUBEWHEEL_kCmdIdSetReset_ = 1;
const static uint8_t CUBEWHEEL_kCmdIdSetWheelSpeed_ = 2;
const static uint8_t CUBEWHEEL_kCmdIdSetWheelTorque_ = 3;
const static uint8_t CUBEWHEEL_kCmdIdSetMotorPowerState_ = 7;
const static uint8_t CUBEWHEEL_kCmdIdSetEncoderPowerState_ = 8;
const static uint8_t CUBEWHEEL_kCmdIdSetHallPowerState_ = 9;
const static uint8_t CUBEWHEEL_kCmdIdSetControlMode_ = 10;
const static uint8_t CUBEWHEEL_kCmdIdSetBackupWheelMode_ = 12;
const static uint8_t CUBEWHEEL_kCmdIdSetClearErrors_ = 20;
const static uint8_t CUBEWHEEL_kCmdIdSetI2cAddresss_ = 31;
const static uint8_t CUBEWHEEL_kCmdIdSetPwmGain_ = 33;
const static uint8_t CUBEWHEEL_kCmdIdSetMainGain_ = 34;
const static uint8_t CUBEWHEEL_kCmdIdSetBackupGain_ = 35;

const static uint8_t CUBEWHEEL_kCmdIdGetIdentification_ = 128;
const static uint8_t CUBEWHEEL_kCmdIdGetExIdentification_ = 129;
const static uint8_t CUBEWHEEL_kCmdIdGetWheelStatus_ = 130;
const static uint8_t CUBEWHEEL_kCmdIdGetWheelSpeed_ = 133;
const static uint8_t CUBEWHEEL_kCmdIdGetWheelReference_ = 134;
const static uint8_t CUBEWHEEL_kCmdIdGetWheelCurrent_ = 135;
const static uint8_t CUBEWHEEL_kCmdIdGetWheelData_ = 137;
const static uint8_t CUBEWHEEL_kCmdIdGetWheelDataAdditional_ = 138;
const static uint8_t CUBEWHEEL_kCmdIdGetPwmGain_ = 139;
const static uint8_t CUBEWHEEL_kCmdIdGetMainGain_ = 140;
const static uint8_t CUBEWHEEL_kCmdIdGetBackupGain_ = 141;
const static uint8_t CUBEWHEEL_kCmdIdGetStatusErrorFlag_ = 145;

const static uint8_t CUBEWHEEL_kCmdLengthSetReset_ = 1;
const static uint8_t CUBEWHEEL_kCmdLengthSetWheelSpeed_ = 2;
const static uint8_t CUBEWHEEL_kCmdLengthSetWheelTorque_ = 2;
const static uint8_t CUBEWHEEL_kCmdLengthSetMotorPowerState_ = 1;
const static uint8_t CUBEWHEEL_kCmdLengthSetEncoderPowerState_ = 1;
const static uint8_t CUBEWHEEL_kCmdLengthSetHallPowerState_ = 1;
const static uint8_t CUBEWHEEL_kCmdLengthSetControlMode_ = 1;
const static uint8_t CUBEWHEEL_kCmdLengthSetBackupWheelMode = 1;
const static uint8_t CUBEWHEEL_kCmdLengthSetClearErrors = 1;
const static uint8_t CUBEWHEEL_kCmdLengthSetI2cAddress = 1;
const static uint8_t CUBEWHEEL_kCmdLengthSetPwmGain = 3;
const static uint8_t CUBEWHEEL_kCmdLengthSetMainGain = 6;
const static uint8_t CUBEWHEEL_kCmdLengthSetBackupGain = 6;


const static uint8_t CUBEWHEEL_kTlmLengthGetIdentification_ = 8;
const static uint8_t CUBEWHEEL_kTlmLengthGetExIdentification_ = 4;
const static uint8_t CUBEWHEEL_kTlmLengthGetWheelStatus_ = 8;
const static uint8_t CUBEWHEEL_kTlmLengthGetWheelSpeed_ = 2;
const static uint8_t CUBEWHEEL_kTlmLengthGetWheelReference_ = 2;
const static uint8_t CUBEWHEEL_kTlmLengthGetWheelCurrent_ = 2;
const static uint8_t CUBEWHEEL_kTlmLengthGetWheelData_ = 6;
const static uint8_t CUBEWHEEL_kTlmLengthGetWheelDataAdditional_ = 4;
const static uint8_t CUBEWHEEL_kTlmLengthGetPwmGain_ = 4;
const static uint8_t CUBEWHEEL_kTlmLengthGetMainGain_ = 7;
const static uint8_t CUBEWHEEL_kTlmLengthGetBackupGain_ = 7;
const static uint8_t CUBEWHEEL_kTlmLengthGetStatusErrorFlag_ = 1;

const static uint8_t CUBEWHEEL_kConvertValueOfCurrentFactor_ = 0.48828125;

// Endianがビッグエンディアン

int CUBEWHEEL_init(CUBEWHEEL_Driver* CUBEWHEEL_driver, uint8_t ch, uint8_t device_address, uint8_t enable_port_no, uint8_t axis_id)
{

  CUBEWHEEL_driver->info.enable_port_no = enable_port_no;
  GPIO_set_direction(CUBEWHEEL_driver->info.enable_port_no, GPIO_OUTPUT);

  CUBEWHEEL_driver->info.axis_id = axis_id;
  CUBEWHEEL_driver->info.maxspeed_in_rpm = 8000; // rpm
  CUBEWHEEL_driver->info.max_torque = 0.23; // mNm

  CUBEWHEEL_driver->info.rotation_direction_b[0] = 1.0f;
  CUBEWHEEL_driver->info.rotation_direction_b[1] = 0.0f;
  CUBEWHEEL_driver->info.rotation_direction_b[2] = 0.0f;

  CUBEWHEEL_driver->driver.i2c_write.ch = ch;
  CUBEWHEEL_driver->driver.i2c_write.frequency_khz = 100;
  CUBEWHEEL_driver->driver.i2c_write.device_address = (device_address << 1); // write
  CUBEWHEEL_driver->driver.i2c_write.stop_flag = 1;
  CUBEWHEEL_driver->driver.i2c_write.timeout_threshold = 500;
  CUBEWHEEL_driver->driver.i2c_write.rx_length = 8;

  // todo: ここはreopen?
  if (I2C_init(&CUBEWHEEL_driver->driver.i2c_write) != 0)
  {
    Printf("Error: I2C_init in i2c_rw_write_.\n");
  }

  CUBEWHEEL_driver->driver.i2c_read.ch = ch;
  CUBEWHEEL_driver->driver.i2c_read.frequency_khz = 100;
  CUBEWHEEL_driver->driver.i2c_read.device_address = (device_address << 1) + 1; // read
  CUBEWHEEL_driver->driver.i2c_read.stop_flag = 1;
  CUBEWHEEL_driver->driver.i2c_read.timeout_threshold = 500;
  CUBEWHEEL_driver->driver.i2c_read.rx_length = 8;

  // todo: ここはreopen?
  if (I2C_init(&CUBEWHEEL_driver->driver.i2c_read) != 0)
  {
    Printf("Error: I2C_init in i2c_rw_read_.\n");
  }

  CUBEWHEEL_driver->driver.i2c_slave.ch = ch;
  CUBEWHEEL_driver->driver.i2c_slave.frequency_khz = 100;
  CUBEWHEEL_driver->driver.i2c_slave.device_address = device_address; // slave
  CUBEWHEEL_driver->driver.i2c_slave.stop_flag = 1;
  CUBEWHEEL_driver->driver.i2c_slave.timeout_threshold = 500;
  CUBEWHEEL_driver->driver.i2c_slave.rx_length = 8;

  // todo: ここはreopen?
  if (I2C_init(&CUBEWHEEL_driver->driver.i2c_slave) != 0)
  {
    Printf("Error: I2C_init in i2c_rw_slave_.\n");
  }

  return 0;
}

DS_CMD_ERR_CODE CUBEWHEEL_Disable(CUBEWHEEL_Driver* CUBEWHEEL_driver)
{
  GPIO_set_output(CUBEWHEEL_driver->info.enable_port_no, GPIO_LOW);

  return DS_CMD_OK;
}

DS_CMD_ERR_CODE CUBEWHEEL_Enable(CUBEWHEEL_Driver* CUBEWHEEL_driver)
{
  int i_ret = 0;

  GPIO_set_output(CUBEWHEEL_driver->info.enable_port_no, GPIO_HIGH);

  if (I2C_reopen(&CUBEWHEEL_driver->driver.i2c_slave, 0) != 0)
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


DS_CMD_ERR_CODE CUBEWHEEL_Startup(CUBEWHEEL_Driver* CUBEWHEEL_driver)
{
  if (GetStartupStatus(CUBEWHEEL_driver) == 0)
  {
    // set speed mode in initial state
    CUBEWHEEL_SetControlMode(CUBEWHEEL_driver, 3);

    Printf("Success Startup: RW id= %d\n", CUBEWHEEL_driver->info.axis_id);
    CUBEWHEEL_driver->info.valid = true;
    return DS_CMD_OK;
  }
  else
  {
    Printf("Error Startup: RW id = %d\n", CUBEWHEEL_driver->info.axis_id);
    CUBEWHEEL_driver->info.valid = false;
    return DS_CMD_UNKNOWN_ERR;
  }

  // GetStartupStatusからの返り値が戻り値になっている．
  return DS_CMD_OK;
}

DS_CMD_ERR_CODE CUBEWHEEL_GetStatus(CUBEWHEEL_Driver* CUBEWHEEL_driver)
{
  if (CUBEWHEEL_driver->info.valid)
  {
    CUBEWHEEL_GetWheelStatus(CUBEWHEEL_driver);
    CUBEWHEEL_GetWheelDuty(CUBEWHEEL_driver);
    CUBEWHEEL_GetWheelSpeed(CUBEWHEEL_driver);
    CUBEWHEEL_GetPwmGain(CUBEWHEEL_driver);
    CUBEWHEEL_GetMainGain(CUBEWHEEL_driver);

  }
  return DS_CMD_OK;
}

uint8_t CUBEWHEEL_SetDutyCycle(CUBEWHEEL_Driver* CUBEWHEEL_driver, int16_t duty)
{
  int i_ret = 0;

  uint8_t data[2];
  data[1] = (duty >> 8) & 0xFF;
  data[0] = duty & 0xFF;
  WriteRegister(CUBEWHEEL_driver, CUBEWHEEL_kCmdIdSetWheelTorque_, data, sizeof(data));

  return i_ret;
}

uint8_t CUBEWHEEL_SetReferenceSpeed(CUBEWHEEL_Driver* CUBEWHEEL_driver, int16_t speed)
{
  int i_ret = 0;
  int16_t raw_value = speed * 2;

  uint8_t data[2] = { 0 };
  data[1] = (raw_value >> 8) & 0xFF;
  data[0] = raw_value & 0xFF;
  WriteRegister(CUBEWHEEL_driver, CUBEWHEEL_kCmdIdSetWheelSpeed_, data, sizeof(data));

  return i_ret;
}

uint8_t CUBEWHEEL_SetControlMode(CUBEWHEEL_Driver* CUBEWHEEL_driver, uint8_t value)
{
  int i_ret = 0;

  uint8_t data[1] = { 0 };
  data[0] = value & 0xFF;
  WriteRegister(CUBEWHEEL_driver, CUBEWHEEL_kCmdIdSetControlMode_, data, sizeof(data));

  return i_ret;
}
uint8_t CUBEWHEEL_SetPwmGain(CUBEWHEEL_Driver* CUBEWHEEL_driver, int Ki, uint8_t KiMultiplier)
{
  int i_ret = 0;

  uint8_t data[3] = { 0 };
  data[1] = (Ki >> 8) & 0xFF;
  data[0] = Ki & 0xFF;

  data[2] = KiMultiplier & 0xFF;

  WriteRegister(CUBEWHEEL_driver, CUBEWHEEL_kCmdIdSetPwmGain_, data, sizeof(data));

  return i_ret;
}

uint8_t CUBEWHEEL_SetMainGain(CUBEWHEEL_Driver* CUBEWHEEL_driver, uint16_t Ki, uint8_t KiMultiplier, uint16_t Kd, uint8_t KdMultiplier)
{

  int i_ret = 0;

  uint8_t data[6] = { 0 };
  data[1] = (Ki >> 8) & 0xFF;
  data[0] = Ki & 0xFF;
  data[2] = KiMultiplier & 0xFF;

  data[4] = (Kd >> 8) & 0xFF;
  data[3] = Kd & 0xFF;
  data[5] = KdMultiplier & 0xFF;

  WriteRegister(CUBEWHEEL_driver, CUBEWHEEL_kCmdIdSetMainGain_, data, sizeof(data));

  return i_ret;
}

uint8_t CUBEWHEEL_GetWheelData(CUBEWHEEL_Driver* CUBEWHEEL_driver)
{
  int i_ret = 0;
  uint8_t data[CUBEWHEEL_kTlmLengthGetWheelData_];
  uint8_t s_data[2] = { 0 };
  ReadRegister(CUBEWHEEL_driver, CUBEWHEEL_kCmdIdGetWheelData_, data, CUBEWHEEL_kTlmLengthGetWheelData_);

  short raw_value = 0;

  s_data[0] = data[0];
  s_data[1] = data[1];
  raw_value = raw2signedshort_little(s_data);
  CUBEWHEEL_driver->info.speed_in_rpm = (float)(raw_value / 2);

  s_data[0] = data[2];
  s_data[1] = data[3];
  raw_value = raw2signedshort_little(s_data);
  CUBEWHEEL_driver->info.reference_in_rpm = (float)(raw_value / 2);

  uint16_t uraw_value = 0;
  uraw_value = data[5] << 8;
  uraw_value += data[4];
  CUBEWHEEL_driver->info.current = (float)(uraw_value * CUBEWHEEL_kConvertValueOfCurrentFactor_);

  return i_ret;
}

uint8_t CUBEWHEEL_GetWheelStatus(CUBEWHEEL_Driver* CUBEWHEEL_driver)
{
  int i_ret = 0;
  uint8_t data[CUBEWHEEL_kTlmLengthGetWheelStatus_];
  ReadRegister(CUBEWHEEL_driver, CUBEWHEEL_kCmdIdGetWheelStatus_, data, CUBEWHEEL_kTlmLengthGetWheelStatus_);

  uint16_t raw_value = 0;
  raw_value = data[1] << 8;
  raw_value += data[0];
  CUBEWHEEL_driver->info.runtime = (uint16_t)raw_value;

  raw_value = data[3] << 8;
  raw_value += data[2];
  CUBEWHEEL_driver->info.runtime_in_msec = (uint16_t)raw_value;

  raw_value = data[5] << 8; // reserve
  raw_value += data[4];  // reserve

  raw_value = data[6];
  CUBEWHEEL_driver->info.motor_mode = (uint8_t)raw_value;

  raw_value = data[7];
  CUBEWHEEL_driver->info.backup_mode_state = raw_value & 0x01;
  CUBEWHEEL_driver->info.motor_state = (raw_value >> 1) & 0x01;
  CUBEWHEEL_driver->info.hall_sensor_state = (raw_value >> 2) & 0x01;
  CUBEWHEEL_driver->info.encoder_state = (raw_value >> 3) & 0x01;
  CUBEWHEEL_driver->info.error_flag = (raw_value >> 4) & 0x01;

  return i_ret;
}

uint8_t CUBEWHEEL_GetWheelSpeed(CUBEWHEEL_Driver* CUBEWHEEL_driver)
{
  int i_ret = 0;

  uint8_t data[CUBEWHEEL_kTlmLengthGetWheelSpeed_];
  ReadRegister(CUBEWHEEL_driver, CUBEWHEEL_kCmdIdGetWheelSpeed_, data, CUBEWHEEL_kTlmLengthGetWheelSpeed_);

  int raw_value = 0;
  uint8_t s_data[2] = { 0 };

  s_data[0] = data[0];
  s_data[1] = data[1];
  raw_value = raw2signedshort_little(s_data);
  CUBEWHEEL_driver->info.speed_in_rpm = (float)(raw_value / 2);

  return i_ret;
}

uint8_t CUBEWHEEL_GetWheelDuty(CUBEWHEEL_Driver* CUBEWHEEL_driver)
{
  uint8_t data[CUBEWHEEL_kTlmLengthGetWheelDataAdditional_];
  ReadRegister(CUBEWHEEL_driver, CUBEWHEEL_kCmdIdGetWheelDataAdditional_, data, CUBEWHEEL_kTlmLengthGetWheelDataAdditional_);

  int raw_value = 0;
  uint8_t s_data[2] = { 0 };

  s_data[0] = data[0];
  s_data[1] = data[1];
  raw_value = raw2signedshort_little(s_data);
  CUBEWHEEL_driver->info.duty = (float)(raw_value);

  s_data[0] = data[2];
  s_data[1] = data[3];
  raw_value = raw2signedshort_little(s_data);
  CUBEWHEEL_driver->info.speed_backup_in_rpm = (float)(raw_value / 2);

  return 0;
}

uint8_t CUBEWHEEL_GetPwmGain(CUBEWHEEL_Driver* CUBEWHEEL_driver)
{
  int i_ret = 0;
  uint8_t data[CUBEWHEEL_kTlmLengthGetPwmGain_];
  ReadRegister(CUBEWHEEL_driver, CUBEWHEEL_kCmdIdGetPwmGain_, data, CUBEWHEEL_kTlmLengthGetPwmGain_);

  int raw_value = 0;
  uint8_t s_data[2] = { 0 };

  s_data[0] = data[0];
  s_data[1] = data[1];
  raw_value = raw2signedshort_little(s_data);
  CUBEWHEEL_driver->info.gain_pwm = (float)raw_value;


  raw_value = data[2];
  CUBEWHEEL_driver->info.gain_pwm_multiplier = (uint8_t)raw_value;

  return i_ret;
}

uint8_t CUBEWHEEL_GetMainGain(CUBEWHEEL_Driver* CUBEWHEEL_driver)
{
  int i_ret = 0;
  uint8_t data[CUBEWHEEL_kTlmLengthGetMainGain_];
  ReadRegister(CUBEWHEEL_driver, CUBEWHEEL_kCmdIdGetMainGain_, data, CUBEWHEEL_kTlmLengthGetMainGain_);

  uint16_t raw_value = 0;
  raw_value = data[1] << 8;
  raw_value += data[0];
  CUBEWHEEL_driver->info.feedback_gain = raw_value;

  raw_value = data[2];
  CUBEWHEEL_driver->info.feedback_gain_multiplier = (uint8_t)raw_value;

  raw_value = data[4] << 8;
  raw_value += data[3];
  CUBEWHEEL_driver->info.integrator_gain = raw_value;

  raw_value = data[5];
  CUBEWHEEL_driver->info.integrator_gain_multiplier = (uint8_t)raw_value;

  return i_ret;

}

uint8_t CUBEWHEEL_SetBackupmode(CUBEWHEEL_Driver* CUBEWHEEL_driver, uint8_t enable_status)
{
  int i_ret = 0;
  uint8_t data[1] = { 0 };
  data[0] = enable_status & 0xFF;
  WriteRegister(CUBEWHEEL_driver, CUBEWHEEL_kCmdIdSetBackupWheelMode_, data, sizeof(data));

  return i_ret;
}

uint8_t CUBEWHEEL_EnableEncoder(CUBEWHEEL_Driver* CUBEWHEEL_driver, uint8_t enable_status)
{
  int i_ret = 0;
  uint8_t data[1] = { 0 };
  data[0] = enable_status & 0xFF;
  WriteRegister(CUBEWHEEL_driver, CUBEWHEEL_kCmdIdSetEncoderPowerState_, data, sizeof(data));

  return i_ret;
}

uint8_t CUBEWHEEL_EnableHallsensor(CUBEWHEEL_Driver* CUBEWHEEL_driver, uint8_t enable_status)
{
  int i_ret = 0;
  uint8_t data[1] = { 0 };
  data[0] = enable_status & 0xFF;
  WriteRegister(CUBEWHEEL_driver, CUBEWHEEL_kCmdIdSetHallPowerState_, data, sizeof(data));

  return i_ret;
}

static uint8_t GetStartupStatus(CUBEWHEEL_Driver* CUBEWHEEL_driver)
{
  uint8_t i_ret = 0;
  uint8_t data[CUBEWHEEL_kTlmLengthGetIdentification_];
  i_ret = ReadID(CUBEWHEEL_driver, data);
  if (i_ret != 0)
  {
    return i_ret;
  }
  i_ret = ReadExtendedID(CUBEWHEEL_driver, data);
  return i_ret;
}

static void WriteRegister(CUBEWHEEL_Driver* CUBEWHEEL_driver, uint8_t register_address, uint8_t* send_data, uint8_t length)
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
  I2C_tx(&CUBEWHEEL_driver->driver.i2c_slave, &send_data_buffer[0], length + 1);

  return;
}

static void ReadRegister(CUBEWHEEL_Driver* CUBEWHEEL_driver, uint8_t register_address, uint8_t* receive_data, uint8_t length)
{
  // Resister set
  uint8_t send_data_buffer[1] = { 0 };
  send_data_buffer[0] = register_address;
  I2C_tx(&CUBEWHEEL_driver->driver.i2c_slave, &send_data_buffer[0], 1);

  // Read data
  CUBEWHEEL_driver->driver.i2c_slave.rx_length = length;
  I2C_rx(&CUBEWHEEL_driver->driver.i2c_slave, &receive_data[0], length);
  return;
}

static uint8_t ReadID(CUBEWHEEL_Driver* CUBEWHEEL_driver, uint8_t* data)
{
  ReadRegister(CUBEWHEEL_driver, CUBEWHEEL_kCmdIdGetIdentification_, data, CUBEWHEEL_kTlmLengthGetIdentification_);

  // TLM128のdata[0]は8固定。
  // Printf("ReadID = %d\n", data[0]);
  if (data[0] != 0x08)
  {
    return(1);
  }

  return 0;
}


static uint8_t ReadExtendedID(CUBEWHEEL_Driver* CUBEWHEEL_driver, uint8_t* data)
{
  ReadRegister(CUBEWHEEL_driver, CUBEWHEEL_kCmdIdGetExIdentification_, data, CUBEWHEEL_kTlmLengthGetExIdentification_);

  // TLM129のdata[2]はI2Cアドレス
  // Printf("ReadExtendedID = %d : %d", data[2], CUBEWHEEL_driver->driver.i2c_write.device_address);
  if (data[2] != CUBEWHEEL_driver->driver.i2c_write.device_address)
  {
    return(1);
  }

  return 0;
}

uint8_t ReadErrorFlag(CUBEWHEEL_Driver* CUBEWHEEL_driver)
{
  int i_ret = 0;
  uint8_t data[CUBEWHEEL_kTlmLengthGetStatusErrorFlag_];

  ReadRegister(CUBEWHEEL_driver, CUBEWHEEL_kCmdIdGetStatusErrorFlag_, data, CUBEWHEEL_kTlmLengthGetStatusErrorFlag_);


  // Printf("CubeWheel Error flag:");
  int i;
  for (i = 0; i < CUBEWHEEL_kTlmLengthGetStatusErrorFlag_; i++)
  {
    // Printf(data[i], HEX); Printf(" ");
  }
  // Printf("");

  return i_ret;
}

C2A_MATH_ERROR CUBEWHEEL_set_rotation_direction_b(CUBEWHEEL_Driver* CUBEWHEEL_driver,
  const float rotation_direction_b[PHYSICAL_CONST_THREE_DIM])
{
  C2A_MATH_ERROR is_normalized = VECTOR_is_normalized(rotation_direction_b);
  if (is_normalized != C2A_MATH_ERROR_OK) return is_normalized;
  VECTOR_copy(CUBEWHEEL_driver->info.rotation_direction_b,
    rotation_direction_b);
  return C2A_MATH_ERROR_OK;
}

#pragma section
