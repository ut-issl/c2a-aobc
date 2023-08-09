#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import os
import sys

import isslwings as wings
import pytest

import time

ROOT_PATH = "../../../../../"
sys.path.append(os.path.dirname(__file__) + "/" + ROOT_PATH + "utils")
import c2a_enum_utils
import wings_utils

c2a_enum = c2a_enum_utils.get_c2a_enum()
ope = wings_utils.get_wings_operation()


@pytest.mark.real
@pytest.mark.sils
def test_target_quaternion():
    assert "SUC" == wings.util.send_rt_cmd_and_confirm(
        ope,
        c2a_enum.Cmd_CODE_APP_AOCS_MANAGER_SET_TARGET_QUATERNION,
        (0.0, 0.0, 0.0, 1.0),
        c2a_enum.Tlm_CODE_AOBC_HK_GEN,
    )
    time.sleep(1.0)
    tlm_AOBC_HK_ALGO = ope.get_latest_tlm(c2a_enum.Tlm_CODE_AOBC_HK_ALGO)[0]
    assert tlm_AOBC_HK_ALGO["AOBC_HK_ALGO.QUATERNION.TARGET_I2T_X"] == 0.0
    assert tlm_AOBC_HK_ALGO["AOBC_HK_ALGO.QUATERNION.TARGET_I2T_Y"] == 0.0
    assert tlm_AOBC_HK_ALGO["AOBC_HK_ALGO.QUATERNION.TARGET_I2T_Z"] == 0.0
    assert tlm_AOBC_HK_ALGO["AOBC_HK_ALGO.QUATERNION.TARGET_I2T_W"] == 1.0

@pytest.mark.real
@pytest.mark.sils
def test_julian_day():
    assert "SUC" == wings.util.send_rt_cmd_and_confirm(
        ope,
        c2a_enum.Cmd_CODE_APP_AOCS_MANAGER_SET_REFERENCE_TIME,
        (2459933.0,),
        c2a_enum.Tlm_CODE_AOBC_HK_GEN,
    )
    time.sleep(1.0)
    tlm_AOBC_HK_ALGO = ope.get_latest_tlm(c2a_enum.Tlm_CODE_AOBC_HK_ALGO)[0]
    assert tlm_AOBC_HK_ALGO["AOBC_HK_ALGO.TIME.REFERENCE_jday"] >= 2459933.0

if __name__ == "__main__":
    pass
