#pragma section REPRO
/**
* @file   rw_controller.h
* @brief  aocs_managerの目標トルク情報から，RWの出力トルクを計算・設定するアプリ
*/

#include "rw_controller.h"

#include "../../../../../Library/vector3.h"
#include "../../../../../Library/matrix33.h"
#include "../../../../../Library/physical_constants.h"
#include "../../../../../Library/math_constants.h"

#include <src_core/System/EventManager/event_logger.h>
#include "../../../../app_registry.h"

static void APP_RW_CONTROLLER_init_(void);
static void APP_RW_CONTROLLER_exec_(void);

AppInfo APP_RW_CONTROLLER_create_app(void)
{
  return AI_create_app_info("RwController", APP_RW_CONTROLLER_init_, APP_RW_CONTROLLER_exec_);
}

void APP_RW_CONTROLLER_init_(void)
{
}

void APP_RW_CONTROLLER_exec_(void)
{
  float torque_rw_Nm[RW0003_IDX_MAX];
  // TODO_L: 4つ以上のRWを搭載することになったら4つ以上のRWに対応できるようにする
  MATRIX33_multiply_matrix_vector(torque_rw_Nm, aocs_manager->rw_distribution_matrix,
                                aocs_manager->internal_torque_target_body_Nm);
  VECTOR3_scalar_product(torque_rw_Nm, -1.0f, torque_rw_Nm); // RW司令は逆方向

  for (int idx_rw = 0; idx_rw < RW0003_IDX_MAX; idx_rw++)
  {
    AOCS_ERROR ret = DI_RW0003_set_torque_Nm((RW0003_IDX)idx_rw, torque_rw_Nm[idx_rw]);  // 実機確認で問題ないことを確認したが要注意
    if (ret != AOCS_ERROR_OK)
    {
      EL_GROUP el_group_error = (EL_GROUP)(EL_GROUP_ERROR_RW0003X + idx_rw);
      EL_record_event(el_group_error, (uint32_t)idx_rw, EL_ERROR_LEVEL_LOW, (uint32_t)RW0003_EL_NOTE_TORQUE_CMD);
    }
  }
}

#pragma section
