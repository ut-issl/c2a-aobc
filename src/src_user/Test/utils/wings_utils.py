#!/usr/bin/env python3
# -*- coding: utf-8 -*-
import isslwings as wings

obc_info = {
    "name": "AOBC",
    "hk_tlm_info": {
        "tlm_name": "AOBC_HK_GEN",
        "cmd_counter": "OBC.GS_CMD.COUNTER",
        "cmd_last_exec_id": "OBC.GS_CMD.LAST_EXEC_ID",
        "cmd_last_exec_sts": "OBC.GS_CMD.LAST_EXEC_STS",
    },
}


def get_wings_operation():
    return wings.Operation(obc_info=obc_info)
