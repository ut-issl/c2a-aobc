#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import csv
import re
import sys
from pathlib import Path

HEADER_ROW_2 = [
    "subsystem","name","type","bit","var","conv",
    "a0","a1","a2","a3","a4","a5",
    "status","description","note","priority"
]

def normalize(s):
    return (s or "").strip()

def to_int_if_numeric(s):
    s = normalize(s)
    if re.fullmatch(r"[+-]?\d+", s):
        return int(s)
    return None

def hex_to_int_maybe(s):
    s = normalize(s)
    m = re.fullmatch(r"0x([0-9A-Fa-f]+)", s)
    if m:
        return int(m.group(1), 16)
    n = to_int_if_numeric(s)
    if n is not None:
        return n
    return None

def is_formula(s):
    return normalize(s).startswith("=")

def needs_underscore_type(base_type, bit_len):
    """基底がビット切り詰め（bitが数値）の整数型なら '_' を付与。"""
    if bit_len is None:
        return False
    bt = (base_type or "").strip()
    return bt.startswith("uint") or bt.startswith("int")

def find_header_block(rows):
    """
    3行ヘッダ（セクション行 / 列名行 / サブ列名行）を検出。
    戻り値: (name_row_idx, header_rows_window)
      - name_row_idx: 「Name」を含む行インデックス
      - header_rows_window: [row0, row1, row2] の最大3行（不足時は存在分）
    """
    for i, r in enumerate(rows):
        if any(cell.strip() == "Name" for cell in r):
            # 直前～2行先までをヘッダ窓として扱う（最大3行）
            window = []
            if i - 1 >= 0:
                window.append(rows[i - 1])
            window.append(rows[i])
            if i + 1 < len(rows):
                window.append(rows[i + 1])
            return i, window
    raise RuntimeError("ヘッダの『Name』行が見つかりません。")

def find_col_in_window(window_rows, candidates):
    """ヘッダ窓の中から候補文字列（部分一致可）で列位置を探す。最初に見つかった位置を返す。"""
    for row in window_rows:
        for idx, cell in enumerate(row):
            c = cell.strip()
            for cand in candidates:
                if c == cand or cand in c:
                    return idx
    return None

def find_exact_in_window(window_rows, exacts):
    """完全一致のみで検索（a0..a5 のような短い語を誤検出しにくくする）。"""
    for row in window_rows:
        for idx, cell in enumerate(row):
            c = cell.strip()
            for ex in exacts:
                if c == ex:
                    return idx
    return None

