#pragma section REPRO
/**
* @file
* @brief  FM25V10のDriver Instance
*/

#include "di_fm25v10.h"

#include <src_core/Library/print.h>
#include <src_core/Library/endian.h>
#include <src_core/TlmCmd/common_cmd_packet_util.h>
#include <src_user/Settings/port_config.h>
#include <src_user/Settings/DriverSuper/driver_buffer_define.h>
#include <math.h>

#define DS_STREAM_REC_BUFFER_SIZE_FM25V10 (DS_IF_RX_BUFFER_SIZE_FM25V10)  //!< DS_StreamRecBuffer のバッファサイズ

static void DI_FM25V10_init_(void);
static void DI_FM25V10_update_(void);

static FM25V10_Driver fm25v10_driver_[FM25V10_IDX_MAX];
const  FM25V10_Driver* const fm25v10_driver[FM25V10_IDX_MAX] = {&fm25v10_driver_[FM25V10_IDX_1],
                                                                &fm25v10_driver_[FM25V10_IDX_2],
                                                                &fm25v10_driver_[FM25V10_IDX_3],
                                                                &fm25v10_driver_[FM25V10_IDX_4]};
// バッファ
static DS_StreamRecBuffer DI_FM25V10_rx_buffer_[FM25V10_IDX_MAX];
static uint8_t DI_FM25V10_rx_buffer_allocation_[FM25V10_IDX_MAX][DS_STREAM_REC_BUFFER_SIZE_FM25V10];


/**
 * @brief  先頭globalアドレスからデバイス毎のlocalアドレスとどのデバイスに書き込むかを求める
 * @param[out] idx                  : 対象となるデバイスID
 * @param[out] local_start_address  : 対象となるデバイスでの先頭アドレス
 * @param[in]  global_start_address : 複数のFRAMをまとめた先頭アドレス
 * @return                          : FM25V10_ERRORに準拠
 */
static DI_FM25V10_ERROR DI_FM25V10_calc_local_address_(FM25V10_IDX* idx,
                                                       uint32_t* local_start_address,
                                                       const uint32_t global_start_address);

/**
 * @brief  デバイスをまたいだメモリアクセスが必要な場合、そのオーバーラン長を計算して返す関数
 * @param[in] local_start_address   : デバイスでの先頭アドレス
 * @param[in] length                : 書き込みデータ長
 * @return                          : オーバーランデータ長(0ならオーバーランなし)
 */
static uint32_t DI_FM25V10_calc_device_overrun_length_(const uint32_t local_start_address,
                                                       const uint32_t length);

AppInfo DI_FM25V10_update(void)
{
  return AI_create_app_info("update_DI_FM25V10", DI_FM25V10_init_, DI_FM25V10_update_);
}

static void DI_FM25V10_init_(void)
{
  int i;
  DS_ERR_CODE ret1;
  DS_INIT_ERR_CODE ret2;

  for (i = 0; i < FM25V10_IDX_MAX; ++i)
  {
    ret1 = DS_init_stream_rec_buffer(&DI_FM25V10_rx_buffer_[i],
                                     DI_FM25V10_rx_buffer_allocation_[i],
                                     sizeof(DI_FM25V10_rx_buffer_allocation_[i]));
    if (ret1 != DS_ERR_CODE_OK)
    {
      Printf("FM25V10 buffer #%d init Failed ! %d \n", i, ret1);
    }
  }

  ret2 = FM25V10_init(&fm25v10_driver_[FM25V10_IDX_1], PORT_CH_SPI_FRAM, PORT_CH_GPIO_OUT_FRAM_CS1, &DI_FM25V10_rx_buffer_[FM25V10_IDX_1]);
  if (ret2 != DS_INIT_OK)  Printf("FM25V10 #1 init Failed ! %d \n", ret2);
  ret2 = FM25V10_init(&fm25v10_driver_[FM25V10_IDX_2], PORT_CH_SPI_FRAM, PORT_CH_GPIO_OUT_FRAM_CS2, &DI_FM25V10_rx_buffer_[FM25V10_IDX_2]);
  if (ret2 != DS_INIT_OK)  Printf("FM25V10 #2 init Failed ! %d \n", ret2);
  ret2 = FM25V10_init(&fm25v10_driver_[FM25V10_IDX_3], PORT_CH_SPI_FRAM, PORT_CH_GPIO_OUT_FRAM_CS3, &DI_FM25V10_rx_buffer_[FM25V10_IDX_3]);
  if (ret2 != DS_INIT_OK)  Printf("FM25V10 #3 init Failed ! %d \n", ret2);
  ret2 = FM25V10_init(&fm25v10_driver_[FM25V10_IDX_4], PORT_CH_SPI_FRAM, PORT_CH_GPIO_OUT_FRAM_CS4, &DI_FM25V10_rx_buffer_[FM25V10_IDX_4]);
  if (ret2 != DS_INIT_OK)  Printf("FM25V10 #4 init Failed ! %d \n", ret2);
}

