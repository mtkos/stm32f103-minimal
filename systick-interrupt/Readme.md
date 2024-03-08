# the systick-interrupt program

Compared to ch32v003, there are two differences. Firstly, by default the Cortex-M3 expects the vector table to be located at address 0. This is the default value of the VTOR register, documented
in section 4.3 of [Cortex-M3]. When the BOOT0 pin is low, flash memory, which starts at address 0x08000000, is also accesible at address 0. However, when starting the program from the bootloader
(BOOT0 is high), the program explicitly needs to be told that flash (and so also the vector table) is to be found at address 0x08000000. This value is defined in the linker script (\_flash\_base)
and stored in the VTOR register.

Secondly, the SysTick interrupt is enabled automatically. Like the other core interrupts (exceptions), they do not have to (and cannot) be enabled in the NVIC. They are documented in [Cortex-M3],
section 2.3.2. The SysTick timer itself is documented in section 4.4.