def convert_tlmcmddb2design_tlm(in_path, out_path):
    in_path = Path(in_path)
    out_path = Path(out_path)

    # === 読み込み ===
    rows = []
    with in_path.open(newline="", encoding="utf-8-sig") as f:
        reader = csv.reader(f)
        for r in reader:
            rows.append(r)

    # --- メタ情報 ---
    target = ""
    packet_id_raw = ""
    is_restricted = ""
    raw_code = ""
    for r in rows[:10]:
        vals = r + [""] * (3 - len(r))
        key = normalize(vals[1] if len(vals) > 1 else "")
        val = normalize(vals[2] if len(vals) > 2 else "")
        if key == "Target":
            target = val
        elif key == "PacketID":
            packet_id_raw = val
            raw_code = normalize(vals[3] if len(vals) > 3 else "")
        elif key == "IsRestricted":
            is_restricted = val
    packet_id_dec = hex_to_int_maybe(packet_id_raw)
    if raw_code:
        raw_code = raw_code.replace("##", "\n").replace("@@", ",")

    # --- 3行ヘッダの検出 ---
    name_row_idx, header_window = find_header_block(rows)

    # 列インデックス決定（3行ヘッダ全体から探す）
    header_map = {}
    header_map["Name"]      = find_col_in_window(header_window, ["Name"])
    header_map["VarType"]   = find_col_in_window(header_window, ["Var.%%##Type","VarType"])
    header_map["VarOrFunc"] = find_col_in_window(header_window, ["Variable or Function Name"])
    header_map["ExtType"]   = find_col_in_window(header_window, ["Ext.%%##Type","ExtType"])
    header_map["OctetPos"]  = find_col_in_window(header_window, ["Octet%%##Pos."])
    header_map["BitPos"]    = find_col_in_window(header_window, ["bit%%##Pos."])
    header_map["BitLen"]    = find_col_in_window(header_window, ["bit%%##Len."])
    header_map["ConvType"]  = find_col_in_window(header_window, ["Conv.%%##Type"])
    # a0..a5 はサブ列名行に明示される想定なので完全一致で拾う
    header_map["a0"] = find_exact_in_window(header_window, ["a0"])
    header_map["a1"] = find_exact_in_window(header_window, ["a1"])
    header_map["a2"] = find_exact_in_window(header_window, ["a2"])
    header_map["a3"] = find_exact_in_window(header_window, ["a3"])
    header_map["a4"] = find_exact_in_window(header_window, ["a4"])
    header_map["a5"] = find_exact_in_window(header_window, ["a5"])
    header_map["Status"]      = find_col_in_window(header_window, ["Status"])
    header_map["Description"] = find_col_in_window(header_window, ["Description"])
    header_map["Note"]        = find_col_in_window(header_window, ["Note"])

    # バリデーション（最低限）
    if header_map["Name"] is None or header_map["VarType"] is None:
        raise RuntimeError("必須列（Name / Var.%%##Type）がヘッダから取得できません。3行ヘッダの整合を確認してください。")

    # --- データ行（Name 行の次から）---
    data_rows = []
    for r in rows[name_row_idx + 1:]:
        # Name が空ならスキップ
        name = normalize(r[header_map["Name"]]) if header_map["Name"] is not None and len(r) > header_map["Name"] else ""
        if not name:
            continue

        var_type   = normalize(r[header_map["VarType"]])   if header_map["VarType"]   is not None and len(r) > header_map["VarType"]   else ""
        var_orfunc = normalize(r[header_map["VarOrFunc"]]) if header_map["VarOrFunc"] is not None and len(r) > header_map["VarOrFunc"] else ""
        conv_type  = normalize(r[header_map["ConvType"]])  if header_map["ConvType"]  is not None and len(r) > header_map["ConvType"]  else ""

        bit_len_raw = r[header_map["BitLen"]] if header_map["BitLen"] is not None and len(r) > header_map["BitLen"] else ""
        bit_len = to_int_if_numeric(bit_len_raw)  # ← ここが数値なら bit 欄に出す

        # conv
        out_conv = conv_type

        # --- var（切り詰め派生は空。式(=...)も空。@@は", "に置換） ---
        if var_type == "||" or var_orfunc == "||" or is_formula(var_orfunc):
            out_var = ""
        else:
            out_var = var_orfunc.replace("@@", ",")

        # a0..a5
        def get_poly(cn):
            idx = header_map.get(cn)
            return normalize(r[idx]) if idx is not None and len(r) > idx else ""
        a0, a1, a2, a3, a4, a5 = (get_poly("a0"), get_poly("a1"), get_poly("a2"),
                                  get_poly("a3"), get_poly("a4"), get_poly("a5"))

        # status / description / note
        status = normalize(r[header_map["Status"]]) if header_map["Status"] is not None and len(r) > header_map["Status"] else ""
        description = normalize(r[header_map["Description"]]) if header_map["Description"] is not None and len(r) > header_map["Description"] else ""
        note = normalize(r[header_map["Note"]]) if header_map["Note"] is not None and len(r) > header_map["Note"] else ""

        # conv=STATUS: マッピング文字列を note に集約（@@→,）、status は空に
        if out_conv == "STATUS":
            mapping = status
            status = mapping.replace("@@", ",")

        # type / bit（切り詰め対応）
        if var_type == "||":
            out_type = ""                          # 派生は type 空
            out_bit  = str(bit_len) if bit_len is not None else ""
        else:
            out_type = var_type if var_type else ""
            if needs_underscore_type(out_type, bit_len):
                out_type = "_" + out_type          # 基底のみ '_' 付与
            out_bit  = str(bit_len) if bit_len is not None else ""

        data_rows.append([
            "",               # subsystem
            name,             # name
            out_type,         # type
            out_bit,          # bit
            out_var,          # var
            out_conv,         # conv
            a0, a1, a2, a3, a4, a5,
            status,           # status
            description,      # description
            note,             # note
            ""                # priority
        ])

    # --- 書き出し ---
    with out_path.open("w", newline="", encoding="utf-8") as f:
        writer = csv.writer(f)
        first_row = [is_restricted, str(packet_id_dec) if packet_id_dec is not None else "", target, raw_code] \
                    + [""] * (len(HEADER_ROW_2) - 4)
        writer.writerow(first_row)
        writer.writerow(HEADER_ROW_2)
        writer.writerows(data_rows)

if __name__ == "__main__":
    if len(sys.argv) < 3:
        print("Usage: python convert_tlm_csv.py input.csv output.csv")
        sys.exit(1)
    convert_tlmcmddb2design_tlm(sys.argv[1], sys.argv[2])
