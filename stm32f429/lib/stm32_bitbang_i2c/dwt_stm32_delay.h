#ifndef DWT_STM32_DELAY_H
#define DWT_STM32_DELAY_H

#ifdef __cplusplus
extern "C" {
#endif


#if defined(STM32F4)
  #include "stm32f4xx_hal.h"
#else
  #include "stm32f1xx_hal.h"
#endif
uint32_t DWT_Delay_Init(void);

#include "stm32_bitbang_i2c.h"

/**
 * @brief  This function provides a delay (in microseconds)
 * @param  microseconds: delay in microseconds
 */
__STATIC_INLINE void DWT_Delay_us(volatile uint32_t microseconds)
{
  // uint32_t clk_cycle_start = DWT->CYCCNT;
  uint32_t clk_cycle_start = GetCYCCNT(0);  // REWRITE:

  /* Go to number of cycles for system */
  microseconds *= (HAL_RCC_GetHCLKFreq() / 1000000);

  /* Delay till end */
  // while ((DWT->CYCCNT - clk_cycle_start) < microseconds);
  while ((GetCYCCNT(microseconds) - clk_cycle_start) < microseconds);  // REWRITE:
}


#ifdef __cplusplus
}
#endif

#endif
