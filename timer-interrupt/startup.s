.set TIM2_IRQn, 44
.section .init

.word _estack
.word prog
.skip 4*(TIM2_IRQn - 2)
.word TIM2_IRQHandler
