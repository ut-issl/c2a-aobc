#ifndef APPSTATUS_H_
#define APPSTATUS_H_

#include <stddef.h> // for size_t

#include <src_core/System/ApplicationManager/app_info.h>
#include <src_core/TlmCmd/common_cmd_packet.h>

typedef enum
{
  AS_SUCCESS,
  AS_INVALID_ID,
  AS_UNKNOWN
} AS_ACK;

typedef struct
{
  size_t app_id;
} ASInfo;

extern const ASInfo* asi;

AppInfo AppStatus(void);

AppInfo print_app_status(void);

CCP_CmdRet Cmd_AS_set_id(const CommonCmdPacket* packet);

#endif
