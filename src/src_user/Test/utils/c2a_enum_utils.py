#!/usr/bin/env python3
# -*- coding: utf-8 -*-
import json
import os
import c2aenum as c2a


def get_c2a_enum():
    c2a_src_abs_path = os.path.dirname(__file__).replace("\\", "/") + "/../../.."

    return c2a.load_enum(c2a_src_abs_path, "utf-8")

