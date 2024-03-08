#include <stdint.h>

#include "../include/common.h"
#include "../include/rcc.h"
#include "../include/gpio.h"
#include "../include/timer.h"

#define PIN2 2

#define CPU_FREQ 8000000
#define CNT_MAX 0xffff
#define PRESCALER_VALUE (CPU_FREQ/CNT_MAX - 1)

void prog(){

    REG(RCC + APB2ENR) = IOPAEN;
    REG(RCC + APB1ENR) = TIM2EN;

    REG(GPIOA + CRL) &= Clear(PIN2);
    REG(GPIOA + CRL) |= Multiplexed_PP_2MHz(PIN2);

    REG(TIM2 + CCER) = CC3E; //Enable Timer 1 Channel 3 output
    REG(TIM2 + CCMR2) = OC3M_2 | OC3M_1; //PWM mode 1
    REG(TIM2 + CCR3) = CNT_MAX/10;

    REG(TIM2 + PSC) = PRESCALER_VALUE;
    REG(TIM2 + CR1) = CEN;

    while(1){
        __asm__ volatile("wfi");
    }
}
