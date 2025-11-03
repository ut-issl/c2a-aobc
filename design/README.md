# 設計情報

設計情報となる`BCT`, `CMD`, `TLM`の情報はこのcsvファイルをベースに`generate.py`で生成する

## 手順

- `bct/xxx.csv`, `tlm/xxx.csv`, `cmd.csv`を編集する
-  VSCode tasksで`generate c2a`を実行する
-  `rye run python design/generate.py`を実行する

## BCT

- 新規追加したい場合は以下の書式に従って既存のcsvに追記するか、新規ファイルを作成する
- 新たにヘッダファイルに`#include`したいファイルが出てきたら`design/generate.py`ファイルの`bc_header_header`を更新する

### 書式

- subsystem, name, bcid(廃止予定), type, option, ti, cmd, args, description, noteからなる
- BCの定義に必要な列
    - subsystem: どこにも使われない情報だが、どこの担当なのかを明確にした方がわかりやすいため定義する。BCごとに定義
    - name: BC名。わかりやすい名前にする
    - bcid(廃止予定): BCID。まもなく自動で振られるようになる
    - description: BCの説明。わかりやすいとよい。定義ファイルにも反映される
    - note: BCに関してなにかあれば。
- BCの中身の定義に必要な列
    - type: app/combine/cmd/deploy/cmd_to_other_obc/codeのいずれか
        - app: `BCL_tool_register_app`
        - combine: `BCL_tool_register_combine`
        - cmd: `BCL_tool_register_cmd`, 必要に応じてargsも定義する。
        - deploy: `BCL_tool_register_deploy`, optionに`TLCD_ID`が必須。
        - cmd_to_other_obc: `BCL_tool_register_cmd_to_other_obc`, optionに`APID`が必須。
        - code: そのままcファイルに情報を含めたい場合。`#ifdef`など
    - option: `type: deploy,cmd_to_other_obc`の場合。`deploy`の場合は`TLCD_ID`を、`cmd_to_other_obc`の場合は`APID`を含める
    - ti: TI。`type: code`以外必須
    - cmd: 引数に取るコマンド。`type: code`以外必須。
    - args: `type: cmd`のときに引数が必要であれば`型1:値1,型2:値2,...`の書式で含める
    - description: BCの中身の説明。わかりやすいとよい。定義ファイルにも反映される。
    - note: BCの中身に関してなにかあれば。

### `design/generate.py`で生成されるもの

- `src/src_user/tlm_cmd/block_command_definitions.c`: `BC_load_defaults`が定義されている。読み込み時間が読めないため、6つごとに`WDT_clear_wdt()`を入れている
- `src/src_user/tlm_cmd/block_command_definitions.h`: `BC_DEFAULT_ID`構造体の定義
- `src/src_user/tlm_cmd/normal_block_command_definition/nbc_header.h`: すべての`bc_xxx.c`でincludeされているヘッダファイル。外部の`#include`はここで行われる
- `src/src_user/tlm_cmd/normal_block_command_definition/bc_xxx.c`: `bc_xxx.csv`に従って定義されるBCの関数
- `database/*.csv`: WINGSとの互換性を保つためにこれまで通りの書式のファイルも生成する

## CMD

- 新規追加したい場合は以下の書式に従って既存のcsvに追記する
- CMDのIDは0から順に振られる

### 書式

- enabled, subsystem, target, name, danger, param1_type, param1_description, ..., param6_type, param6_description, description, note
- enabled: そのコマンドが有効か無効か
- subsystem: どこにも使われない情報だが、合った方がよいため定義。
- name: コマンド名
- dangaer: 危険フラグ。どこにも使われてないが、いつか使われるかもしれない
- paramx_type: パラメタの型。`uint8_t/uint16_t/uint32_t/float/double`のいずれか
- paramx_description: パラメタの説明。定義するとよい。
- description: コマンドの説明
- note: コマンドのメモ

### `design/generate.py`で生成されるもの

- `src/src_user/tlm_cmd/command_definitions.c`
- `src/src_user/tlm_cmd/command_definitions.h`
- `database/*.csv`: WINGSとの互換性を保つためにこれまで通りの書式のファイルも生成する

## TLM

- 新規追加したい場合や変更したい場合は以下の書式に従って既存のcsvに追記する

### 書式

- 1行目
    - TRUE/FALSE: 有効か無効か
    - ID: テレメのID
    - TARGET
    - LOCAL VAR
- 2行目以降
    - subsystem, name, type, bit, var, conv, a0-a5, status, description, note, priority
    - subsystem: どこにも使われないが担当を振った場合などに利用
    - name: テレメ名
    - type: テレメの値の型
        - `uint8_t, uint16_t, uint32_t, int8_t, int16_t, int32_t, float, double`のいずれか
        - bit詰めをする場合は最初に`_`を追加する
    - bit: bit詰めをする場合のみ
        - bitいくつ分使うかを定義する。以下は一例。`uint16_t`は16バイトなので3つの値を合わせて16バイトとなっている
            - `_uint16_t,3`
            - `,1`
            - `,12`
    - var: 参照する変数
    - conv: NONE(そのまま), HEX(16進数表示), POLY(a0+a1x+a2x^2+...+a5x^5の結果で表示), STATUS(statusに従って表示)
    - a0-a5: convがPOLYの場合
    - status: convがSTATUSの場合、`値1=文字列1,値2=文字列2,...`のようにして変換して表示される。
    - description: テレメの値の説明。あるとよい
    - note: テレメの値のメモ。あるとよい

### `design/generate.py`で生成されるもの

- `src/src_user/tlm_cmd/telemetry_definitions.c`
- `src/src_user/tlm_cmd/telemetry_definitions.h`
- `database/TLM/*.csv`: WINGSとの互換性を保つためにこれまで通りの書式のファイルも生成する
- `database/TLM/calced_data/*.csv`: WINGSとの互換性を保つためにこれまで通りの書式のファイルも生成する
