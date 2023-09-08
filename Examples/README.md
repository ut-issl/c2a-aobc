# C2A-AOBC-EXAMPLE

## Overview

- `C2A-AOBC-EXAMPLE` is an example of a project-specific repository of `C2A-AOBC`.
- Users can copy this directory to make their simulation environment.
  - **NOTE**: Please rewrite all words like `example, EXAMPLE, or Example` to suit your project in this document after you copy the directory. After that, please delete this description.
- For other detailed descriptions, please also see README of [c2a-aobc](https://github.com/ut-issl/c2a-aobc).

## How to construct the repository

- Directory tree
  ```
  - c2a-aobc-example (Examples directory)
    - src_aobc (git submodule)
    - src_aobc_example
    - project definition files.
  ```
- `git submodule`
  - This repository includes the [c2a-aobc](https://github.com/ut-issl/c2a-aobc) with the `git submodule`. And the `c2a-aobc` also includes [c2a-core](https://github.com/ut-issl/c2a-core) as a submodule. Please use the following command to clone the repository recursively.
    ```
    $ git clone --recursive git@github.com:ut-issl/c2a-aobc-example.git
    ```
## Build with vMicro for real AOBC (Attitude On Board Computer) development

- Please see the `README` of [c2a-aobc](./src_aobc/README.md)
  - This link will be available after the repository construction of `c2a-aobc-example`.

## Build with S2E for SILS test

- Please see the `README` of `s2e-aobc-example`.

## How to change the project-specific parameters

- In the `src_aobc_example/Settings/SatelliteParameters` directory, there are C source files to define the initial satellite parameters.
- Please modify the parameters to suit your project.
