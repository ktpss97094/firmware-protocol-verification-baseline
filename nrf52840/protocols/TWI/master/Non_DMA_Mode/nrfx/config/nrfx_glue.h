/**
 * Copyright (c) 2017 - 2021, Nordic Semiconductor ASA
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form, except as embedded into a Nordic
 *    Semiconductor ASA integrated circuit in a product or a software update for
 *    such product, must reproduce the above copyright notice, this list of
 *    conditions and the following disclaimer in the documentation and/or other
 *    materials provided with the distribution.
 *
 * 3. Neither the name of Nordic Semiconductor ASA nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * 4. This software, with or without modification, must only be used with a
 *    Nordic Semiconductor ASA integrated circuit.
 *
 * 5. Any software provided in binary form under this license must not be reverse
 *    engineered, decompiled, modified and/or disassembled.
 *
 * THIS SOFTWARE IS PROVIDED BY NORDIC SEMICONDUCTOR ASA "AS IS" AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY, NONINFRINGEMENT, AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL NORDIC SEMICONDUCTOR ASA OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#ifndef NRFX_GLUE_H__
#define NRFX_GLUE_H__

#ifdef __cplusplus
extern "C" {
#endif

// We need to valid headers for these macros
#include <nrfx.h>
#include <nrf.h>

//------------------------------------------------------------------------------

#define NRFX_ASSERT(expression)

#define NRFX_STATIC_ASSERT(expression) _Static_assert(expression, "Static assertion failed")

//------------------------------------------------------------------------------

#define NRFX_IRQ_PRIORITY_SET(irq_number, priority)  NVIC_SetPriority(irq_number, priority)

#define NRFX_IRQ_ENABLE(irq_number)  NVIC_EnableIRQ(irq_number)

#define NRFX_IRQ_IS_ENABLED(irq_number)  NVIC_GetEnableIRQ(irq_number)

#define NRFX_IRQ_DISABLE(irq_number)  NVIC_DisableIRQ(irq_number)

#define NRFX_IRQ_PENDING_SET(irq_number)  NVIC_SetPendingIRQ(irq_number)

#define NRFX_IRQ_PENDING_CLEAR(irq_number)  NVIC_ClearPendingIRQ(irq_number)

#define NRFX_IRQ_IS_PENDING(irq_number)  NVIC_GetPendingIRQ(irq_number)

#define NRFX_CRITICAL_SECTION_ENTER()  __disable_irq()

#define NRFX_CRITICAL_SECTION_EXIT()   __enable_irq()

//------------------------------------------------------------------------------

#define NRFX_DELAY_DWT_BASED 0

static inline void nrf_delay_us(uint32_t number_of_us)
{
    // Simple delay loop implementation
    volatile uint32_t i;
    // Approx loop count for 64MHz, very rough estimate
    for (i = 0; i < number_of_us * 20; i++) {
        __asm__("nop");
    }
}
#define NRFX_DELAY_US(us_time)  nrf_delay_us(us_time)

#ifndef NRFX_CTZ
#define NRFX_CTZ(x) __builtin_ctz(x)
#endif

//------------------------------------------------------------------------------

typedef volatile uint32_t nrfx_atomic_t;
#define NRFX_ATOMIC_T               nrfx_atomic_t

#define NRFX_ATOMIC_FETCH_STORE(p_data, value)  __atomic_exchange_n(p_data, value, __ATOMIC_SEQ_CST)

#define NRFX_ATOMIC_FETCH_OR(p_data, value)     __atomic_fetch_or(p_data, value, __ATOMIC_SEQ_CST)

#define NRFX_ATOMIC_FETCH_AND(p_data, value)    __atomic_fetch_and(p_data, value, __ATOMIC_SEQ_CST)

#define NRFX_ATOMIC_FETCH_XOR(p_data, value)    __atomic_fetch_xor(p_data, value, __ATOMIC_SEQ_CST)

#define NRFX_ATOMIC_FETCH_ADD(p_data, value)    __atomic_fetch_add(p_data, value, __ATOMIC_SEQ_CST)

#define NRFX_ATOMIC_FETCH_SUB(p_data, value)    __atomic_fetch_sub(p_data, value, __ATOMIC_SEQ_CST)

#define NRFX_ATOMIC_CAS(p_data, old_value, new_value) \
    __atomic_compare_exchange_n(p_data, &old_value, new_value, false, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST)

//------------------------------------------------------------------------------

#define NRFX_CUSTOM_ERROR_CODES 0

#define NRFX_EVENT_READ(p_event)  (*(volatile uint32_t *)(p_event))

//------------------------------------------------------------------------------

#ifdef __cplusplus
}
#endif

#endif // NRFX_GLUE_H__
