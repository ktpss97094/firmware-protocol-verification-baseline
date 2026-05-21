#include "stm32_bitbang_i2c.h"
#include <stdbool.h>

extern unsigned int nondet_uint(void);

uint32_t GetCYCCNT(uint32_t delay)
{
  /* Wrap-around monotonic variable */
  
  static unsigned int idx = 0;
  static uint32_t B = 0;

  if (delay == 0) {
    B = B + nondet_uint();
    return B;
  }

  /* Fitness Value */
  const int A = 1;
  const int fitness_value = (delay / A) + 1;  // Table 5.1 (9)，B = clk_cycle_start, C = clk_cycle_start + microseconds
  
  /* Producer Vector V_x */
  const unsigned int V_x_len = fitness_value + 1;

  /* Infinitely Recurring Values V_x[recur, *] */
  int V_x_recur = nondet_uint();

  // Table 5.3 (b)
  idx += nondet_uint();
  if (idx < V_x_len) {
    unsigned int V_x = B + idx * A;
    return V_x;
  }
  else {
    return V_x_recur;
  }
}