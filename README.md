# Automated Firmware Verification Using Firmware-Hardware Interaction Patterns

> [!WARNING]
> This repository is loosely inspired by the concepts discussed in the paper [Automated Firmware Verification Using Firmware-Hardware Interaction Patterns](http://arks.princeton.edu/ark:/88435/dsp01s4655k00v). It is intended solely for evaluation purposes and does not represent the source code or a direct implementation of the paper.

## Project Structure

* `nrf52840/lib/nrfx`: A **git subtree** of the [nrfx](https://github.com/NordicSemiconductor/nrfx) repository
* `stm32f429/lib/stm32f4xx-hal-driver`: A **git subtree** of the [stm32f4xx-hal-driver](https://github.com/STMicroelectronics/stm32f4xx-hal-driver) repository

## Prerequisite

1. Install [cbmc](https://github.com/diffblue/cbmc)

## Verify

### STM32F429

```sh
cbmc <Src path>/*.c stm32f429/lib/automated-firmware-verification-using-firmware-hardware-interaction-patterns/stm32f4xx-hal-driver/Src/hw.c $(find stm32f429/lib/stm32f4xx-hal-driver/Src -maxdepth 1 -name "*.c" ! -name "*template.c") -I <Inc path> -I stm32f429/lib/automated-firmware-verification-using-firmware-hardware-interaction-patterns/stm32f4xx-hal-driver/Inc -I stm32f429/lib/stm32f4xx-hal-driver/Inc -I stm32f429/lib/cmsis-device-f4/Include -I stm32f429/lib/CMSIS_5/CMSIS/Core/Include -DSTM32F429xx -DUSE_HAL_DRIVER --arch arm --32 --no-standard-checks --drop-unused-functions --unwindset I2C_WaitOnFlagUntilTimeout.1:3,I2C_WaitOnMasterAddressFlagUntilTimeout.2:3,I2C_WaitOnTXEFlagUntilTimeout.1:3,I2C_WaitOnBTFFlagUntilTimeout.1:3
```

### nRF52840

```sh
cd nrf52840/protocols/TWI/master/Non_DMA_Mode/nrfx
cbmc src/main.c $(git rev-parse --show-toplevel)/nrf52840/lib/nrfx/drivers/src/nrfx_twi.c -I config/ -I $(git rev-parse --show-toplevel)/nrf52840/lib/nrfx -I $(git rev-parse --show-toplevel)/nrf52840/lib/nrfx/drivers/include -I $(git rev-parse --show-toplevel)/nrf52840/lib/CMSIS_5/CMSIS/Core/Include -I $(git rev-parse --show-toplevel)/nrf52840/lib/nrfx/bsp/stable -I $(git rev-parse --show-toplevel)/nrf52840/lib/nrfx/bsp/stable/mdk -DNRF52840_XXAA --arch arm --32 --unwind 20 --trace --no-standard-checks --drop-unused-functions --beautify
```