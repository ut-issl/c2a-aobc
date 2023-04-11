#pragma section REPRO
/**
 * @file
 * @brief  PacketHandlerのコマンドルーター，コマンドアナライザのユーザー定義部分
 */
#include "user_packet_handler.h"
#include <src_core/TlmCmd/packet_list_util.h>
#include <src_core/TlmCmd/common_cmd_packet_util.h>

void PH_user_init(void)
{
}


PH_ACK PH_user_analyze_cmd(const CommonCmdPacket* packet)
{
  switch (CCP_get_dest_type(packet))
  {
  default:
    return PH_ACK_UNKNOWN;
  }
}


CCP_CmdRet PH_user_cmd_router(const CommonCmdPacket* packet)
{
  APID apid = CCP_get_apid(packet);
  switch (apid)
  {
  default:
    // 該当する配送先が定義されていない場合
    return CCP_make_cmd_ret(CCP_EXEC_ROUTING_FAILED, apid);
  }
}

#pragma section
