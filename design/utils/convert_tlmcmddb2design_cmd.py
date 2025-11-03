#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import csv
import re
import sys
from pathlib import Path

# 出力ヘッダ
OUT_HEADER = [
    "enabled","subsystem","target","name","danger",
    "param1_type","param1_description",
    "param2_type","param2_description",
    "param3_type","param3_description",
    "param4_type","param4_description",
    "param5_type","param5_description",
    "param6_type","param6_description",
    "description","note"
]

def normalize(s):
    return (s or "").strip()

def to_int_if_numeric(s):
    s = normalize(s)
    if re.fullmatch(r"[+-]?\d+", s):
        return int(s)
    return None

def find_3line_header(rows):
    """
    先頭から順に、
    1) 1行目: 'Component,Name,Target,Code,...Danger Flag,Is Restricted,Description,Note'
    2) 2行目: 'MOBC,,,,Num Params,Param1,,Param2,,Param3,,Param4,,Param5,,Param6,,,,,'
    3) 3行目: 'Comment,,,,,Type,Description,Type,Description,...'
    の3行ブロックを検出し、各列インデックスを返す。
    """
    # 1行目（列名行）を探す
    header1_idx = None
    for i, r in enumerate(rows):
        if any(cell.strip() == "Component" for cell in r) and \
           any(cell.strip() == "Name" for cell in r) and \
           any("Danger" in cell for cell in r) and \
           any("Restricted" in cell for cell in r):
            header1_idx = i
            break
    if header1_idx is None:
        raise RuntimeError("1行目ヘッダ（Component/Name/.../Danger Flag/Is Restricted/...）が見つかりません。")

    # 2行目（Param列のヘッダ）を拾う
    if header1_idx + 1 >= len(rows):
        raise RuntimeError("2行目ヘッダが存在しません。")
    header2 = rows[header1_idx + 1]

    # 3行目（Type/Description 行）
    if header1_idx + 2 >= len(rows):
        raise RuntimeError("3行目ヘッダが存在しません。")
    header3 = rows[header1_idx + 2]

    # --- 1行目の基本列 ---
    def find_col(row, name_contains):
        for idx, c in enumerate(row):
            if name_contains in (c or ""):
                return idx
        return None

    name_idx        = find_col(rows[header1_idx], "Name")
    target_idx      = find_col(rows[header1_idx], "Target")
    component_idx   = find_col(rows[header1_idx], "Component")
    code_idx        = find_col(rows[header1_idx], "Code")
    danger_idx      = find_col(rows[header1_idx], "Danger Flag")
    restricted_idx  = find_col(rows[header1_idx], "Is Restricted")
    desc_idx        = find_col(rows[header1_idx], "Description")
    note_idx        = find_col(rows[header1_idx], "Note")

    for k, v in {
        "Name": name_idx, "Target": target_idx, "Code": code_idx,
        "Description": desc_idx, "Note": note_idx
    }.items():
        if v is None:
            raise RuntimeError(f"必須列 '{k}' が1行目ヘッダから見つかりません。")

    # --- 2/3行目から Param 列（Type/Description のペア）を特定 ---
    # 方針: 3行目を左から見て、「Type」「Description」のペアを順番に拾う。
    # その直上（2行目）が Param1/Param2/... である前提だが、名称は使わず順番で最大6つまで採用。
    param_pairs = []  # [(type_col_idx, desc_col_idx), ...]
    for c in range(len(header3) - 1):
        if header3[c].strip() == "Type" and header3[c+1].strip() == "Description":
            param_pairs.append((c, c+1))
    # 最大6個に丸める（不足していてもOK）
    param_pairs = param_pairs[:6]

    # Num Params 列（2行目に "Num Params" と書かれた列）
    num_params_col = None
    for c, v in enumerate(header2):
        if normalize(v) == "Num Params":
            num_params_col = c
            break

    return {
        "start_data_row": header1_idx + 3,
        "indices": {
            "name": name_idx, "target": target_idx, "component": component_idx,
            "code": code_idx, "danger": danger_idx, "restricted": restricted_idx,
            "description": desc_idx, "note": note_idx, "num_params": num_params_col
        },
        "param_pairs": param_pairs
    }

def convert_cmdcsv_to_design(in_csv_path, out_csv_path, subsystem="CDH"):
    in_path = Path(in_csv_path)
    out_path = Path(out_csv_path)

    # 読み込み
    rows = []
    with in_path.open(newline="", encoding="utf-8-sig") as f:
        for r in csv.reader(f):
            rows.append(r)

    hdr = find_3line_header(rows)
    start_row = hdr["start_data_row"]
    idx = hdr["indices"]
    param_pairs = hdr["param_pairs"]

    # 書き出し
    with out_path.open("w", newline="", encoding="utf-8") as f:
        w = csv.writer(f)
        w.writerow(OUT_HEADER)

        for r in rows[start_row:]:
            # セクション行や空行をスキップ（例: "* C2A_CORE,基幹機能コマンド,..."）
            head0 = normalize(r[0]) if r else ""
            if not any(r) or head0.startswith("*"):
                continue

            # Name 必須（なければスキップ）
            name = normalize(r[idx["name"]]) if idx["name"] is not None and len(r) > idx["name"] else ""
            if not name:
                continue

            target = normalize(r[idx["target"]]) if idx["target"] is not None and len(r) > idx["target"] else ""
            # Danger はそのまま（例では空）
            danger = normalize(r[idx["danger"]]) if idx["danger"] is not None and len(r) > idx["danger"] else ""
            description = normalize(r[idx["description"]]) if idx["description"] is not None and len(r) > idx["description"] else ""
            note = normalize(r[idx["note"]]) if idx["note"] is not None and len(r) > idx["note"] else ""

            # パラメータ数
            num_params = 0
            if idx["num_params"] is not None and len(r) > idx["num_params"]:
                num_params = to_int_if_numeric(r[idx["num_params"]]) or 0
            if num_params < 0:  # 念のため
                num_params = 0
            if num_params > 6:
                num_params = 6

            # param i（最大6）を詰める
            params_out = []
            for i in range(6):
                if i < len(param_pairs) and i < num_params:
                    t_idx, d_idx = param_pairs[i]
                    ptype = normalize(r[t_idx]) if len(r) > t_idx else ""
                    pdesc = normalize(r[d_idx]) if len(r) > d_idx else ""
                else:
                    ptype = ""
                    pdesc = ""
                params_out.extend([ptype, pdesc])

            row_out = [
                "TRUE",           # enabled: 常に TRUE
                subsystem,        # subsystem: 例に合わせて "CDH"
                target,           # target
                name,             # name
                danger,           # danger
                *params_out,      # param1..6 (type,description ×6)
                description,      # description
                note,             # note
            ]
            w.writerow(row_out)

# 直接実行
if __name__ == "__main__":
    if len(sys.argv) < 3:
        print("Usage: python convert_cmd_csv.py input.csv output.csv")
        sys.exit(1)
    convert_cmdcsv_to_design(sys.argv[1], sys.argv[2])
