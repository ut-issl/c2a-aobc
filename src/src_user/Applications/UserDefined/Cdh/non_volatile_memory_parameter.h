/**
* @file   non_volatile_memory_parameter.h
* @brief  不揮発メモリパラメータの定義
*/
#ifndef NON_VOLATILE_MEMORY_PARAMETER_H_
#define NON_VOLATILE_MEMORY_PARAMETER_H_

#include "non_volatile_memory_partition.h"
#include "../../../Settings/Applications/NvmParams/non_volatile_memory_normal_parameters.h"
#include "../../../Settings/Applications/NvmParams/non_volatile_memory_redundant_parameters.h"
#include "../../../Library/stdint.h"

/**
 * @enum  APP_NVM_PARAM_TYPE
 * @brief 不揮発メモリ管理で使うデータ型
 * @note  型はuint8_tを想定
 */
typedef enum
{
  APP_NVM_PARAM_TYPE_UINT8 = 0,
  APP_NVM_PARAM_TYPE_UINT16,
  APP_NVM_PARAM_TYPE_UINT32,
  APP_NVM_PARAM_TYPE_INT8,
  APP_NVM_PARAM_TYPE_INT16,
  APP_NVM_PARAM_TYPE_INT32,
  APP_NVM_PARAM_TYPE_FLOAT,
  APP_NVM_PARAM_TYPE_DOUBLE,
  APP_NVM_PARAM_TYPE_MAX,
} APP_NVM_PARAM_TYPE;

/**
 * @struct APP_NVM_PARAM_Element
 * @brief  不揮発メモリ管理上の基本要素
 */
typedef struct
{
  APP_NVM_PARAM_TYPE data_type;  //!< データの型
  uint8_t  registered_flag;      //!< 登録フラグ(0=未登録、1=登録), 未登録の場所にはアクセスできない
  uint32_t start_address;        //!< パーティション内での先頭アドレス
  uint8_t  length;               //!< 要素数
} APP_NVM_PARAM_Element;

/**
 * @struct NonVolatileMemoryParameter
 * @brief  不揮発メモリパラメータ管理構造体
 */
typedef struct
{
  APP_NVM_PARAM_Element normal_params[NVM_NORMAL_PARAMS_MAX];        //!< 冗長など無いノーマルパラメータ
  APP_NVM_PARAM_Element redundant_params[NVM_REDUNDANT_PARAMS_MAX];  //!< 三重冗長で管理するパラメータ
} NonVolatileMemoryParameter;

extern const NonVolatileMemoryParameter* const non_volatile_memory_parameter;

AppInfo APP_NVM_PARAMETER_create_app(void);

// 外部公開関数
/**
 * @brief  不揮発メモリノーマルパラメータの登録
 * @param[in]  param_id      : パラメータID
 * @param[in]  data_type     : 保存するデータの型
 * @param[in]  start_address : ノーマルパーティション内での書き込み先頭アドレス
 * @param[in]  length        : 保存するデータの配列長(通常変数の場合は1)
 * @return                   : APP_NVM_MANAGER_ERRORに従う
 */
APP_NVM_MANAGER_ERROR APP_NVM_PARAM_register_normal_param(const NVM_NORMAL_PARAMS param_id,
                                                          const APP_NVM_PARAM_TYPE data_type,
                                                          const uint32_t start_address,
                                                          const uint8_t length);
/**
 * @brief  不揮発メモリノーマルパラメータの登録解除
 * @param[in]  param_id      : パラメータID
 * @return                   : APP_NVM_MANAGER_ERRORに従う
 * @note 登録解除するとそのパラメータにはアクセスできなくなる
 */
APP_NVM_MANAGER_ERROR APP_NVM_PARAM_clear_normal_param(const NVM_NORMAL_PARAMS param_id);
/**
 * @brief  不揮発メモリノーマルパラメータの登録解除
 * @param[in]  param_id      : パラメータID
 * @param[in]  write_data    : 書き込みデータポインタ
 * @return                   : APP_NVM_MANAGER_ERRORに従う
 */
APP_NVM_MANAGER_ERROR APP_NVM_PARAM_write_normal_param(const NVM_NORMAL_PARAMS param_id, const void* write_data);
/**
 * @brief  不揮発メモリノーマルパラメータの登録解除
 * @param[out] read_data     : 読み出しデータポインタ(事前にメモリ確保しておく必要あり)
 * @param[in]  param_id      : パラメータID
 * @return                   : APP_NVM_MANAGER_ERRORに従う
 */
APP_NVM_MANAGER_ERROR APP_NVM_PARAM_read_normal_param (void* read_data, const NVM_NORMAL_PARAMS param_id);


/**
 * @brief  不揮発メモリ三重冗長パラメータの登録
 * @param[in]  param_id      : パラメータID
 * @param[in]  data_type     : 保存するデータの型
 * @param[in]  start_address : 三重冗長パーティション内での書き込み先頭アドレス
 * @param[in]  length        : 保存するデータの配列長(通常変数の場合は1)
 * @return                   : APP_NVM_MANAGER_ERRORに従う
 */
APP_NVM_MANAGER_ERROR APP_NVM_PARAM_register_redundant_param(const NVM_REDUNDANT_PARAMS param_id,
                                                             const APP_NVM_PARAM_TYPE data_type,
                                                             const uint32_t start_address,
                                                             const uint8_t length);
/**
 * @brief  不揮発メモリ三重冗長パラメータの登録解除
 * @param[in]  param_id      : パラメータID
 * @return                   : APP_NVM_MANAGER_ERRORに従う
 * @note 登録解除するとそのパラメータにはアクセスできなくなる
 */
APP_NVM_MANAGER_ERROR APP_NVM_PARAM_clear_redundant_param(const NVM_REDUNDANT_PARAMS param_id);
/**
 * @brief  不揮発メモリ三重冗長パラメータの登録解除
 * @param[in]  param_id      : パラメータID
 * @param[in]  write_data    : 書き込みデータポインタ
 * @return                   : APP_NVM_MANAGER_ERRORに従う
 */
APP_NVM_MANAGER_ERROR APP_NVM_PARAM_write_redundant_param(const NVM_REDUNDANT_PARAMS param_id, const void* write_data);
/**
 * @brief  不揮発メモリ三重冗長パラメータの登録解除
 * @param[out] read_data     : 読み出しデータポインタ(事前にメモリ確保しておく必要あり)
 * @param[in]  param_id      : パラメータID
 * @return                   : APP_NVM_MANAGER_ERRORに従う
 */
APP_NVM_MANAGER_ERROR APP_NVM_PARAM_read_redundant_param (void* read_data, const NVM_REDUNDANT_PARAMS param_id);

#endif
