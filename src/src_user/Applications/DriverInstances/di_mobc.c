#pragma section REPRO
/**
* @file
* @brief MOBC の DI. MOBC における GS との DI に相当する
*/

#include "di_mobc.h"
#include <src_core/TlmCmd/packet_handler.h>
#include <src_core/Library/print.h>
#include "../../Settings/port_config.h"
#include "../../Settings/DriverSuper/driver_buffer_define.h"

static void DI_MOBC_init_(void);
static void DI_MOBC_update_(void);
static void DI_MOBC_ms_tlm_packet_handler_init_(void);
static void DI_MOBC_ms_tlm_packet_handler_(void);

static MOBC_Driver mobc_driver_;
const MOBC_Driver* const mobc_driver = &mobc_driver_;

// バッファ
static DS_StreamRecBuffer DI_MOBC_rx_buffer_;
static uint8_t DI_MOBC_rx_buffer_allocation_[DS_STREAM_REC_BUFFER_SIZE_DEFAULT];

/**
 * 一度に送出する最大テレメ数
 * 115200 bps なので 240 Byte 1 packet 送るだけでも 17 ms かかる計算
 * 4 packet も送信すると それだけ 70ms 近くかかり送信だけで姿勢制御が 10 Hz で回らなくなるためそこまで送るべきではない
 * 逆に AOBC は 1 秒で 10 Tlm 以上送れなくなる
 */
static const uint8_t DI_MOBC_kMsTlmPhMaxNumOfProc_ = 1;


AppInfo DI_MOBC_update(void)
{
  return AI_create_app_info("update_MOBC", DI_MOBC_init_, DI_MOBC_update_);
}

static void DI_MOBC_init_(void)
{
  DS_ERR_CODE ret1;
  DS_INIT_ERR_CODE ret2;

  ret1 = DS_init_stream_rec_buffer(&DI_MOBC_rx_buffer_,
                                   DI_MOBC_rx_buffer_allocation_,
                                   sizeof(DI_MOBC_rx_buffer_allocation_));
  if (ret1 != DS_ERR_CODE_OK)
  {
    Printf("MOBC buffer init Failed ! %d \n", ret1);
  }

  ret2 = MOBC_init(&mobc_driver_, PORT_CH_UART_MOBC, &DI_MOBC_rx_buffer_);
  if (ret2 != DS_INIT_OK)
  {
    Printf("MOBC init Failed ! %d \n", ret2);
  }
}

static void DI_MOBC_update_(void)
{
  DS_REC_ERR_CODE ret;
  ret = MOBC_rec(&mobc_driver_);
}


AppInfo DI_MOBC_ms_tlm_packet_handler(void)
{
  return AI_create_app_info("MOBC_ms_tlm_ph",
                            DI_MOBC_ms_tlm_packet_handler_init_,
                            DI_MOBC_ms_tlm_packet_handler_);
}

static void DI_MOBC_ms_tlm_packet_handler_init_(void)
{
  // do nothing
}

static void DI_MOBC_ms_tlm_packet_handler_(void)
{
  CommonTlmPacket* packet;
  mobc_driver_.info.c2a.send_tlm_err_code = DS_CMD_OK;

  for (uint8_t i = 0; i < DI_MOBC_kMsTlmPhMaxNumOfProc_; i++)
  {
    // TODO_L: PH_ms_tlm_listをDIから隠蔽する？それか何かしらの共用構造体でのインスタンスの一部にするか？
    //         そうすると，ms_tlmの他のtlmができたときに共通化が容易

    if (PL_is_empty(&PH_ms_tlm_list))
    {
      return;
    }

    packet = (CommonTlmPacket*)PL_get_head(&PH_ms_tlm_list)->packet;

    // 現状，WINGSの問題から DUMP TLMは考えない
    if (CTP_get_apid(packet) != CTP_APID_FROM_ME)
    {
      // TODO_L: 送信不良アノマリを入れるか考える
      mobc_driver_.info.c2a.send_tlm_err_code = DS_CMD_ILLEGAL_PARAMETER;
      continue;
    }

    // CTP のヘッダを別途設定する
    // 今は必要が無い

    DS_CMD_ERR_CODE ret = MOBC_send(&mobc_driver_, packet);
    if (ret != DS_CMD_OK)
    {
      mobc_driver_.info.c2a.send_tlm_err_code = ret;
    }

    PL_drop_executed(&PH_ms_tlm_list);
  }
}

#pragma section
