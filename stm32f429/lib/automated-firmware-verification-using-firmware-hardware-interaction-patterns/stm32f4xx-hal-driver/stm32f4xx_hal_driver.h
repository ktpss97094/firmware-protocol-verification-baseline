#ifndef __AUTOMATED_FIRMWARE_VERIFICATION_USING_FIRMWARE_HARDWARE_INTERACTION_PATTERNS_STM32F4XX_HAL_DRIVER_H
#define __AUTOMATED_FIRMWARE_VERIFICATION_USING_FIRMWARE_HARDWARE_INTERACTION_PATTERNS_STM32F4XX_HAL_DRIVER_H

#include "hw.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_i2c.h"

void stub_SET_BIT(__IO uint32_t *, uint32_t);
void stub_CLEAR_BIT(__IO uint32_t *, uint32_t);
uint32_t stub_READ_BIT(__IO uint32_t *, uint32_t);
FlagStatus stub_HAL_I2C_GET_FLAG(I2C_HandleTypeDef *, uint32_t);
void stub_HAL_I2C_CLEAR_ADDRFLAG(I2C_HandleTypeDef *);
uint32_t HAL_GetTick(void);

#ifdef SET_BIT
#undef SET_BIT
#define SET_BIT(REG, BIT) stub_SET_BIT(&(REG), BIT)
#endif

#ifdef CLEAR_BIT
#undef CLEAR_BIT
#define CLEAR_BIT(REG, BIT) stub_CLEAR_BIT(&(REG), BIT)
#endif

#ifdef READ_BIT
#undef READ_BIT
#define READ_BIT(REG, BIT) stub_READ_BIT(&(REG), BIT)
#endif

#ifdef READ_REG
#undef READ_REG
#define READ_REG(REG) GetRegister(&(REG))
#endif

#ifdef __HAL_I2C_GET_FLAG
#undef __HAL_I2C_GET_FLAG
#define __HAL_I2C_GET_FLAG(__HANDLE__, __FLAG__) stub_HAL_I2C_GET_FLAG(__HANDLE__, __FLAG__)
#endif

#ifdef __HAL_I2C_CLEAR_ADDRFLAG
#undef __HAL_I2C_CLEAR_ADDRFLAG
#define __HAL_I2C_CLEAR_ADDRFLAG(__HANDLE__) stub_HAL_I2C_CLEAR_ADDRFLAG(__HANDLE__)
#endif

#endif /* __AUTOMATED_FIRMWARE_VERIFICATION_USING_FIRMWARE_HARDWARE_INTERACTION_PATTERNS_STM32F4XX_HAL_DRIVER_H */