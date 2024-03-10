# the blink-asm program

The Cortex-M3 instruction set is described in [Cortex-M3]. When you read the program it may be surprising that the second entry of the vector table is 'prog + 1', not prog, as in the blink\-small
example. The reason is that the Cortex-M3 processor works in 'Thumb' mode. It requires function addresses to be odd. In the blink-small example this is taken care of by the compiler, but here we
have to do it ourselves. Unfortunately, I have not been able to locate where this is documented.
