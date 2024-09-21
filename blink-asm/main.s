.set RCC, 0x40021000
.set APB2ENR, 0x18
.set IOPCEN, 1<<4
.set GPIOC, 0x40011000
.set CRH, 0x4
.set ODR, 0xc
.set PIN, 13
.set PIN_Clear, ~((0xf)<<(4*(PIN - 8)))
.set ResetValue, 0x44444444
.set Pin_Output_PP_2MHz, (0x2)<<(4*(PIN - 8))

.word _estack
.word prog + 1

prog:
ldr r0, = RCC
mov r1, # IOPCEN
str r1, [r0, # APB2ENR]

ldr r0, = GPIOC
ldr r1, = (ResetValue & PIN_Clear) | Pin_Output_PP_2MHz
str r1, [r0, # CRH]

1:
ldr r1, = (1<<PIN)
ldr r2, [r0, # ODR]
eor r2, r1
str r2, [r0, # ODR]

ldr r1, = 1000000
2:
sub r1, # 1
cmp r1, # 0
bne 2b

b 1b
