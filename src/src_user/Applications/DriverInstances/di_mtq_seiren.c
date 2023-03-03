#pragma section REPRO
/**
* @file   di_mtq_seiren.c
* @brief  MTQ_SEIRENのDriver Instance
*/

#include "di_mtq_seiren.h"

#include <src_core/TlmCmd/common_cmd_packet_util.h>
#include <src_core/Library/print.h>
#include "../UserDefined/AOCS/aocs_manager.h"
#include "../../Settings/port_config.h"
#include "../../Library/stdint.h"
#include "../../Library/vector3.h"


static MTQ_SEIREN_Driver mtq_seiren_driver_[MTQ_SEIREN_IDX_MAX];
const  MTQ_SEIREN_Driver* const mtq_seiren_driver[MTQ_SEIREN_IDX_MAX] = {&mtq_seiren_driver_[MTQ_SEIREN_IDX_X],
                                                                         &mtq_seiren_driver_[MTQ_SEIREN_IDX_Y],
                                                                         &mtq_seiren_driver_[MTQ_SEIREN_IDX_Z]};

static void DI_MTQ_SEIREN_init_(void);
static void DI_MTQ_SEIREN_update_(void);

/**
 * @brief  AOCS MANAGERに磁気モーメント方向行列をセットする
 * @return AOCS_MANAGER_ERRORに準拠
 */
static AOCS_MANAGER_ERROR DI_MTQ_SEIREN_set_direction_matrix_to_aocs_manager_(void);

static const float DI_MTQ_SEIREN_kMaxMagMoment_Am2_ = 0.32f; //!< MTQのが出力可能な最大の磁気モーメント

AppInfo DI_MTQ_SEIREN_update(void)
{
  return AI_create_app_info("update_DI_MTQ_SEIREN", DI_MTQ_SEIREN_init_, DI_MTQ_SEIREN_update_);
}


static void DI_MTQ_SEIREN_init_(void)
{
  GPIO_ERR_CODE ret;

  // ある軸のMTQの初期化に失敗しても，別の軸のMTQの初期化はトライしたいので，早期リターンはしない
  ret = MTQ_SEIREN_init(&mtq_seiren_driver_[MTQ_SEIREN_IDX_X], PORT_CH_GPIO_OUT_MTQ_XP, PORT_CH_GPIO_OUT_MTQ_XN, DI_MTQ_SEIREN_kMaxMagMoment_Am2_);
  if (ret != GPIO_OK) Printf("MTQ-X init Failed ! %d \n", ret);
  ret = MTQ_SEIREN_init(&mtq_seiren_driver_[MTQ_SEIREN_IDX_Y], PORT_CH_GPIO_OUT_MTQ_YP, PORT_CH_GPIO_OUT_MTQ_YN, DI_MTQ_SEIREN_kMaxMagMoment_Am2_);
  if (ret != GPIO_OK) Printf("MTQ-Y init Failed ! %d \n", ret);
  ret = MTQ_SEIREN_init(&mtq_seiren_driver_[MTQ_SEIREN_IDX_Z], PORT_CH_GPIO_OUT_MTQ_ZP, PORT_CH_GPIO_OUT_MTQ_ZN, DI_MTQ_SEIREN_kMaxMagMoment_Am2_);
  if (ret != GPIO_OK) Printf("MTQ-Z init Failed ! %d \n", ret);

  // 磁気モーメント方向設定
  float direction_x[] = {-1.0f, 0.0f, 0.0f};
  MTQ_SEIREN_set_magnetic_moment_direction_b(&mtq_seiren_driver_[MTQ_SEIREN_IDX_X], direction_x);
  float direction_y[] = {0.0f, -1.0f, 0.0f};
  MTQ_SEIREN_set_magnetic_moment_direction_b(&mtq_seiren_driver_[MTQ_SEIREN_IDX_Y], direction_y);
  float direction_z[] = {0.0f, 0.0f, -1.0f};
  MTQ_SEIREN_set_magnetic_moment_direction_b(&mtq_seiren_driver_[MTQ_SEIREN_IDX_Z], direction_z);
  AOCS_MANAGER_ERROR ret_manager = DI_MTQ_SEIREN_set_direction_matrix_to_aocs_manager_();
  if (ret_manager != AOCS_MANAGER_ERROR_OK) Printf("MTQ direction setting Failed ! %d \n", ret);
}


