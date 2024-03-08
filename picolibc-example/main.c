#include <stdio.h>
#include <math.h>

#include "../include/common.h"
#include "../include/flash.h"
#include "../include/rcc.h"
#include "../include/gpio.h"
#include "../include/uart.h"

#define BAUDRATE 921600
#define CLOCK_FREQUENCY 72000000
#define BAUD_REG (CLOCK_FREQUENCY/BAUDRATE - 1)

#define PIN9 9
#define PIN10 10

int uart_putc(char c, FILE *file){

    REG(USART1 + DR) = c;
    while(!(REG(USART1 + SR) & TC));
    return c;
}

int uart_getc(FILE *file){

    unsigned char c;

    while(!(REG(USART1 + SR) & RXNE));
    c = REG(USART1 + DR);
    return c;
}

FILE __stdio = FDEV_SETUP_STREAM(uart_putc, uart_getc, NULL, _FDEV_SETUP_RW);

FILE *const stdin = &__stdio;
__strong_reference(stdin, stdout);
__strong_reference(stdin, stderr);

void uart_init(){

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
}

void main(){

    double x = 0.0;

    uart_init();

    while(1){
        getchar();
        printf("%5.1f %19.16f\r\n", x, sin(x));
        x += 0.1;
    }
}
