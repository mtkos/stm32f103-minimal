#include <stdint.h>

#include "../include/common.h"
#include "../include/rcc.h"
#include "../include/dma.h"
#include "../include/gpio.h"
#include "../include/timer.h"

extern uint32_t _sidata, _sdata, _edata, _ebss, _sbss;

void flash_to_ram(){

    uint32_t *p_idata = &_sidata;

    for(uint32_t *p_data = &_sdata; p_data < &_edata; *p_data++ = *p_idata++);
    for(uint32_t *p_bss = &_sbss; p_bss < &_ebss; *p_bss++ = 0);
}

#define BIT0 2 //8MHzclock -> 0.25 us high
#define BIT1 8 //1 us high
#define CNT_MAX 10 //1.25 us pulse length
#define NUM_LEDS 3
#define SIZE NUM_LEDS*24 + 1

uint8_t bits[SIZE];

void set_pixel(int led, uint32_t rgb){

    uint32_t bit;
    int i;

    i = 24*led;
    for(bit = 1<<23; bit; bit >>= 1)
        bits[i++] = (rgb & bit) ? BIT1 : BIT0;
}

#define PIN2 2

void init_clocks(){

    REG(RCC + APB2ENR) = IOPAEN;
    REG(RCC + APB1ENR) = TIM2EN;
    REG(RCC + AHBENR) = DMA1EN;
}

void dma_init(){

    REG(DMA1 + CPAR2) = TIM2 + CCR3;
    REG(DMA1 + CMAR2) = (uint32_t)bits;
    REG(DMA1 + CCR2) = PSIZE16 | MINC | DIR;
}

void timer_init(){

    REG(GPIOA + CRL) &= Clear(PIN2);
    REG(GPIOA + CRL) |= Multiplexed_PP_2MHz(PIN2);

    REG(TIM2 + CCER) = CC3E;
    REG(TIM2 + CCMR2) = OC3M_2 | OC3M_1 | OC3PE;

    REG(TIM2 + ARR) = CNT_MAX;

    REG(TIM2 + DIER) = UDE;
    REG(TIM2 + CR1) = CEN;
}

void send_bits(){

    REG(DMA1 + CCR2) &= ~(EN);
    REG(DMA1 + CNDTR2) = SIZE;
    REG(DMA1 + CCR2) |= EN;
}

uint32_t colors[NUM_LEDS] = {0x080000, 0x000800, 0x000008};

void cycle(){

    uint32_t c = colors[0];
    for(int i = 0; i < NUM_LEDS - 1; i++)
        colors[i] = colors[i + 1];
    colors[NUM_LEDS - 1] = c;
}

void prog(){

    flash_to_ram();
    init_clocks();
    dma_init();
    timer_init();

    while(1){
        for(int i = 0; i < NUM_LEDS; i++)
            set_pixel(i, colors[i]);
        send_bits();
        cycle();
        for(int i = 1000000; i; i--);
    }
}
