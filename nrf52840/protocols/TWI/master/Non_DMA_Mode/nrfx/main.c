#include <stdint.h>
#include <stdbool.h>

#include "nrf.h"

#include "nrfx_twi.h"
#include <drivers/nrfx_errors.h>
#include "hal/nrf_gpio.h"

#define I2C_SCL_PIN         NRF_GPIO_PIN_MAP(0, 15)
#define I2C_SDA_PIN         NRF_GPIO_PIN_MAP(0, 14)

#define BLOCKING_MODE
// #define INTERRUPT_MODE

#define TX
// #define RX
// #define TXRX
// #define TXTX

static nrfx_twi_t m_twi = NRFX_TWI_INSTANCE(0);

void twi_handler(nrfx_twi_evt_t const * p_event, void * p_context)
{
}

extern bool nondet_bool(void);
extern unsigned int nondet_uint(void);

void main() {
    /* Initial values */
    nrfx_twi_config_t config = NRFX_TWI_DEFAULT_CONFIG(I2C_SCL_PIN, I2C_SDA_PIN);
    nrfx_twi_init(&m_twi, &config, 
#if defined(INTERRUPT_MODE)
        twi_handler
#else
        NULL
#endif
    , NULL);
    nrfx_twi_enable(&m_twi);
    
    /* Preconditions */
    uint8_t p_tx[3];
    uint8_t p_tx2[3];
    uint8_t p_rx[3];
    for (int i = 0; i < 3; i++) {
        p_tx[i] = nondet_uint() & 0xFF;
        p_tx2[i] = nondet_uint() & 0xFF;
        p_rx[i] = nondet_uint() & 0xFF;
    }
    nrfx_twi_xfer_desc_t desc =
#if defined(TX)
        NRFX_TWI_XFER_DESC_TX(nondet_uint() & 0xFF, p_tx, nondet_uint() & 0x3);
#elif defined(RX)
        NRFX_TWI_XFER_DESC_RX(nondet_uint() & 0xFF, p_rx, nondet_uint() & 0x3);
#elif defined(TXRX)
        NRFX_TWI_XFER_DESC_TXRX(nondet_uint() & 0xFF, p_tx, nondet_uint() & 0x3, p_rx, nondet_uint() & 0x3);
#elif defined(TXTX)
        NRFX_TWI_XFER_DESC_TXTX(nondet_uint() & 0xFF, p_tx, nondet_uint() & 0x3, p_tx2, nondet_uint() & 0x3);
#endif

    /* FUV */
    nrfx_err_t err_code = nrfx_twi_xfer(&m_twi, &desc, nondet_uint());

    /* Postconditions */
}
