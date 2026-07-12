#include <stdint.h>

#include "../include/common.h"
#include "../include/rcc.h"
#include "../include/nvic.h"
#include "../include/flash.h"
#include "../include/gpio.h"
#include "../include/uart.h"
#include "../include/usb.h"

#define BAUDRATE 921600
#define CLOCK_FREQUENCY 48000000
#define BAUD_REG CLOCK_FREQUENCY/BAUDRATE - 1

#define PIN9 9 //PA9 = TX
#define PIN10 10 //PA10 = RX

#define USB_LP_CAN1_RX0_IRQn 20

uint8_t ctrl_request[8];

void clocks_init(){ //set system and usb clock to 48MHz

    REG(FLASH + ACR) |= ONE_WAIT_STATE;
    REG(RCC + CFGR) = USBPRE | PLLSRC | SW_PLL | PLL_MUL6;
    REG(RCC + CR) = PLLON | HSEON;
    while(!(REG(RCC + CR) & PLLRDY));
}

void uart_init(){

    REG(RCC + APB2ENR) = USART1EN | IOPAEN;

    REG(GPIOA + CRH) &= Clear(PIN9 - 8);
    REG(GPIOA + CRH) |= Multiplexed_PP_2MHz(PIN9 - 8);

    REG(GPIOA + CRH) &= Clear(PIN10 - 8);
    REG(GPIOA + CRH) |= Input_PUPD(PIN10 - 8);

    REG(USART1 + BRR) = BAUD_REG;
    REG(USART1 + CR1) = TE | RE | UE;
}

void uart_send(uint8_t c){

    while(!(REG(USART1 + SR) & TC));
    REG(USART1 + DR) = c;
};

void usb_init(){

    REG(RCC + APB1ENR) = USBEN;

    REG(USB_PMA + 0*16 + ADDR_RX) = 8; //to leave room for btable

    REG(USB + CNTR) = CTRM | RESETM;
}

void handle_reset(){

    REG(USB + ISTR) = 0;
    REG(USB + EPR0) = RX_VALID | CONTROL | TX_NAK | 0;
    REG(USB + DADDR) = EF;
}

void pma_to_buffer(uint8_t ep, uint8_t* buf){

    uint8_t len = REG(USB_PMA + ep*16 + COUNT_RX) & COUNT_BITS;
    uint32_t* pma = (uint32_t *)(USB_PMA + 2*REG(USB_PMA + ep*16 + ADDR_RX));
    for(uint8_t i = 0; 2*i < len; i++)
        ((uint16_t*)buf)[i] = pma[i];
}

void send_zero_length_packet(){

        REG(USB_PMA + 0*16 + COUNT_TX) = 0; //next IN packet must be zero length
}

void handle_ep0_setup(){

    pma_to_buffer(0, ctrl_request);
    for(int i = 0; i < 8; i++)
        uart_send(ctrl_request[i]);
    send_zero_length_packet();

    REG(USB + EPR0) = CONTROL | CTR_TX | ((TX_NAK) ^ (TX_VALID)); //clear RX interrupt flag, change STAT_TX from NAK (reference manual 23.5.2) to VALID, for zero length packet
}

void handle_ep0_in(){

    REG(USB + EPR0) = CTR_RX | ((RX_NAK) ^ (RX_VALID)) | CONTROL;
}

void handle_ep0(){

    switch(REG(USB + EPR0) & (SETUP | CTR_TX)){
    case SETUP:
        handle_ep0_setup();
        break;
    case CTR_TX: //transmitted reply to GET request or zero length packet for SET request
        handle_ep0_in();
    }
}

void USB_LP_CAN1_RX0_IRQ_Handler() __attribute__((interrupt));

void USB_LP_CAN1_RX0_IRQ_Handler(){

    uint16_t istr = REG(USB + ISTR);

    if(istr & RESET)
        handle_reset();
    else
        handle_ep0();
}

void prog(){

    clocks_init();
    usb_init();
    uart_init();

    REG(NVIC + ISER0) = 1<<USB_LP_CAN1_RX0_IRQn;

    while(1){

        __asm__ volatile("wfi");
    }
}