static void DI_MTQ_SEIREN_update_(void)
{
  for (int i = 0; i < MTQ_SEIREN_IDX_MAX; i++)
  {
    MTQ_SEIREN_output_pwm(&mtq_seiren_driver_[i]);
  }
}

AOCS_ERROR DI_MTQ_SEIREN_set_pwm_duty(MTQ_SEIREN_IDX axis, int8_t pwm_signed_duty_percent)
{
  MTQ_SEIREN_set_pwm_signed_duty(&mtq_seiren_driver_[(int)axis], pwm_signed_duty_percent);
  return AOCS_ERROR_OK;
}

static AOCS_MANAGER_ERROR DI_MTQ_SEIREN_set_direction_matrix_to_aocs_manager_(void)
{
  float mtq_magnetic_moment_direction_body[AOCS_MANAGER_NUM_OF_MTQ][PHYSICAL_CONST_THREE_DIM];
  for (int idx = 0; idx < AOCS_MANAGER_NUM_OF_MTQ; idx++)
  {
    VECTOR3_copy(mtq_magnetic_moment_direction_body[idx], mtq_seiren_driver_[idx].info.magnetic_moment_direction_b);
  }
  AOCS_MANAGER_ERROR ret = AOCS_MANAGER_set_mtq_magnetic_moment_direction_matrix(mtq_magnetic_moment_direction_body);
  return ret;
}


// コマンド関数
CCP_EXEC_STS Cmd_DI_MTQ_SEIREN_SET_PWM_PERIOD_MS(const CommonCmdPacket* packet)
{
  const uint8_t* param = CCP_get_param_head(packet);

  uint32_t pwm_period_ms;
  endian_memcpy(&pwm_period_ms, param, sizeof(uint32_t));

  DS_CMD_ERR_CODE ret;
  ret = MTQ_SEIREN_set_pwm_period_ms(pwm_period_ms);

  return DS_conv_cmd_err_to_ccp_exec_sts(ret);
}

CCP_EXEC_STS Cmd_DI_MTQ_SEIREN_SET_PWM_DUTY(const CommonCmdPacket* packet)
{
  const uint8_t* param = CCP_get_param_head(packet);

  MTQ_SEIREN_IDX axis;
  int8_t pwm_signed_duty_percent;

  axis = (MTQ_SEIREN_IDX)param[0];
  if (axis >= MTQ_SEIREN_IDX_MAX) return CCP_EXEC_ILLEGAL_PARAMETER;

  endian_memcpy(&pwm_signed_duty_percent, param + sizeof(uint8_t), sizeof(int8_t));
  if (pwm_signed_duty_percent < -100 || pwm_signed_duty_percent > 100) return CCP_EXEC_ILLEGAL_PARAMETER;

  DI_MTQ_SEIREN_set_pwm_duty(axis, pwm_signed_duty_percent);
  return CCP_EXEC_SUCCESS;
}

CCP_EXEC_STS Cmd_DI_MTQ_SEIREN_SET_MAGNETIC_MOMENT_DIRECTION_VECTOR(const CommonCmdPacket* packet)
{
  uint8_t arg_position = 0;

  MTQ_SEIREN_IDX idx = (MTQ_SEIREN_IDX)CCP_get_param_from_packet(packet, arg_position, uint8_t);
  arg_position++;

  if (idx >= MTQ_SEIREN_IDX_MAX) return CCP_EXEC_ILLEGAL_PARAMETER;

  float magnetic_moment_direction_b[PHYSICAL_CONST_THREE_DIM];
  for (int body_axis = 0; body_axis < PHYSICAL_CONST_THREE_DIM; body_axis++)
  {
    magnetic_moment_direction_b[body_axis] = (float)CCP_get_param_from_packet(packet, arg_position, float);
    arg_position++;
  }

  C2A_MATH_ERROR ret_math = MTQ_SEIREN_set_magnetic_moment_direction_b(&mtq_seiren_driver_[idx], magnetic_moment_direction_b);
  if (ret_math != C2A_MATH_ERROR_OK) return CCP_EXEC_ILLEGAL_PARAMETER;

  AOCS_MANAGER_ERROR ret_manager = DI_MTQ_SEIREN_set_direction_matrix_to_aocs_manager_();
  if (ret_manager != AOCS_MANAGER_ERROR_OK) return CCP_EXEC_ILLEGAL_CONTEXT;

  return CCP_EXEC_SUCCESS;
}

#pragma section
