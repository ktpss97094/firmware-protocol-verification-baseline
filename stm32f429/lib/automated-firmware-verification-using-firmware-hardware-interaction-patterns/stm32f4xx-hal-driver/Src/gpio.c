#include "gpio.h"
#include "main.h"

extern unsigned int nondet_uint(void);

bool prev_scl_out = true, arbitration_lost = false, arbitration_lost_byte_end = false;
unsigned int bit_count = 0;

void SetRegister(__IO uint32_t *reg_addr, uint32_t value)
{
  *reg_addr = value;

  uint32_t offset = (uint32_t)reg_addr & 0x3FF;
  uint32_t idr = GetRegister(reg_addr - offset + offsetof(GPIO_TypeDef, IDR));
  uint32_t odr = GetRegister(reg_addr - offset + offsetof(GPIO_TypeDef, ODR));
  bool sda_in = (idr & SW_I2C_SDA_Pin) != 0;
  bool scl_out = (odr & SW_I2C_SCL_Pin) != 0, sda_out = (odr & SW_I2C_SDA_Pin) != 0;

  switch (offset) {
    // BSRR
    case offsetof(GPIO_TypeDef, BSRR):
      if (!prev_scl_out && scl_out) {
        bit_count = (bit_count + 1) % 9;

        arbitration_lost = arbitration_lost || (sda_out && !sda_in);

        arbitration_lost_byte_end = arbitration_lost_byte_end || (arbitration_lost && bit_count == 0);
      }

      prev_scl_out = scl_out;
    }
}

uint32_t GetRegister(__IO uint32_t *reg_addr)
{
  uint32_t output = *reg_addr;
  uint32_t offset = (uint32_t)reg_addr & 0x3FF;

  switch (offset) {
    // IDR
    case offsetof(GPIO_TypeDef, IDR):
    case offsetof(GPIO_TypeDef, ODR):
      output = (output & 0xFFFF) | (nondet_uint() & 0xFFFF);
      
      break;
  }

  return output;
}