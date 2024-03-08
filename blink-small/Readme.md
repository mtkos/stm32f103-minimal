# the blink-small program

Unlike its ch32v003 counterpart, this program needs a small piece of startup code, which is best done in assembly. A risc-v processor simply starts executing the first instruction in flash
memory. The ARM Cortex-M3, on the other hand, needs the initial value of the stack pointer at the first memory word: adress 0. The second word (addres 4) contains the address of the function
to be executed on reset, the reset handler. This is documented in section 2.3.4 of [Cortex-M3]. In our case this reset handler is our main program, which we chose to call 'prog'. All of this is
done in the startup.s code. Because this code needs to come first in flash memory we put in in the .init section. The linker script then places this section at the start of flash memory. The
start address of flash memory is not 0. It is 0x08000000, as documented in [RM0008], section 3.3.3. But if pin BOOT0 is pulled low (otherwise the bootloader will be started), flash is aliased
to address 0, and everything works as expected. See [RM0008], section 3.4.
