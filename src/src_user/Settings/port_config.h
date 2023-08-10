#ifndef PORT_CONFIG_H_
#define PORT_CONFIG_H_

// ============================================ //
//             UART関連のポート設定             //
// ============================================ //
#define  PORT_CH_UART_DEBUG     (0)  //!< Serial 0
// #define  PORT_CH_UART_NAN    (1)  //!< Serial 1 今回はここは使えない
#define  PORT_CH_UART_OEM       (2)  //!< Serial 2
#define  PORT_CH_UART_MOBC      (3)  //!< Serial 3
#define  PORT_CH_UART_STIM      (4)  //!< Serial 4
#define  PORT_CH_UART_SAGITTA   (5)  //!< Serial 5
#define  PORT_CH_MAX_UART_PORT  (6)  //!< UART max port for Arduino/chipKIT


// ============================================ //
//             GPIO関連のポート設定             //
//             PICピン番号で定義する            //
// ============================================ //
// 電源スイッチ
// 5V系統
#define  PORT_CH_GPIO_OUT_POWER_INA     (38)  //!< INA電流センサの電源スイッチ（負論理）
#define  PORT_CH_GPIO_OUT_POWER_MPU     (60)  //!< MPU9250 9軸センサの電源スイッチ
#define  PORT_CH_GPIO_OUT_POWER_RM      (61)  //!< RM3100精磁気センサの電源スイッチ
#define  PORT_CH_GPIO_OUT_POWER_SS      (73)  //!< サンセンサの電源スイッチ
#define  PORT_CH_GPIO_OUT_POWER_MTQ     (91)  //!< MTQの電源スイッチ
// 12V系統
#define  PORT_CH_GPIO_OUT_POWER_STIM    ( 6)  //!< STIM210精ジャイロセンサの電源スイッチ
#define  PORT_CH_GPIO_OUT_POWER_SAGITTA ( 7)  //!< STTの電源スイッチ
#define  PORT_CH_GPIO_OUT_POWER_OEM     ( 8)  //!< GSP受信機の電源スイッチ
#define  PORT_CH_GPIO_OUT_POWER_RWX     (71)  //!< X軸RWの電源スイッチ
#define  PORT_CH_GPIO_OUT_POWER_RWY     (79)  //!< Y軸RWの電源スイッチ
#define  PORT_CH_GPIO_OUT_POWER_RWZ     (80)  //!< Z軸RWの電源スイッチ

// MTQ操作
#define  PORT_CH_GPIO_OUT_MTQ_XP      (78)  //!< X軸MTQプラス方向
#define  PORT_CH_GPIO_OUT_MTQ_XN      (81)  //!< X軸MTQマイナス方向
#define  PORT_CH_GPIO_OUT_MTQ_YP      (82)  //!< Y軸MTQプラス方向
#define  PORT_CH_GPIO_OUT_MTQ_YN      (83)  //!< Y軸MTQマイナス方向
#define  PORT_CH_GPIO_OUT_MTQ_ZP      (84)  //!< Z軸MTQプラス方向
#define  PORT_CH_GPIO_OUT_MTQ_ZN      (68)  //!< Z軸MTQマイナス方向

// FRAM チップセレクト
#define  PORT_CH_GPIO_OUT_FRAM_CS1    (69)  //!< FRAM #1のチップセレクトポート
#define  PORT_CH_GPIO_OUT_FRAM_CS2    (96)  //!< FRAM #2のチップセレクトポート
#define  PORT_CH_GPIO_OUT_FRAM_CS3    (97)  //!< FRAM #3のチップセレクトポート
#define  PORT_CH_GPIO_OUT_FRAM_CS4    (95)  //!< FRAM #4のチップセレクトポート

// その他
#define  PORT_CH_GPIO_OUT_SELF_RST    (17)  //!< PIC自己リセット
#define  PORT_CH_GPIO_OUT_OEM_RST     (74)  //!< GPS-Rリセット（負論理）
#define  PORT_CH_GPIO_OUT_STIM_RST    (77)  //!< STIM210リセット（負論理）
#define  PORT_CH_GPIO_OUT_STIM_TRIG   (76)  //!< STIM210 External Triger
#define  PORT_CH_GPIO_OUT_SAGITTA_PPS (88)  //!< STTへのPPS信号バイパス
#define  PORT_CH_GPIO_IN_REFV         (20)  //!< UNREG+Vが供給されているかどうか判断できるよう分圧した電圧

// 入力
#define  PORT_CH_GPIO_IN_MOBC_1       (18)  //!< MOBC 1 // TODO_L 適切な名前を考える
#define  PORT_CH_GPIO_IN_MOBC_2       (87)  //!< MOBC 2 // TODO_L 適切な名前を考える
#define  PORT_CH_GPIO_IN_OEM_PPS      (19)  //!< GPS-R PPS信号入力
#define  PORT_CH_GPIO_IN_MTQ_FAULT    (92)  //!< MTQ異常フラグ（負論理）

// 制限
#define  PORT_CH_MAX_GPIO_PORT        (101) //!< UART max port for Arduino/chipKIT

