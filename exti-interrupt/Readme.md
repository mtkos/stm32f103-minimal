# the exti-interrupt program

Compared to ch32v003 there is a small difference in how interrupt numbers are used.
For example, the EXTI0 interrupt has interrupt number 6. However, the vector table table entry is at position 6 + 16 = 22. This is because there are 16 core interrupts (exceptions), which occupy the 
first 16 words of the vector table, and which do not need the NVIC. See [ARMv7-M], section B1.5.2 and B1.5.3. So the startup code uses value 22.