static void DI_FM25V10_update_(void)
{
  // とりあえずアップデートはなし
  return;
}

CCP_CmdRet Cmd_DI_FM25V10_WRITE_BYTE(const CommonCmdPacket* packet)
{
  const uint8_t* param = CCP_get_param_head(packet);

  FM25V10_IDX idx;
  idx = (FM25V10_IDX)param[0];
  if (idx >= FM25V10_IDX_MAX) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);

  uint32_t start_address;
  ENDIAN_memcpy(&start_address, param + 1, sizeof(uint32_t));
  if (start_address >= FM25V10_STOP_ADDRESS) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);

  uint8_t write_data_byte;
  write_data_byte = param[5];
  DS_CMD_ERR_CODE ret;
  ret = FM25V10_write_bytes(&fm25v10_driver_[idx], start_address, &write_data_byte, 1);

  return DS_conv_cmd_err_to_ccp_cmd_ret(ret);
}

CCP_CmdRet Cmd_DI_FM25V10_READ_BYTE(const CommonCmdPacket* packet)
{
  const uint8_t* param = CCP_get_param_head(packet);

  FM25V10_IDX idx;
  idx = (FM25V10_IDX)param[0];
  if (idx >= FM25V10_IDX_MAX) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);

  uint32_t start_address;
  ENDIAN_memcpy(&start_address, param + 1, sizeof(uint32_t));
  if (start_address >= FM25V10_STOP_ADDRESS) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);

  uint8_t read_data_byte = 0x00;
  DS_CMD_ERR_CODE ret;
  ret = FM25V10_read_bytes(&fm25v10_driver_[idx], start_address, &read_data_byte, 1);

  return DS_conv_cmd_err_to_ccp_cmd_ret(ret);
}

CCP_CmdRet Cmd_DI_FM25V10_MANAGE_STATUS(const CommonCmdPacket* packet)
{
  const uint8_t* param = CCP_get_param_head(packet);

  FM25V10_IDX idx;
  idx = (FM25V10_IDX)param[0];
  if (idx >= FM25V10_IDX_MAX) return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);

  uint8_t read_or_write;  // 0 = write, others = read
  read_or_write = param[1];

  uint8_t write_status;
  write_status = param[2];

  DS_CMD_ERR_CODE ret;
  if (read_or_write == 0)
  {
    ret = FM25V10_write_status(&fm25v10_driver_[idx], write_status);
  }
  else
  {
    ret = FM25V10_read_status(&fm25v10_driver_[idx]);
  }

  return DS_conv_cmd_err_to_ccp_cmd_ret(ret);
}

