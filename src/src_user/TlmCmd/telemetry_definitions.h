/**
 * @file
 * @brief  テレメトリ定義
 * @note   このコードは自動生成されています！
 */
#ifndef TELEMETRY_DEFINITIONS_H_
#define TELEMETRY_DEFINITIONS_H_

typedef enum
{
  Tlm_CODE_AOBC_OBC = 0x00,
  Tlm_CODE_AOBC_TL = 0x01,
  Tlm_CODE_AOBC_BL = 0x02,
  Tlm_CODE_AOBC_CA = 0x03,
  Tlm_CODE_AOBC_TF = 0x04,
  Tlm_CODE_AOBC_DCU = 0x05,
  Tlm_CODE_AOBC_MM = 0x07,
  Tlm_CODE_AOBC_AM = 0x08,
  Tlm_CODE_AOBC_APP_TIME = 0x09,
  Tlm_CODE_AOBC_EL = 0x0a,
  Tlm_CODE_AOBC_EL_TLOG = 0x0b,
  Tlm_CODE_AOBC_EL_CLOG = 0x0c,
  Tlm_CODE_AOBC_EH = 0x0d,
  Tlm_CODE_AOBC_EH_RULE = 0x0e,
  Tlm_CODE_AOBC_EH_LOG = 0x0f,
  Tlm_CODE_AOBC_EH_INDEX = 0x10,
  Tlm_CODE_AOBC_GIT_REV = 0x11,
  Tlm_CODE_AOBC_HK_GEN = 0x20,
  Tlm_CODE_AOBC_HK_COMPO = 0x21,
  Tlm_CODE_AOBC_HK_ALGO = 0x22,
  Tlm_CODE_AOBC_COMPONENTS = 0x23,
  Tlm_CODE_AOBC_POWER = 0x24,
  Tlm_CODE_AOBC_FRAME_TRANSFORMATION = 0x25,
  Tlm_CODE_AOBC_CONTROL = 0x26,
  Tlm_CODE_AOBC_BIAS = 0x27,
  Tlm_CODE_AOBC_TEMP_CALIB = 0x28,
  Tlm_CODE_AOBC_INTERPOLATION_TARGET1 = 0x29,
  Tlm_CODE_AOBC_INTERPOLATION_TARGET2 = 0x2a,
  Tlm_CODE_AOBC_NON_VOLATILE = 0x2b,
  Tlm_CODE_AOBC_ORBIT = 0x2c,
  Tlm_CODE_AOBC_AOCS_MANAGER = 0x2d,
  Tlm_CODE_AOBC_SUN_POINTING = 0x2e,
  Tlm_CODE_AOBC_THREE_AXIS_MTQ = 0x2f,
  Tlm_CODE_AOBC_THREE_AXIS_RW = 0x30,
  Tlm_CODE_AOBC_FILTERS = 0x31,
  Tlm_CODE_AOBC_ANOMALY = 0x32,
  Tlm_CODE_AOBC_APP_TIME_2 = 0x33,
  Tlm_CODE_AOBC_SAGITTA_SET1 = 0x34,
  Tlm_CODE_AOBC_SAGITTA_SET2 = 0x35,
  Tlm_CODE_AOBC_SAGITTA_READ1 = 0x36,
  Tlm_CODE_AOBC_SAGITTA_READ2 = 0x37,
  Tlm_CODE_AOBC_STT_GYRO_EKF1 = 0x38,
  Tlm_CODE_AOBC_STT_GYRO_EKF2 = 0x39,
  Tlm_CODE_AOBC_GPSR_RANGE_P1 = 0x3a,
  Tlm_CODE_AOBC_GPSR_RANGE_P2 = 0x3b,
  Tlm_CODE_AOBC_ORBIT2 = 0x3c,
  Tlm_CODE_AOBC_FILTERS_2 = 0x3d,
  Tlm_CODE_AOBC_DR_ALGORITHM = 0x3e,
  Tlm_CODE_AOBC_RW_DETAILED = 0x3f,
  Tlm_CODE_AOBC_SAGITTA1 = 0x40,
  Tlm_CODE_AOBC_SAGITTA2 = 0x41,
  Tlm_CODE_AOBC_SAGITTA3 = 0x42,
  Tlm_CODE_AOBC_SAGITTA4 = 0x43,
  Tlm_CODE_AOBC_SAGITTA5 = 0x44,

  TLM_CODE_MAX
} TLM_CODE;

#endif
