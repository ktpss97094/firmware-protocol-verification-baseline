#include "stm32f4xx_hal_driver.h"
#include "main.h"
#include <stdbool.h>

extern bool nondet_bool(void);
extern unsigned int nondet_uint(void);

extern uint16_t CBMC_SIZE;
__attribute__((weak)) extern uint32_t CBMC_TIMEOUT;

bool prev_scl_out = true, arbitration_lost = false, arbitration_lost_byte_end = false;
unsigned int bit_count = 0;

void _CalcBaseOffset(uint32_t reg_addr, uint32_t *base, uint32_t *offset) {
  if (reg_addr >= 0xE0000000U) {  // Cortex-M Core 內部周邊
    *base = reg_addr & ~0xFFFU;
    *offset = reg_addr & 0xFFFU;
  } else {  // 外部周邊
    *base = reg_addr & ~0x3FFU;
    *offset = reg_addr & 0x3FFU;
  }
}

uint32_t GetRegister(__IO uint32_t *reg_addr)
{
  uint32_t output = *reg_addr;
  uint32_t base, offset;

  _CalcBaseOffset((uint32_t)reg_addr, &base, &offset);

  switch (base) {
    case I2C1_BASE:
    case I2C2_BASE:
    case I2C3_BASE:
      switch (offset) {
        // CR1
        case offsetof(I2C_TypeDef, CR1):  
          // POS
          output = (output & ~I2C_CR1_POS) | (nondet_bool() ? I2C_CR1_POS : 0);

          // START
          output = (output & ~I2C_CR1_START) | (nondet_bool() ? I2C_CR1_START : 0);

          // STOP
          output = (output & ~I2C_CR1_STOP) | (nondet_bool() ? I2C_CR1_STOP : 0);

          __CPROVER_assume((output & I2C_CR1_PE) == I2C_CR1_PE);
          __CPROVER_assume((output & I2C_CR1_SWRST) != I2C_CR1_SWRST);

          break;
        
        case offsetof(I2C_TypeDef, SR1):
          // SB
          output = (output & ~I2C_SR1_SB) | (nondet_bool() ? I2C_SR1_SB : 0);

          // ADD10
          output = (output & ~I2C_SR1_ADD10) | (nondet_bool() ? I2C_SR1_ADD10 : 0);

          // AF
          output = (output & ~I2C_SR1_AF) | (nondet_bool() ? I2C_SR1_AF : 0);

          // ADDR
          output = (output & ~I2C_SR1_ADDR) | (nondet_bool() ? I2C_SR1_ADDR : 0);

          // TxE
          output = (output & ~I2C_SR1_TXE) | (nondet_bool() ? I2C_SR1_TXE : 0);

          // BTF
          output = (output & ~I2C_SR1_BTF) | (nondet_bool() ? I2C_SR1_BTF : 0);

          // ARLO
          output = (output & ~I2C_SR1_ARLO) | (nondet_bool() ? I2C_SR1_ARLO : 0);

          break;

        case offsetof(I2C_TypeDef, SR2):
          // TRA
          output = (output & ~I2C_SR2_TRA) | (nondet_bool() ? I2C_SR2_TRA : 0);

          // MSL
          output = (output & ~I2C_SR2_MSL) | (nondet_bool() ? I2C_SR2_MSL : 0);

          break;
      }
      break;

    case GPIOA_BASE:
    case GPIOB_BASE:
    case GPIOC_BASE:
    case GPIOD_BASE:
    case GPIOE_BASE:
    case GPIOF_BASE:
    case GPIOG_BASE:
    case GPIOH_BASE:
    case GPIOI_BASE:
    case GPIOJ_BASE:
    case GPIOK_BASE:
      switch (offset) {
        case offsetof(GPIO_TypeDef, IDR):
        case offsetof(GPIO_TypeDef, ODR):
          output = (output & ~0xFFFF) | (nondet_uint() & 0xFFFF);
          
          break;
      }
      break;
  }
  
  return output;
}

void SetRegister(__IO uint32_t *reg_addr, uint32_t value)
{
  *reg_addr = value;

  uint32_t base, offset;

  _CalcBaseOffset((uint32_t)reg_addr, &base, &offset);

  switch (base) {
    case GPIOA_BASE:
    case GPIOB_BASE:
    case GPIOC_BASE:
    case GPIOD_BASE:
    case GPIOE_BASE:
    case GPIOF_BASE:
    case GPIOG_BASE:
    case GPIOH_BASE:
    case GPIOI_BASE:
    case GPIOJ_BASE:
    case GPIOK_BASE:
      switch (offset) {
        // BSRR
        case offsetof(GPIO_TypeDef, BSRR):
#if defined(SW_I2C_SCL_Pin) && defined(SW_I2C_SDA_Pin)
          uint32_t idr = GetRegister(base + offsetof(GPIO_TypeDef, IDR));
          uint32_t odr = GetRegister(base + offsetof(GPIO_TypeDef, ODR));
          bool sda_in = idr & SW_I2C_SDA_Pin;
          bool scl_out = odr & SW_I2C_SCL_Pin, sda_out = odr & SW_I2C_SDA_Pin;

          if (!prev_scl_out && scl_out) {
            bit_count = (bit_count + 1) % 9;

            arbitration_lost = arbitration_lost || (sda_out && !sda_in);

            arbitration_lost_byte_end = arbitration_lost_byte_end || (arbitration_lost && bit_count == 0);
          }

          prev_scl_out = scl_out;
#endif
          break;
      }
      break;
  }
}

void stub_SET_BIT(__IO uint32_t *REG_addr, uint32_t BIT)
{
  *REG_addr = GetRegister(REG_addr) | BIT;
}

void stub_CLEAR_BIT(__IO uint32_t *REG_addr, uint32_t BIT)
{
  *REG_addr = GetRegister(REG_addr) & ~BIT;
}

uint32_t stub_READ_BIT(__IO uint32_t *REG_addr, uint32_t BIT)
{
  return GetRegister(REG_addr) & BIT;
}

FlagStatus stub_HAL_I2C_GET_FLAG(I2C_HandleTypeDef *__HANDLE__, uint32_t __FLAG__)
{
  return (((uint8_t)((__FLAG__) >> 16U)) == 0x01U) ? (((GetRegister(&((__HANDLE__)->Instance->SR1)) & ((__FLAG__) & I2C_FLAG_MASK)) == ((__FLAG__) & I2C_FLAG_MASK)) ? SET : RESET) : (((GetRegister(&((__HANDLE__)->Instance->SR1)) & ((__FLAG__) & I2C_FLAG_MASK)) == ((__FLAG__) & I2C_FLAG_MASK)) ? SET : RESET);
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
  static unsigned int idx = 0;
  static uint32_t tickstart;
  static bool first_call = true;

  if (first_call) {  // 第一次呼叫 HAL_GetTick() 是取得 tickstart
    first_call = false;
    tickstart = nondet_uint();
    uwTick = tickstart;
    return uwTick;
  }

  /* Fitness Value */
  const int A = 1, B = tickstart, C = tickstart + CBMC_TIMEOUT;
  const int fitness_value = (B - C) > 0 ? 1 : ((C - B) / A) + 1;  // Table 5.2
  
  /* Producer Vector V_x */
  const unsigned int V_x_len = fitness_value + 1;

  /* Infinitely Recurring Values V_x[recur, *] */
  int V_x_recur = tickstart + fitness_value + nondet_uint();

  idx += nondet_uint();

  if (idx < V_x_len) {
    unsigned int V_x = tickstart + idx * A;
    uwTick = V_x;
  }
  else {
    uwTick = V_x_recur;
  }

  return uwTick;
}