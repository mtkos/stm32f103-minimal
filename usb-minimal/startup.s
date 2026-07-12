.set USB_LP_CAN1_RX0_IRQn, 36
.section .init

.word _estack
.word prog
.skip 4*(USB_LP_CAN1_RX0_IRQn - 2)
.word USB_LP_CAN1_RX0_IRQ_Handler
