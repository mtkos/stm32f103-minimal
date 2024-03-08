#include<stdint.h>

#include "../include/common.h"
#include "../include/flash.h"
#include "../include/rcc.h"
#include "../include/gpio.h"
#include "../include/uart.h"

#define BAUDRATE 921600
#define CLOCK_FREQUENCY 72000000
#define BAUD_REG CLOCK_FREQUENCY/BAUDRATE - 1

#define PIN9 9 //PA9 = TX
#define PIN10 10 //PA10 = RX

void prog(){

    REG(FLASH + ACR) |= TWO_WAIT_STATES;
    REG(RCC + CFGR) = PLLSRC | SW_PLL | PLL_MUL9;

    REG(RCC + CR) = PLLON | HSEON;
    while(!(REG(RCC + CR) & PLLRDY));

    REG(RCC + APB2ENR) = USART1EN | IOPAEN;

    REG(GPIOA + CRH) &= Clear(PIN9 - 8);
    REG(GPIOA + CRH) |= Multiplexed_PP_2MHz(PIN9 - 8);

    REG(GPIOA + CRH) &= Clear(PIN10 - 8);
    REG(GPIOA + CRH) |= Input_PUPD(PIN10 - 8);

    REG(USART1 + BRR) = BAUD_REG;
    REG(USART1 + CR1) = TE | RE | UE;

    while(1){
        while(!(REG(USART1 + SR) & RXNE));
        REG(USART1 + DR)++;
        while(!(REG(USART1 + SR) & TC));
    }
}