// 外部公開関数
DI_FM25V10_ERROR DI_FM25V10_global_write_bytes(const uint32_t global_start_address, const uint32_t length, const uint8_t* write_data)
{
  if (length > DI_FM25V10_MAX_LENGTH) return DI_FM25V10_ERROR_NG_LENGTH;

  uint32_t stop_address = global_start_address + length;
  if (stop_address > DI_FM25V10_STOP_GLOBAL_ADDRESS) return DI_FM25V10_ERROR_NG_LENGTH;

  FM25V10_IDX idx;
  uint32_t local_start_address;
  DI_FM25V10_ERROR error_in_address = DI_FM25V10_calc_local_address_(&idx, &local_start_address, global_start_address);
  if (error_in_address != DI_FM25V10_ERROR_OK) return error_in_address;

  uint32_t overrun_length;
  overrun_length = DI_FM25V10_calc_device_overrun_length_(local_start_address, length);
  uint32_t length_first_device = length - overrun_length;

  DS_CMD_ERR_CODE error_in_write = FM25V10_write_bytes(&fm25v10_driver_[idx], local_start_address, write_data, length_first_device);
  if (error_in_write != DS_CMD_OK) return DI_FM25V10_ERROR_NG_WRITE;

  if (overrun_length == 0) return DI_FM25V10_ERROR_OK;

  error_in_write = FM25V10_write_bytes(&fm25v10_driver_[idx + 1], 0, write_data + length_first_device, overrun_length);
  if (error_in_write != DS_CMD_OK) return DI_FM25V10_ERROR_NG_WRITE;
  // 2回オーバーランが起きるほどの大きなデータ書き込みは想定しない (実行時間的にも多分無理なので)

  return DI_FM25V10_ERROR_OK;
}

DI_FM25V10_ERROR DI_FM25V10_global_read_bytes(uint8_t* read_data, const uint32_t global_start_address, const uint32_t length)
{
  if (length > DI_FM25V10_MAX_LENGTH) return DI_FM25V10_ERROR_NG_LENGTH;

  uint32_t stop_address = global_start_address + length;
  if (stop_address > DI_FM25V10_STOP_GLOBAL_ADDRESS) return DI_FM25V10_ERROR_NG_LENGTH;

  FM25V10_IDX idx;
  uint32_t local_start_address;
  DI_FM25V10_ERROR error_in_address = DI_FM25V10_calc_local_address_(&idx, &local_start_address, global_start_address);
  if (error_in_address != DI_FM25V10_ERROR_OK) return error_in_address;

  uint32_t overrun_length;
  overrun_length = DI_FM25V10_calc_device_overrun_length_(local_start_address, length);
  uint32_t length_first_device = length - overrun_length;

  DS_CMD_ERR_CODE error_in_read = FM25V10_read_bytes(&fm25v10_driver_[idx], local_start_address, read_data, length_first_device);
  if (error_in_read != DS_CMD_OK) return DI_FM25V10_ERROR_NG_READ;

  if (overrun_length == 0) return DI_FM25V10_ERROR_OK;

  error_in_read = FM25V10_read_bytes(&fm25v10_driver_[idx + 1], 0, read_data + length_first_device, overrun_length);
  if (error_in_read != DS_CMD_OK) return DI_FM25V10_ERROR_NG_READ;
  // 2回オーバーランが起きるほどの大きなデータ読み出しは想定しない (実行時間的にも多分無理なので)

  return DI_FM25V10_ERROR_OK;
}

static DI_FM25V10_ERROR DI_FM25V10_calc_local_address_(FM25V10_IDX* idx,
                                                       uint32_t* local_start_address,
                                                       const uint32_t global_start_address)
{
  if (global_start_address >= DI_FM25V10_STOP_GLOBAL_ADDRESS) return DI_FM25V10_ERROR_NG_ADDRESS;

  const uint32_t kMaxDataSize = FM25V10_STOP_ADDRESS;

  uint8_t fram_idx = (uint8_t)(global_start_address / kMaxDataSize);  // 切り捨て
  if (fram_idx >= FM25V10_IDX_MAX) return DI_FM25V10_ERROR_NG_ADDRESS;
  *idx = (FM25V10_IDX)fram_idx;

  *local_start_address = global_start_address - (fram_idx * kMaxDataSize);

  return DI_FM25V10_ERROR_OK;
}

static uint32_t DI_FM25V10_calc_device_overrun_length_(const uint32_t local_start_address,
                                                       const uint32_t length)
{
  uint32_t stop_address = local_start_address + length;
  if (stop_address < FM25V10_STOP_ADDRESS) return 0;

  return stop_address - FM25V10_STOP_ADDRESS;
}
#pragma section
