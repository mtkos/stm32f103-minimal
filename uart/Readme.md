# the uart program

This program uses the (maximal for this MCU) 72MHz clock frequency. This is done by using the 8MHz HSI (high speed internal clock), and multiplying the frequency by 9 using the PLL.
There is one little subtlety: to use this frequency, flash memory needs to be configured with two wait states. This is documented in section 3.3.3 of [RM0008].
