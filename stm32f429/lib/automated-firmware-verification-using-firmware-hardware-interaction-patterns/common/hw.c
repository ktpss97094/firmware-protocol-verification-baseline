#include "hw.h"
#include "stm32f429xx.h"
#include <stdbool.h>

#if __has_include("config.h")
#include "config.h"
#endif

extern bool nondet_bool(void);
extern unsigned int nondet_uint(void);

extern uint16_t CBMC_SIZE;
__attribute__((weak)) extern uint32_t CBMC_TIMEOUT;

bool prev_scl_in = true, prev_sda_in = true, prev_scl_out = true, prev_sda_out = true, arbitration_lost = false, arbitration_lost_byte_end = false, wait_state = false;
unsigned int bit_count = 0;

I2C_TypeDef I2C1_Model, I2C2_Model, I2C3_Model;
GPIO_TypeDef GPIOA_Model, GPIOB_Model, GPIOC_Model, GPIOD_Model, GPIOE_Model, GPIOF_Model, GPIOG_Model, GPIOH_Model, GPIOI_Model, GPIOJ_Model, GPIOK_Model;

static void* _GetModelPtr(uint32_t base) {
  switch(base) {
    case I2C1_BASE: return &I2C1_Model;
    case I2C2_BASE: return &I2C2_Model;
    case I2C3_BASE: return &I2C3_Model;
    case GPIOA_BASE: return &GPIOA_Model;
    case GPIOB_BASE: return &GPIOB_Model;
    case GPIOC_BASE: return &GPIOC_Model;
    case GPIOD_BASE: return &GPIOD_Model;
    case GPIOE_BASE: return &GPIOE_Model;
    case GPIOF_BASE: return &GPIOF_Model;
    case GPIOG_BASE: return &GPIOG_Model;
    case GPIOH_BASE: return &GPIOH_Model;
    case GPIOI_BASE: return &GPIOI_Model;
    case GPIOJ_BASE: return &GPIOJ_Model;
    case GPIOK_BASE: return &GPIOK_Model;
    default: return 0;
  }
}

static void _CalcBaseOffset(uint32_t reg_addr, uint32_t *base, uint32_t *offset) {
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
  uint32_t base, offset;
  _CalcBaseOffset((uint32_t)reg_addr, &base, &offset);

  uint32_t output = 0;
  void* model_ptr = _GetModelPtr(base);
  if (model_ptr) {
    output = *(uint32_t *)((uint8_t *)model_ptr + offset);
  }

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

          // __CPROVER_assume((output & I2C_CR1_PE) == I2C_CR1_PE);
          // __CPROVER_assume((output & I2C_CR1_SWRST) != I2C_CR1_SWRST);

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

          // BUSY
          output = (output & ~I2C_SR2_BUSY) | (nondet_bool() ? I2C_SR2_BUSY : 0);

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

  *(uint32_t *)((uint8_t *)model_ptr + offset) = output;
  *reg_addr = output;

  switch (base) {
#if defined(SW_I2C_SCL_Pin) && defined(SW_I2C_SDA_Pin)
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
          bool scl_in = output & SW_I2C_SCL_Pin;
          bool sda_in = output & SW_I2C_SDA_Pin;
          
          bool is_stop_condition = prev_scl_in && scl_in && (!prev_sda_in) && sda_in;

          if (is_stop_condition) {
            arbitration_lost = false;
            arbitration_lost_byte_end = false;
          }

          if (wait_state && scl_in) {
            wait_state = false;
          }

          prev_scl_in = scl_in;
          prev_sda_in = sda_in;
          
          break;
      }
      break;
#endif
  }
  
  return output;
}

