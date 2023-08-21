#ifndef USER_DEFINED_I2C_ADDRESS_H_
#define USER_DEFINED_I2C_ADDRESS_H_

// Undefined
#undef I2C_DEVICE_ADDR_AOBC_MPU
#undef I2C_DEVICE_ADDR_AOBC_MPU_AK
#undef I2C_DEVICE_ADDR_AOBC_RM
#undef I2C_DEVICE_ADDR_EXT_RM
#undef I2C_DEVICE_ADDR_SS_IDX_0
#undef I2C_DEVICE_ADDR_SS_IDX_1
#undef I2C_DEVICE_ADDR_SS_IDX_2
#undef I2C_DEVICE_ADDR_SS_IDX_3
#undef I2C_DEVICE_ADDR_SS_IDX_4
#undef I2C_DEVICE_ADDR_SS_IDX_5
#undef I2C_DEVICE_ADDR_RW_X
#undef I2C_DEVICE_ADDR_RW_Y
#undef I2C_DEVICE_ADDR_RW_Z

// Sensors
#define I2C_DEVICE_ADDR_AOBC_MPU     (0x68)  //!< MPU9250 on AOBC board
#define I2C_DEVICE_ADDR_AOBC_MPU_AK  (0x0c)  //!< AK8963 magnetometer in MPU9250
#define I2C_DEVICE_ADDR_AOBC_RM      (0x20)  //!< RM3100 magnetometer on AOBC board
#define I2C_DEVICE_ADDR_EXT_RM       (0x23)  //!< External RM3100 magnetometer
#define I2C_DEVICE_ADDR_SS_IDX_0     (0x60)  //!< nanoSSOC-D60 Sun-sensor IDX 0
#define I2C_DEVICE_ADDR_SS_IDX_1     (0x61)  //!< nanoSSOC-D60 Sun-sensor IDX 1
#define I2C_DEVICE_ADDR_SS_IDX_2     (0x62)  //!< nanoSSOC-D60 Sun-sensor IDX 2
#define I2C_DEVICE_ADDR_SS_IDX_3     (0x63)  //!< nanoSSOC-D60 Sun-sensor IDX 3
#define I2C_DEVICE_ADDR_SS_IDX_4     (0x64)  //!< nanoSSOC-D60 Sun-sensor IDX 4
#define I2C_DEVICE_ADDR_SS_IDX_5     (0x65)  //!< nanoSSOC-D60 Sun-sensor IDX 5

// RWs
#define I2C_DEVICE_ADDR_RW_X         (0x11)  //!< RW on X-axis
#define I2C_DEVICE_ADDR_RW_Y         (0x12)  //!< RW on Y-axis
#define I2C_DEVICE_ADDR_RW_Z         (0x13)  //!< RW on Z-axis

#endif // USER_DEFINED_I2C_ADDRESS_H_
