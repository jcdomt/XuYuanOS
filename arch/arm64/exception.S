.section .text

.global exception_vector
.type exception_vector, %function

// ARM 要求中断向量表需要 2048 字节对齐
// 也就是 16 个 128 字节的异常向量表
.align 11
exception_vector:

    // Current EL with SP0 - Synchronous
    b sync_exception

    // Current EL with SP0 - IRQ
    .balign 128
    b irq_exception

    // Current EL with SP0 - FIQ
    .balign 128
    b fiq_exception

    // Current EL with SP0 - SError
    .balign 128
    b serror_exception


    // Current EL with SPx - Synchronous
    .balign 128
    b sync_exception

    // Current EL with SPx - IRQ
    .balign 128
    b irq_exception

    // Current EL with SPx - FIQ
    .balign 128
    b fiq_exception

    // Current EL with SPx - SError
    .balign 128
    b serror_exception


    // Lower EL using AArch64 - Synchronous
    .balign 128
    b sync_exception

    // Lower EL using AArch64 - IRQ
    .balign 128
    b irq_exception

    // Lower EL using AArch64 - FIQ
    .balign 128
    b fiq_exception

    // Lower EL using AArch64 - SError
    .balign 128
    b serror_exception


    // Lower EL using AArch64 - Synchronous
    .balign 128
    b sync_exception

    // Lower EL using AArch64 - IRQ
    .balign 128
    b irq_exception

    // Lower EL using AArch64 - FIQ
    .balign 128
    b fiq_exception

    // Lower EL using AArch64 - SError
    .balign 128
    b serror_exception


sync_exception:
    bl exception_sync_handler

1:
    wfe
    b 1b

irq_exception:
    bl exception_irq_handler

1:
    wfe
    b 1b

fiq_exception:
    bl exception_fiq_handler

1:
    wfe
    b 1b

serror_exception:
    bl exception_serror_handler

1:
    wfe
    b 1b