/**
* @file    sun_vecor_propagator.h
* @brief   太陽方向ベクトル伝搬アプリ
*
*          最後にサンセンサによって太陽方向が取れた時の姿勢を基準姿勢とする．基準姿勢から今の姿勢への変換クオータニオンを計算し，それを使って今の太陽方向ベクトルを計算する．
*/
#ifndef SUN_VECTOR_PROPAGATOR_H_
#define SUN_VECTOR_PROPAGATOR_H_

#include <stdint.h>
#include <src_core/System/ApplicationManager/app_info.h>
#include  <src_core/System/TimeManager/time_manager.h>
#include "../../../../Library/quaternion.h"
#include "../../../../Library/physical_constants.h"
#include "../../AOCS/aocs_error.h"

/**
 * @struct SunVectorPropagator
 * @brief  太陽方向ベクトル伝搬に必要な情報を格納する構造体
 */
typedef struct
{
  float last_seen_sun_vec_body[PHYSICAL_CONST_THREE_DIM]; //!< 最後にサンセンサセレクタが取得できた，body座標系での太陽方向ベクトル
  Quaternion quaternion_sun_ref_to_body;                  //!< 最後に太陽が見えたときのbody座標系から，現在のbody座標系に変換するquaternion
  ObcTime previous_obc_time;                              //!< 前回太陽方向ベクトル伝搬アプリが呼び出された時の時刻情報
} SunVectorPropagator;

extern const SunVectorPropagator* const sun_vector_propagator;

/**
 * @brief  太陽方向ベクトル伝搬定期実行アプリ生成関数
 * @param  void
 * @return AppInfo
 */
AppInfo APP_SUN_VEC_PROPAGATOR_create_app(void);

#endif
