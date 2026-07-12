#include "hw.h"
#include "stm32f4xx_hal_driver.h"

void stub_SET_BIT(__IO uint32_t *REG_addr, uint32_t BIT)
{
  SetRegister(REG_addr, GetRegister(REG_addr) | BIT);
}

void stub_CLEAR_BIT(__IO uint32_t *REG_addr, uint32_t BIT)
{
  SetRegister(REG_addr, GetRegister(REG_addr) & ~BIT);
}

uint32_t stub_READ_BIT(__IO uint32_t *REG_addr, uint32_t BIT)
{
  return GetRegister(REG_addr) & BIT;
}

FlagStatus stub_HAL_I2C_GET_FLAG(I2C_HandleTypeDef *__HANDLE__, uint32_t __FLAG__)
{
  return (((uint8_t)((__FLAG__) >> 16U)) == 0x01U) ? (((GetRegister(&((__HANDLE__)->Instance->SR1)) & ((__FLAG__) & I2C_FLAG_MASK)) == ((__FLAG__) & I2C_FLAG_MASK)) ? SET : RESET) : (((GetRegister(&((__HANDLE__)->Instance->SR2)) & ((__FLAG__) & I2C_FLAG_MASK)) == ((__FLAG__) & I2C_FLAG_MASK)) ? SET : RESET);
}

void stub_HAL_I2C_CLEAR_ADDRFLAG(I2C_HandleTypeDef *__HANDLE__)
{
  do {
    __IO uint32_t tmpreg = 0x00U;
    tmpreg = GetRegister(&((__HANDLE__)->Instance->SR1));
    tmpreg = GetRegister(&((__HANDLE__)->Instance->SR2));
    UNUSED(tmpreg);
  } while(0);
}

uint32_t HAL_GetTick(void)
{
  uwTick = GetSysTick();
  return uwTick;
}