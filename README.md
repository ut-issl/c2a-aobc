# C2A-AOBC
- 姿勢制御を行うOBCであるAOBC用搭載S/Wのレポジトリです。
- 基本的には次のような構成で使うことを想定しています。
  - 実機
    - vMicro + Visual Studio 2019
  - SILS
    - S2E-AOBC: [`develop` branch](https://github.com/ut-issl/s2e-aobc)
    - WINGS: 最新の [`develop` branch](https://gitlab.com/ut_issl/wings/wings)

# clone方法
- `C2A-CORE` がgit submoduleとして含まれているので，以下で一発．
```
$ git clone --recursive git@github.com:ut-issl/c2a-aobc.git
```
-  もしくは，以下でもOK．
```
$ git clone git@github.com:ut-issl/c2a-aobc.git
$ cd c2a-aobc/
$ git submodule init
$ git submodule update
```
- clone後， windowsの場合は`RUN_AFTER_CLONE.bat`，macの場合は`RUN_AFTER_CLONE.command`を実行する．
  - これで，localのgit環境設定が行われる
  - 別ディレクトリなどに新しくcloneし直した場合も，実行し直す
  - `Examples`フォルダへのコピーも行われる．


# 開発環境構築手順
1. Arduino IDEのインストール
   - [Arduino](https://www.arduino.cc/en/software)をインストールする
   - Arduinoプロジェクトを起動する
   - ボード設定の追加
     - `メニューバー＞ファイル＞環境設定`　で追加のボードマネージャのURLに下記を入力してOKボタンを閉じる
     - https://github.com/chipKIT32/chipKIT-core/raw/master/package_chipkit_index.json
   - ChipKITのインストール
     - `メニューバー＞ツール＞ボード＞ボードマネージャ`の一番下にある「chipKIT」の「インストール」ボタンをクリック。
     - インストール完了を待つ。
2. ボード定義ファイルの追加
   - `C:\Users\^^^\AppData\Local\Arduino15\packages\chipKIT\hardware\pic32\2.1.0`ディレクトリを開く
     - ディレクトリ名は環境によって変わる可能性があるので注意
   - 上記ディレクトリ内の`board.txt`を開き、一番最後の行に移動する
   - `settings\board.txt`を開き、内容をコピーし、上で開いている`2.1.0\board.txt`の最期に貼り付ける
   - `2.1.0\board.txt`を保存して閉じる
   - `settings\MXL795`ディレクトリをコピーし、`\2.1.0\variants`ディレクトリ内に貼り付ける

3. Visual StudioとVisual Microをインストールする
   - 手順は略

4. Visual Microの設定
   - `AOBC_BBM.sln`をダブルクリックして開く
   - `拡張機能>vMicro`を選び、次のように設定する
     - IDE: Arduino 1.6/1.8
     - Board: PIC32MX795F512L(80MHz)
     - Port: 環境依存


# テレコマ編集方法
1. テレコマ DB のエクセルを用いて csv を編集
   - [テレコマDBの使い方](https://github.com/ut-issl/tlm-cmd-db)
2. tlm-cmd-generator を実行してソースコードに反映させる
   - [tlm-cmd-generatorの使い方](https://github.com/ut-issl/c2a-tlm-cmd-code-generator)
   - [ISSL用のブランチ](https://github.com/ut-issl/c2a-tlm-cmd-code-generator/tree/feature/issl) に git checkout すること
   - settings.json は AOBC 用に以下のように編集すること
```
{
  "c2a_root_dir" : "各自の環境でのc2a-aobc/src/への相対パス",
  "db_path" : "各自の環境でのc2a-aobc/src/src_user/Settings/TlmCmd/DataBase/への相対パス",
  "db_prefix" : "ISSL6U_AOBC",
  "tlm_id_range" : ["0x00", "0x100"],
  "is_cmd_prefixed_in_db" : 1,
  "input_file_encoding" : "utf-8",
  "output_file_encoding" : "utf-8",
  "is_main_obc" : 0
}
```


# 開発方針
+ branch構成
```
├── main        # 実機での動作保証のある最新版
├── develop     # ビルドが通る最新版
└── feature/*   # 開発途中のソフトウェア群
└── hotfix/*    # mainに対してパッチを当てるソフトウェア群
```
+ 機能開発の流れ
	1. developブランチからfeatureブランチを作成する．
	2. featureブランチ内で変更をcommit&pushする．
	3. 実装が完了した時点でdevelopブランチへのマージリクエストを出す．
	4. Maintainerによるレビューを行う．問題があれば 5 へ．なければ 6 へ．
	5. レビューへの回答，及び修正のcommit&pushを行う． 4 へ．
	6. Maintainerがdevelopブランチへのマージを行う．
+ バイナリファイル
	- 基本的にコミットしない．
	- 目安として．200kBを超えるファイルは怪しいと思おう！


# コーディング規則
- 基本的にCDHが制定している[コーディング規則](https://gitlab.com/ut_issl/c2a/documents_oss/-/blob/master/General/CodingRule.md)に従う
- その他、AOCSとして特有な下記のことにも気をつける
  - 変数、関数命名時に単位がわかるように配慮する
    - 搭載S/W内では、系内で意思統一した単位系を用いる
    - 地上局での表示などでは人間が直感的に理解しやすい単位系でも良い
  - 変数、関数命名時に座標系がわかるように配慮する
    - `eci`: ECI
    - `body`: 機体固定座標系
    - `compo`: コンポーネント座標系
    - 他にもあれば追記していく
  - 略語については、[こちら](https://gitlab.com/ut_issl/c2a/c2a_core_oss/-/issues/77#note_568506174)を見る。
  - C2A Core部はC言語としてビルドされている点に注意
    - C2AのCore機能はC言語としてビルドされているので、C++と組み合わせる時には注意する。
    - 基本的には適切に`Extern "C"`を使えば問題ないはず

# Tips
- Visual Studioをフィルタではなくフォルダで表示する設定にすると使いやすい
- S2Eと組み合わせた動作検証時のビルドエラーについては，まずは下記の操作を試してみると良い
  - `git submodule update`を実施する
  - S2Eのcmakeキャッシュを削除する

# 本リポジトリに対するユーザーサイドリポジトリの設定について
- 同じAOCSユニットを使う場合でも、衛星固有でパラメータを調整する必要がある。これらのパラメータは`src/Settings/SatelliteParameters`に整理されており、衛星毎に別途値を定める必要がある。
- 衛星固有の設定ファイルなどを含んだユーザーリポジトリを作成する際は、`Examples` のフォルダ構成を参考にすること。

## Brief history of development
- 21st Jan. 2021: Development start in a private repository at GitLab.
- 03rd Mar. 2023: Move to a private repository at GitHub to prepare publish as OSS

## Contributors in the GitLab repository
- ISSL, UT
  - Satoshi Ikari: 963 commits
  - Ryo Suzumoto: 367 commits
  - Takayuki Hosonuma: 352 commits
  - Toshihiro Suzuki: 341 commits
  - Hirotaka Sekine: 242 commits
  - Toshio Imamura: 149 commits
  - Masahiro Fujiwara: 129 commits
  - Yutaro Ito: 55 commits
  - Riki Nakamura: 14 commits
  - Yoshinari Gyu: 13 commits
- ArkEdge Space Inc.
  - sksat: 8 commits
