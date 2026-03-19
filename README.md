# Automated Firmware Verification Using Firmware-Hardware Interaction Patterns

> [!WARNING]
> This repository is loosely inspired by the concepts discussed in the paper [Automated Firmware Verification Using Firmware-Hardware Interaction Patterns](http://arks.princeton.edu/ark:/88435/dsp01s4655k00v). It is intended solely for evaluation purposes and does not represent the source code or a direct implementation of the paper.

## Project Structure

* `nrf52840/lib/nrfx`: A **git subtree** of the [nrfx](https://github.com/NordicSemiconductor/nrfx) repository

## Verify

```sh
cd nrf52840/protocols/TWI/master/Non_DMA_Mode/nrfx
cbmc src/main.c $(git rev-parse --show-toplevel)/nrf52840/lib/nrfx/drivers/src/nrfx_twi.c -I config/ -I $(git rev-parse --show-toplevel)/nrf52840/lib/nrfx -I $(git rev-parse --show-toplevel)/nrf52840/lib/nrfx/drivers/include -I $(git rev-parse --show-toplevel)/nrf52840/lib/CMSIS_5/CMSIS/Core/Include -I $(git rev-parse --show-toplevel)/nrf52840/lib/nrfx/bsp/stable -I $(git rev-parse --show-toplevel)/nrf52840/lib/nrfx/bsp/stable/mdk -DNRF52840_XXAA --arch arm --32 --unwind 20 --trace --no-standard-checks --unwinding-assertions --drop-unused-functions --beautify
```