void SetRegister(__IO uint32_t *reg_addr, uint32_t value)
{
  uint32_t base, offset;
  _CalcBaseOffset((uint32_t)reg_addr, &base, &offset);

  void* model_ptr = _GetModelPtr(base);
  if (model_ptr) {
    switch (base) {
#if !defined(SW_I2C_SCL_Pin) && !defined(SW_I2C_SDA_Pin)
      case I2C1_BASE:
      case I2C2_BASE:
      case I2C3_BASE:
        switch (offset) {
          case offsetof(I2C_TypeDef, CR1):
            __CPROVER_assert(
              !(value & I2C_CR1_START) ||
              !(GetRegister(base + offsetof(I2C_TypeDef, SR2)) & I2C_SR2_MSL) ||
              !(GetRegister(base + offsetof(I2C_TypeDef, SR1)) & I2C_SR1_ARLO)
              , "arbitration (hw spec 1) violation"
            );

            __CPROVER_assert(
              !(value & I2C_CR1_STOP) ||
              !(GetRegister(base + offsetof(I2C_TypeDef, SR2)) & I2C_SR2_MSL) ||
              !(GetRegister(base + offsetof(I2C_TypeDef, SR1)) & I2C_SR1_ARLO)
              , "arbitration (hw spec 3) violation"
            );

            break;

          case offsetof(I2C_TypeDef, DR):
            __CPROVER_assert(
              !(GetRegister(base + offsetof(I2C_TypeDef, SR2)) & I2C_SR2_MSL) ||
              !(GetRegister(base + offsetof(I2C_TypeDef, SR1)) & I2C_SR1_ARLO)
              , "arbitration (hw spec 2) violation"
            );

          break;
        }
        break;
#endif

#if defined(SW_I2C_SCL_Pin) && defined(SW_I2C_SDA_Pin)
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
          case offsetof(GPIO_TypeDef, ODR):
            __CPROVER_assert(
              (value & ((uint32_t)SW_I2C_SDA_Pin)) ||
              (!arbitration_lost)
              , "arbitration (sw spec 1) violation"
            );

            __CPROVER_assert(
              (value & ((uint32_t)SW_I2C_SCL_Pin)) ||
              (!arbitration_lost_byte_end)
              , "arbitration (sw spec 3) violation"
            );

            __CPROVER_assert(
              (value & ((uint32_t)SW_I2C_SCL_Pin)) ||
              (!wait_state)
              , "clock_stretching (spec 1) violation"
            );
            break;

          case offsetof(GPIO_TypeDef, BSRR):
            __CPROVER_assert(
              (value & ((uint32_t)SW_I2C_SDA_Pin)) ||
              (!(value & ((uint32_t)SW_I2C_SDA_Pin << 16U))) || 
              (!arbitration_lost)
              , "arbitration (sw spec 2) violation"
            );

            __CPROVER_assert(
              (value & ((uint32_t)SW_I2C_SCL_Pin)) ||
              (!(value & ((uint32_t)SW_I2C_SCL_Pin << 16U))) || 
              (!arbitration_lost_byte_end)
              , "arbitration (sw spec 4) violation"
            );

            __CPROVER_assert(
              (value & ((uint32_t)SW_I2C_SCL_Pin)) ||
              (!(value & ((uint32_t)SW_I2C_SCL_Pin << 16U))) || 
              (!wait_state)
              , "clock_stretching (spec 2) violation"
            );
            break;
        }
        break;
#endif
    }

    *(uint32_t *)((uint8_t *)model_ptr + offset) = value;
    *reg_addr = value;
  }

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
        case offsetof(GPIO_TypeDef, BSRR):
        case offsetof(GPIO_TypeDef, ODR):
#if defined(SW_I2C_SCL_Pin) && defined(SW_I2C_SDA_Pin)
          uint32_t idr = GetRegister((__IO uint32_t *)(base + offsetof(GPIO_TypeDef, IDR)));
          uint32_t odr = GetRegister((__IO uint32_t *)(base + offsetof(GPIO_TypeDef, ODR)));
          bool scl_in = idr & SW_I2C_SCL_Pin;
          bool sda_in = idr & SW_I2C_SDA_Pin;
          bool scl_out = odr & SW_I2C_SCL_Pin;
          bool sda_out = odr & SW_I2C_SDA_Pin;

          bool is_rising_edge = (!prev_scl_out) && scl_out;
          bool is_falling_edge = prev_scl_out && (!scl_out);
          bool is_start_condition = prev_scl_out && scl_out && prev_sda_out && (!sda_out);

          if (is_start_condition) {
            bit_count = 0;
          }
          else {
            if (is_rising_edge) {
              bit_count = (bit_count + 1) % 9;
            }
          }

          if (is_rising_edge) {
            arbitration_lost = arbitration_lost || (sda_out && (!sda_in));
          }

          if (is_rising_edge && (!scl_in)) {
            wait_state = true;
          }

          if (is_falling_edge) {
            arbitration_lost_byte_end = arbitration_lost_byte_end || (arbitration_lost && (bit_count == 0));
          }

          prev_scl_out = scl_out;
          prev_sda_out = sda_out;
#endif
          break;
      }
      break;
  }
}

uint32_t GetSysTick(void) {
  static unsigned int idx = 0;
  static uint32_t tickstart;
  static bool first_call = true;

  if (first_call) {  // 第一次呼叫 HAL_GetTick() 是取得 tickstart
    first_call = false;
    tickstart = nondet_uint();
    return tickstart;
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
    return V_x;
  }
  else {
    return V_x_recur;
  }
}