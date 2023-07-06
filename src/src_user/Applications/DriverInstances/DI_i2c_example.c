
// core side
#include <src_core/IfWrapper/i2c.h>
#include <src_core/Library/print.h>

// user side
#include "DI_i2c_example.h"
#include <src_user/Settings/port_config.h>

static I2C_Config I2C_EXAMPLE_port_;

static void I2C_EXAMPLE_init_(void);
static void I2C_EXAMPLE_exec_(void);

AppInfo I2C_EXAMPLE_update(void)
{
  return AI_create_app_info("I2C_EXAMPLE", I2C_EXAMPLE_init_, I2C_EXAMPLE_exec_);
}

static void I2C_EXAMPLE_init_(void)
{
  int i_ret = 0;
  I2C_EXAMPLE_port_.ch = PORT_CH_I2C_SENSORS;
  I2C_EXAMPLE_port_.frequency_khz = 100;
  I2C_EXAMPLE_port_.device_address = I2C_DEVICE_ADDR_AOBC_RM;
  I2C_EXAMPLE_port_.stop_flag = 1;
  I2C_EXAMPLE_port_.timeout_threshold = 500;

  if (I2C_init(&I2C_EXAMPLE_port_) != 0)
  {
    Printf("Error: I2C_init in i2c_example.\n");
  }

  // Initialize
  unsigned char init_wdata[2] = { 0x01, 0x7d };
  I2C_tx(&I2C_EXAMPLE_port_, init_wdata, 2);

  return;
}

static void I2C_EXAMPLE_exec_(void)
{
  // Read data
  unsigned char reg_addr = 0x24;
  I2C_tx(&I2C_EXAMPLE_port_, &reg_addr, 1);
  unsigned char mag_c_raw[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
  I2C_rx(&I2C_EXAMPLE_port_, mag_c_raw, 9);

  // Printf("I2C EXP: %x,%x,%x,%x,%x,%x,%x,%x,%x\n",mag_c_raw[0], mag_c_raw[1], mag_c_raw[2], mag_c_raw[3], mag_c_raw[4], mag_c_raw[5], mag_c_raw[6], mag_c_raw[7], mag_c_raw[8]);

  return;
}
