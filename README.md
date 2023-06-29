# C2A-AOBC

## Overview

- `C2A-AOBC` is the flight software for the AOCS module developed by ISSL/UT, Seiren, and JAXA.
- Support environments
  - For real AOBC (Attitude On Board Computer)
    - [vMicro](https://www.visualmicro.com/) + Visual Studio 2019
  - For SILS/HILS test
    - [S2E-AOBC](https://github.com/ut-issl/s2e-aobc) v3.0.0
  - Telemetry/Command interface
    - [tlm-cmd-generator](https://github.com/ut-issl/c2a-tlm-cmd-code-generator)
      - Version: [ISSL Branch](https://github.com/ut-issl/c2a-tlm-cmd-code-generator/tree/feature/issl) 
    - [WINGS](https://gitlab.com/ut_issl/wings/wings)
- How to use
  - `The main developers` of the AOCS module directly use this repository to add new features and improve the module.
  - `General users` of the AOCS module do not need to use and edit this repository directly. They need to create a project-specific repository and define spacecraft-specific parameters within the repository.

## Release style

- We use [Semantic Versioning 2.0.0](https://semver.org/) as the versioning style
  - Basic version format is `<major>.<minor>.<patch>`(like `4.0.0`)
  - Public API is declared in the code itself (currently, there is no definitive list)
- All releases should be tagged as `v<semver>`(like `v4.0.0`)

## For general users
### How to make a project-specific repository

- We recommend making a project-specific repository named `c2a-aobc-hoge-satellite`.
- Users can refer to the `c2a-aobc/Example` directory to make a directory construction of `c2a-aobc-hoge-satellite`.
  - **NOTE** Please rewrite the `example` to your project name.
  ```
  TBW
  ```
- To use `git submodule` features, please see the reference [Git Tools Submodule](https://git-scm.com/book/en/v2/Git-Tools-Submodules).

### How to build and execute the project-specific repository

- Please see the [README.md](./Example/README.md) in the `Example` directly.

### How to join the development of this repository

- When general users add new features or remove this repository's bugs, please make upstream pull requests from a forked repository.
- Before making PRs, please carefully read the following `Development style`.
- If you have any questions, please feel free to ask us.


## For main developers
## How to clone the repository
  - This repository includes [c2a-core](https://github.com/ut-issl/c2a-core) with the `git submodule`. Please use the following commands to construct the directory.
    ```
    $ git clone git@github.com:ut-issl/c2a-aobc.git
    $ cd c2a-aobc/
    $ git submodule init
    $ git submodule update
    ```
  - Or use the following commands to clone the repository.
    ```
    $ git clone --recursive git@github.com:ut-issl/c2a-aobc.git
    ```
- After the clone, please execute the following script file for the local setting of git.
  - Windows: `RUN_AFTER_CLONE.bat`
  - Mac: `RUN_AFTER_CLONE.command`


### How to construct the development environment for real AOBC
1. Install Arduino IDE
   - Install [Arduino](https://www.arduino.cc/en/software)
   - Launch Arduino project
   - Add board setting
     - `メニューバー＞ファイル＞環境設定`
     - Add the following description to the additional board manager's URL
       - https://github.com/chipKIT32/chipKIT-core/raw/master/package_chipkit_index.json
   - Install `ChipKIT`
     - `メニューバー＞ツール＞ボード＞ボードマネージャ`
     - Click `Install` of `chipKIT`
2. Add board definition file
   - Open `C:\Users\^^^\AppData\Local\Arduino15\packages\chipKIT\hardware\pic32\2.1.0` directory
     - **NOTE** This directory name is changed by the PC environment
   - Open `board.txt`, and move to the end of the line.
   - Open `c2a-aobc/settings/board.txt`
   - Copy all contents and paste them to the end of the `2.1.0\board.txt`.
   - Save and close the `2.1.0\board.txt`.
   - Copy `c2a-aobc/settings/MXL795` directory and paste to the `\2.1.0\variants` directory

3. Install Visual Studio 2019 and [vMicro](https://www.visualmicro.com/)
   - TBW

4. Setting of `vMicro`
   - Open `C2A_AOBC.sln`
   - Select `拡張機能>vMicro` and set it as follows.
     - IDE: Arduino 1.6/1.8
     - Board: PIC32MX795F512L(80MHz)
     - Port: depends on your environment


### How to edit TLM/CMD (Telemetry/Command)
1. Open TLM/CMD DB
   - Please find detailed information on [How to use TLM/CMD DB](https://github.com/ut-issl/tlm-cmd-db).
2. Execute `tlm-cmd-generator` and generate source codes.
   - [How to use tlm-cmd-generator](https://github.com/ut-issl/c2a-tlm-cmd-code-generator)
   - Edit the `settings.json` as follows.
     ```
     {
       "c2a_root_dir" : "Relative path to c2a-aobc/src/",
       "db_path" : "Relative path to c2a-aobc/src/src_user/Settings/TlmCmd/DataBase/",
       "db_prefix" : "ISSL6U_AOBC",
       "tlm_id_range" : ["0x00", "0x100"],
       "is_cmd_prefixed_in_db" : 1,
       "input_file_encoding" : "utf-8",
       "output_file_encoding" : "utf-8",
       "is_main_obc" : 0
     }
     ```


### Development style

- Repository settings
  - Branch structure
    ```
    - main        # The latest operation guaranteed codes for general users
    - develop     # The latest buildable codes for S2E primary developers
    - feature/*   # Developing codes
    - hotfix/*    # Bug Fix codes
    ```
  - Push to `main` and `develop` is prohibited. All developers have to develop with `feature/*` or `hotfix/*` branch and make a pull request.

- Flow of development
  1. Make a `feature/*` branch from the `develop` branch.
     - To fix the small bugs in the latest release codes, please make `hotfix/*` branch from the `main` branch.
  2. Edit, commit, and push in the branch.
     - Please check the coding conventions and the `code format` in the next section.
  3. Create a new pull request to the `develop` branch.
     - The target branch becomes the `main` branch for the `hotfix/*` branches.
  4. A maintainer reviews the pull request. If some problems are found, the maintainer proposes modifications.
  5. According to the maintainer's proposal, the developer modifies the codes and goes back to 3.
  6. The maintainer merges the `feature/*` branch with the `develop` branch.
  7. The code owners decide to merge the `develop` branch to the `main` branch and release a new version.

- Binary files
  - Binary file commit is prohibited.
  - Please write the link to such files, or make a script file to get the files.
  - **Exception**
    - Images for markdown document files are allowable when the file size is smaller than 200K Bytes.

### Coding conventions
- Please follow the [Coding rule of c2a-core](https://github.com/ut-issl/c2a-core/blob/042cdfa15b0056880398e857cdd5d5a430562fd1/Docs/General/coding_rule.md)に従う
- Please also care about the AOCS-specific rules as follows.
  - Add unit information in the name of variables and functions
  - Add frame information in the name of variables and functions
    - `eci`: ECI frame
    - `body`: Body-fixed frame
    - `compo`: Component frame
  - For abbreviations, please see the ISSL internal [document](https://gitlab.com/ut_issl/c2a/c2a_core_oss/-/issues/77#note_568506174).

### Tips
- When using `vMicro`, setting the `Visual Studio` to display by folders instead of filters makes it easier to use.
- For build errors when SILS test with `S2E`, try the following operations first.
  - Perform `git submodule update`.
  - Delete cmake cache of S2E.

## Brief history of development
### History
- 21st Jan. 2021: Development started in a private repository at GitLab.
- 03rd Mar. 2023: Move to a private repository at GitHub to prepare to publish as OSS

### Contributors in the GitLab repository
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

## Publications
1. S. Ikari, and et al., "Development of Compact and Highly Capable Integrated AOCS Module for CubeSats", [2022-f-41](https://archive.ists.ne.jp/upload_pdf/F-9-05.pdf), 33rd ISTS, 2022.
