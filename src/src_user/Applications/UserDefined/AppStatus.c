#pragma section REPRO
#include "AppStatus.h"

#include <src_core/TlmCmd/common_cmd_packet_util.h>
#include <src_core/Library/print.h>
#include "../../Library/vt100.h"
#include "../../TlmCmd/command_definitions.h"
#include <src_core/System/ApplicationManager/app_manager.h>

static ASInfo asi_;
const ASInfo* asi;

static void AppStatus_init_(void);
static void print_app_status_(void);
static AS_ACK set_app_id_(size_t id);

AppInfo AppStatus(void)
{
  return AI_create_app_info("as", AppStatus_init_, NULL);
}

static void AppStatus_init_(void)
{
  asi = &asi_;
  asi_.app_id = 0;
}

AppInfo print_app_status(void)
{
  return AI_create_app_info("ass", NULL, print_app_status_);
}

static void print_app_status_(void)
{
  VT100_erase_line();
  Printf("APP: ID %d, NAME %s, PREV %d, MIN %d, MAX %d\n",
         asi_.app_id, app_manager->ais[asi_.app_id].name, app_manager->ais[asi_.app_id].prev,
         app_manager->ais[asi_.app_id].min, app_manager->ais[asi_.app_id].max);
}

CCP_CmdRet Cmd_AS_set_id(const CommonCmdPacket* packet)
{
  AS_ACK ack = AS_UNKNOWN;

  if (CCP_get_param_len(packet) != 1)
  {
    return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_LENGTH);
  }

  ack = set_app_id_((size_t)CCP_get_param_head(packet)[0]);

  switch (ack)
  {
  case AS_SUCCESS:
    return CCP_make_cmd_ret_without_err_code(CCP_EXEC_SUCCESS);

  case AS_INVALID_ID:
    return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);

  default:
    return CCP_make_cmd_ret_without_err_code(CCP_EXEC_ILLEGAL_PARAMETER);
  }
}

static AS_ACK set_app_id_(size_t id)
{
  if (id >= AM_MAX_APPS)
  {
    return AS_INVALID_ID;
  }

  asi_.app_id = id;
  return AS_SUCCESS;
}
#pragma section