// ============================================ //
//             ADC関連のポート設定              //
// ============================================ //
#define  PORT_CH_MIN_ADC_PORT          (1)  //!< ADC min port
#define  PORT_CH_ADC_REFERENCE         (2)  //!< ADCリファレンス電圧
#define  PORT_CH_ADC_TEMPERATURE_PIC   (3)  //!< AOBC基板PIC付近の温度計
#define  PORT_CH_ADC_TEMPERATURE_DCDC  (4)  //!< AOBC基板DCDC付近の温度計
#define  PORT_CH_MAX_ADC_PORT          (5)  //!< ADC max port


// ============================================ //
//             I2C関連のポート設定              //
// ============================================ //
#define  PORT_CH_I2C_SENSORS  (0)  //!< I2C DTWI
#define  PORT_CH_I2C_RWS      (1)  //!< I2C DTWI1
// #define  PORT_CH_I2C_NAN   (2)  //!< I2C DTWI2 今回はここは使えない
// #define  PORT_CH_I2C_NAN   (3)  //!< I2C DTWI3 今回はここは使えない
#define  PORT_CH_I2C_INAS     (4)  //!< I2C DTWI4
#define  PORT_CH_MAX_I2C_PORT (5)  //!< I2C max port for Arduino/chipKIT


// ============================================ //
//             SPI関連のポート設定              //
// ============================================ //
#define  PORT_CH_SPI_FRAM     (0)  //!< SPI DSPI0
// #define  PORT_CH_SPI_NAN   (1)  //!< SPI DSPI1 今回はここは使えない
// #define  PORT_CH_SPI_NAN   (2)  //!< SPI DSPI2 今回はここは使えない
// #define  PORT_CH_SPI_NAN   (3)  //!< SPI DSPI3 今回はここは使えない
#define  PORT_CH_MAX_SPI_PORT (4)  //!< SPI max port for Arduino/chipKIT


// ============================================ //
//         I2Cデバイスアドレス設定              //
// ============================================ //
// PORT_CH_I2C_SENSORS
#define I2C_DEVICE_ADDR_AOBC_MPU     (0x68)  //!< AOBC基板上のMPU9250 9軸センサ
#define I2C_DEVICE_ADDR_AOBC_MPU_AK  (0x0c)  //!< AOBC基板上のMPU9250内のAK8963磁気センサ
#define I2C_DEVICE_ADDR_AOBC_RM      (0x20)  //!< AOBC基板上のRM3100精磁気センサ
#define I2C_DEVICE_ADDR_EXT_RM       (0x23)  //!< 外部基板上のRM3100精磁気センサ
#define I2C_DEVICE_ADDR_SS_IDX_0     (0x60)  //!< nanoSSOC-D60サンセンサ IDX 0
#define I2C_DEVICE_ADDR_SS_IDX_1     (0x61)  //!< nanoSSOC-D60サンセンサ IDX 1
#define I2C_DEVICE_ADDR_SS_IDX_2     (0x62)  //!< nanoSSOC-D60サンセンサ IDX 2
#define I2C_DEVICE_ADDR_SS_IDX_3     (0x63)  //!< nanoSSOC-D60サンセンサ IDX 3
#define I2C_DEVICE_ADDR_SS_IDX_4     (0x64)  //!< nanoSSOC-D60サンセンサ IDX 4
#define I2C_DEVICE_ADDR_SS_IDX_5     (0x65)  //!< nanoSSOC-D60サンセンサ IDX 5

// PORT_CH_I2C_RWS
#define I2C_DEVICE_ADDR_RW_X         (0x11)  //!< X軸RW-FM
#define I2C_DEVICE_ADDR_RW_Y         (0x12)  //!< Y軸RW-FM
#define I2C_DEVICE_ADDR_RW_Z         (0x13)  //!< Z軸RW-FM
#define I2C_DEVICE_ADDR_RW_EM        (0x35)  //!< RW-EM 試験時にのみ使う

// PORT_CH_I2C_INAS
#define I2C_DEVICE_ADDR_INA_PIC      (0x40)  //!< PIC用のINA260電流センサ
#define I2C_DEVICE_ADDR_INA_STIM     (0x41)  //!< STIM210用のINA260電流センサ
#define I2C_DEVICE_ADDR_INA_SAGITTA  (0x42)  //!< STT用のINA260電流センサ
#define I2C_DEVICE_ADDR_INA_OEM      (0x43)  //!< GPS-R用のINA260電流センサ
#define I2C_DEVICE_ADDR_INA_RM       (0x44)  //!< RM3100用のINA260電流センサ
#define I2C_DEVICE_ADDR_INA_SS       (0x45)  //!< SS用のINA260電流センサ
#define I2C_DEVICE_ADDR_INA_MTQ      (0x46)  //!< MTQ用のINA260電流センサ
#define I2C_DEVICE_ADDR_INA_RWX      (0x47)  //!< RW_X軸用のINA260電流センサ
#define I2C_DEVICE_ADDR_INA_RWY      (0x48)  //!< RW_Y軸用のINA260電流センサ
#define I2C_DEVICE_ADDR_INA_RWZ      (0x49)  //!< RW_Z軸用のINA260電流センサ

// Use user defined I2C address
#ifdef USER_DEFINED_I2C_ADDRESS
#include <Settings/user_defined_i2c_address.h>
#endif

#endif
