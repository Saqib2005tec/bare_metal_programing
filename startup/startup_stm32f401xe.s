.syntax unified
.cpu cortex-m4
.thumb

.global _estack
.global Reset_Handler

.extern _sidata
.extern _sdata
.extern _edata
.extern _sbss
.extern _ebss
.extern main


/* Vector table */

.section .isr_vector, "a", %progbits

.word _estack
.word Reset_Handler

.word NMI_Handler
.word HardFault_Handler
.word MemManage_Handler
.word BusFault_Handler
.word UsageFault_Handler

.word 0
.word 0
.word 0
.word 0

.word SVC_Handler
.word DebugMon_Handler

.word 0

.word PendSV_Handler
.word SysTick_Handler


/* Reset handler */

.section .text.Reset_Handler
.type Reset_Handler, %function

Reset_Handler:

    /* Copy .data from Flash to RAM */

    ldr r0, =_sidata
    ldr r1, =_sdata
    ldr r2, =_edata

data_copy:

    cmp r1, r2
    bcc data_copy_loop
    b bss_init

data_copy_loop:

    ldr r3, [r0]
    str r3, [r1]

    adds r0, r0, #4
    adds r1, r1, #4

    b data_copy


/* Clear .bss */

bss_init:

    ldr r1, =_sbss
    ldr r2, =_ebss

    movs r3, #0

bss_loop:

    cmp r1, r2
    bcc bss_clear_loop
    b main

bss_clear_loop:

    str r3, [r1]

    adds r1, r1, #4

    b bss_loop


/* Default interrupt handler */

.section .text.Default_Handler
.type Default_Handler, %function

Default_Handler:

    b Default_Handler


/* Weak exception handlers */

.weak NMI_Handler
.thumb_set NMI_Handler, Default_Handler

.weak HardFault_Handler
.thumb_set HardFault_Handler, Default_Handler

.weak MemManage_Handler
.thumb_set MemManage_Handler, Default_Handler

.weak BusFault_Handler
.thumb_set BusFault_Handler, Default_Handler

.weak UsageFault_Handler
.thumb_set UsageFault_Handler, Default_Handler

.weak SVC_Handler
.thumb_set SVC_Handler, Default_Handler

.weak DebugMon_Handler
.thumb_set DebugMon_Handler, Default_Handler

.weak PendSV_Handler
.thumb_set PendSV_Handler, Default_Handler

.weak SysTick_Handler
.thumb_set SysTick_Handler, Default_Handler