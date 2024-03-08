#include <stdint.h>

#include "../include/common.h"
#include "../include/scb.h"
#include "../include/nvic.h"
#include "../include/rcc.h"
#include "../include/gpio.h"
#include "../include/timer.h"

#define TIM2_IRQn 28

#define PIN13 13

#define CPU_FREQ 8000000
#define CNT_MAX 0xffff
#define PRESCALER_VALUE (CPU_FREQ/CNT_MAX - 1)

extern uint32_t _flash_base;

void TIM2_IRQHandler() __attribute__((interrupt));

void TIM2_IRQHandler(){

    REG(TIM2 + SR) = 0;
}

void prog(){

    REG(SCB + VTOR) = (uint32_t)&_flash_base;

    REG(RCC + APB2ENR) = IOPCEN;
    REG(RCC + APB1ENR) = TIM2EN;

    REG(GPIOC + CRH) &= Clear(PIN13 - 8);
    REG(GPIOC + CRH) |= Output_PP_2MHz(PIN13 - 8);

    REG(TIM2 + PSC) = PRESCALER_VALUE;

    REG(TIM2 + DIER) = UIE;
    REG(NVIC + ISER0) = 1<<TIM2_IRQn;

    REG(TIM2 + CR1) = CEN;

    while(1){
        __asm__ volatile ("wfi");
        REG(GPIOC + ODR) ^= Pin(PIN13);
    }
}
