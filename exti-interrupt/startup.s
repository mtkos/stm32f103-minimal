.set EXTI0_IRQn, 22
.section .init

.word _estack
.word prog
.skip 4*(EXTI0_IRQn - 2)
.word EXTI0_IRQHandler
