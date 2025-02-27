.globl HardFault_Handler
.syntax unified

HardFault_Handler:
tst lr, #4
ite eq
mrseq r0, msp
mrsne r0, psp
b HardFault_Handler_C