# C2A-AOBC

## Overview

- `C2A-AOBC` is the flight software for the AOCS module developed by ISSL/UT, Seiren, and JAXA.
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

- Please refer [c2a-aobc-example](https://github.com/ut-issl/c2a-aobc-example) repository.
- Users can fork or copy of this example repository.
- We also have manuals to construct SILS test environment. Please visit the [Google Drive](https://drive.google.com/drive/folders/19g8cxltzAX10oCNxCCKT4S8HwTYReHKe) to find the manuals.
  - Sorry, but we only have manuals written in Japanese now. We need help to translate them.

### How to join the development of this repository

- When general users add new features or remove this repository's bugs, please make upstream pull requests from a forked repository.
- Before making PRs, please carefully read the following `Development style` for main developers.
- If you have any questions, please feel free to ask us.

### Note
- The term `body-fixed frame` in the codes and tlm/cmd database means `the body-fixed frame of the AOCS module`. It does not necessarily coincide with the body-fixed frame of your satellite.

## For main developers
### How to clone the repository
  - This repository includes [c2a-core](https://github.com/arkedge/c2a-core) as the `git submodule`. Please use the following commands to construct the directory.
    ```
    $ git clone git@github.com:ut-issl/c2a-aobc-core-v4.git
    $ cd c2a-aobc-core-v4/
    $ git submodule init
    $ git submodule update
    ```
  - Or use the following commands to clone the repository.
    ```
    $ git clone --recursive git@github.com:ut-issl/c2a-aobc-core-v4.git
    ```

### How to construct the development environment for SILS
- Follow the instructions in the [c2a_core documentation](https://github.com/arkedge/c2a-core/blob/main/docs/sils/c2a_dev_runtime.md)
- Install the required tools:
  - rustup: https://www.rust-lang.org/learn/get-started
  - nvm: https://github.com/nvm-sh/nvm#installing-and-updating
  - yarn: https://classic.yarnpkg.com/lang/en/docs/install/
  - rye: https://rye-up.com/guide/installation/
- Install compilers:
    ```
    sudo apt install gcc-multilib protobuf-compiler libclang-dev cmake g++ g++-multilib
    ```
- Setup
    - Under the c2a-aobc-core-v4/sils directory, run:
    ```
    nvm install
    corepack enable
    pnpm install
    ```
### Running SILS
- To run only C2A
    - Under the c2a-aobc-core-v4/sils directory, run:
    ```
    cargo run
    ```
- To run together with DevTools
    - Under the c2a-aobc-core-v4 directory, run:
    - After running the command below, you can access DevTools at [http://localhost:8900/devtools/](http://localhost:8900/devtools/)
    ```
    pnpm run devtools:sils
    ```

### How to construct the development environment for real AOBC
- Please use [monazite](https://github.com/arkedge/monazite).

       
### How to edit TLM/CMD (Telemetry/Command)
- Please find detailed information on `c2a-aobc-core-v4/design/README.md`.


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
- Please follow the [Coding rule of c2a-core](https://github.com/ut-issl/c2a-core/blob/042cdfa15b0056880398e857cdd5d5a430562fd1/Docs/General/coding_rule.md)
- Please also care about the AOCS-specific rules as follows.
  - Add unit information in the name of variables and functions
  - Add frame information in the name of variables and functions
    - `eci`: Earth Center Inertia frame
    - `body`: Body-fixed frame (**body frame of AOBC module**)
    - `compo`: Component frame
  - For abbreviations, please see the ISSL internal [document](https://gitlab.com/ut_issl/c2a/c2a_core_oss/-/issues/77#note_568506174).

## Used Projects

| Project Name          | Developer                             | Launch  | Refs       |
| ----------------------| ------------------------------------  | ------- | ---------- |
| SPHERE-1 EYE          | Sony Group Corporation and ISSL, UT   | 2023    | [Sekine 2023](https://archive.ists.ne.jp/upload_pdf/2023-f-6-02.pdf) |
| ONGLAISAT             | ISSL, UT                              | -       | [Ikari 2022](https://doi.org/10.57350/jesa.63) |

## Brief history of development
### History
- 21st Jan. 2021: Development started in a private repository at GitLab.
- 03rd Mar. 2023: Move to a private repository at GitHub to prepare to publish as OSS
- 14th Jul. 2023: Convert to a public repository.

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
1. S. Ikari, and et al., "Development of Compact and Highly Capable Integrated AOCS Module for CubeSats", Journal of Evolving Space Activities, vol. 1, ID 63, 2023. [Link](https://doi.org/10.57350/jesa.63)
1. H. Sekine, and et al., "Development of Software-In-the-Loop Simulator and Hardware-In-the-Loop Simulator of AOCS Module for CubeSats", 34th ISTS, Kurume, 2023. [Link](https://archive.ists.ne.jp/upload_pdf/2023-f-6-02.pdf)
