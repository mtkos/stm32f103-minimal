#include <stdint.h>

#include "../include/common.h"
#include "../include/rcc.h"
#include "../include/gpio.h"

#define PIN13 13
#define PIN14 14

uint32_t count = 1000;

extern uint32_t _sidata, _sdata, _edata, _ebss, _sbss;

uint32_t period(){

    count += 1000;
    return count;
}

void flash_to_ram(){

    uint32_t *p_idata = &_sidata;

    for(uint32_t *p_data = &_sdata; p_data < &_edata; *p_data++ = *p_idata++);
    for(uint32_t *p_bss = &_sbss; p_bss < &_ebss; *p_bss++ = 0);

}

void prog(){

    flash_to_ram();

    REG(RCC + APB2ENR) = IOPCEN;

    REG(GPIOC + CRH) &= Clear(PIN13 - 8);
    REG(GPIOC + CRH) |= Output_PP_2MHz(PIN13 - 8);
    REG(GPIOC + CRH) &= Clear(PIN14 - 8);
    REG(GPIOC + CRH) |= Output_PP_2MHz(PIN14 - 8);

//    REG(GPIOC + ODR) = Pin(PIN13);
    while(1){
        for(volatile uint32_t i = period(); i; i--);
        REG(GPIOC + ODR) ^= Pin(PIN13) | Pin(PIN14);
    }
}
