#define RCC 0x40021000
#define   CR 0x0
#define     PLLRDY 1<<25
#define     PLLON 1<<24
#define     HSEON 1<<16
#define   CFGR 0x4
#define     HPRE_Reset_Mask 0xFFFFFF0F
#define     HPRE_DIV1 0
#define     PLL_MUL9 (0b0111 << 18)
#define     PLL_MUL12 (0b1010 << 18)
#define     PLLSRC 1<<16
#define     SW_PLL 0x2
#define     SW_HSE 0x1
#define   AHBENR 0x14
#define     DMA1EN 1<<0
#define   APB2ENR 0x18
#define     USART1EN 1<<14
#define     TIM1EN 1<<11
#define     IOPCEN 1<<4
#define     IOPBEN 1<<3
#define     IOPAEN 1<<2
#define     AFIOEN 1<<0
#define   APB1ENR 0x1c
#define     TIM2EN 1<<0
