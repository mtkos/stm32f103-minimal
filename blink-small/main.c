#include<stdint.h>

#include "../include/common.h"
#include "../include/rcc.h"
#include "../include/gpio.h"

#define PIN 13

void prog(){
    REG(RCC + APB2ENR) = IOPCEN;

    REG(GPIOC + CRH) &= Clear(PIN - 8);
    REG(GPIOC + CRH) |= Output_PP_2MHz(PIN - 8);

    while(1){
        for(volatile uint32_t i = 100000; i; i--);
        REG(GPIOC + ODR) ^= Pin(PIN);
    }
}
