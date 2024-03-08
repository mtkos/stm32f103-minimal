#include <stdint.h>

#include "../include/common.h"
#include "../include/scb.h"
#include "../include/nvic.h"
#include "../include/rcc.h"
#include "../include/gpio.h"
#include "../include/exti.h"
#include "../include/afio.h"

#define EXTI0_IRQn 6

#define PIN0 0
#define PIN13 13

extern uint32_t _flash_base;

void EXTI0_IRQHandler() __attribute__((interrupt));

void EXTI0_IRQHandler(){

    REG(EXTI + PR) = Pin(PIN0);
}

void prog(){

    REG(SCB + VTOR) = (uint32_t)&_flash_base;

    REG(RCC + APB2ENR) = IOPAEN | IOPCEN | AFIOEN;

    REG(GPIOA + CRL) &= Clear(PIN0);
    REG(GPIOA + CRL) |= Input_Floating(PIN0);

    REG(GPIOC + CRH) &= Clear(PIN13 - 8);
    REG(GPIOC + CRH) |= Output_PP_2MHz(PIN13 - 8);

    REG(AFIO + EXTICR1) = PA_Exti(PIN0);
    REG(EXTI + IMR) = Pin(PIN0);
    REG(EXTI + RTSR) = Pin(PIN0);

    REG(NVIC + ISER0) = 1<<EXTI0_IRQn;

    while(1){
        __asm__ volatile("wfi");
        REG(GPIOC + ODR) ^= Pin(PIN13);
    }
}
