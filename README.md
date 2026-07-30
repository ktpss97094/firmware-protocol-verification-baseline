# Firmware Protocol Verification Baseline

> [!WARNING]
> This repository is loosely inspired by the concepts discussed in the paper [Automated Firmware Verification Using Firmware-Hardware Interaction Patterns](http://arks.princeton.edu/ark:/88435/dsp01s4655k00v). It is intended solely for evaluation purposes and does not represent the source code or a direct implementation of the paper.

## Project Structure

* `stm32f429/lib/stm32f4xx-hal-driver`: A **git subtree** of the [stm32f4xx-hal-driver](https://github.com/STMicroelectronics/stm32f4xx-hal-driver) repository
* `stm32f429/lib/stm32_bitbang_i2c`: A **git subtree** of the [stm32_bitbang_i2c](https://github.com/ktpss97094/stm32_bitbang_i2c) repository
* `stm32f429/lib/libopencm3`: A **git subtree** of the [libopencm3](https://github.com/libopencm3/libopencm3) repository

## Environment

Ubuntu 24.04 LTS

## Prerequisite

1. Install [cbmc](https://github.com/diffblue/cbmc)

## Build

```sh
cd <mcu directory>
cmake -DCMAKE_TOOLCHAIN_FILE=../cmake/toolchains/cbmc.cmake -B build
```

## Compile

```sh
cd build
make
```

## Verify

```sh
make <cmake project name>-verify
```

## Utilities

### Show Loops

```sh
make <cmake project name>-show-loops
```