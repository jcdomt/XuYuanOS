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
    /*
    uint64_t x[31];

    uint64_t sp;
    uint64_t elr;
    uint64_t spsr;
    uint64_t esr;
    */
    sub sp, sp, #288

    // 保存 x0-x30 寄存器
    stp x0, x1, [sp, #16 * 0]
    stp x2, x3, [sp, #16 * 1]
    stp x4, x5, [sp, #16 * 2]
    stp x6, x7, [sp, #16 * 3]
    stp x8, x9, [sp, #16 * 4]
    stp x10, x11, [sp, #16 * 5]
    stp x12, x13, [sp, #16 * 6]
    stp x14, x15, [sp, #16 * 7]
    stp x16, x17, [sp, #16 * 8]
    stp x18, x19, [sp, #16 * 9]
    stp x20, x21, [sp, #16 * 10]
    stp x22, x23, [sp, #16 * 11]
    stp x24, x25, [sp, #16 * 12]
    stp x26, x27, [sp, #16 * 13]
    stp x28, x29, [sp, #16 * 14]
    str x30, [sp, #16 * 15] // 240

    // SP
    add x0, sp, #288
    str x0, [sp, #16 * 15 + 8] // 248

    // ELR
    mrs x0, ELR_EL1
    str x0, [sp, #16 * 15 + 16] // 256

    // SPSR
    mrs x0, SPSR_EL1
    str x0, [sp, #16 * 15 + 24] // 264

    // ESR
    mrs x0, ESR_EL1
    str x0, [sp, #16 * 15 + 32] // 272

    mov x0, sp

    bl exception_sync_handler

    ldp x0, x1, [sp, #16 * 0]
    ldp x2, x3, [sp, #16 * 1]
    ldp x4, x5, [sp, #16 * 2]
    ldp x6, x7, [sp, #16 * 3]
    ldp x8, x9, [sp, #16 * 4]
    ldp x10, x11, [sp, #16 * 5]
    ldp x12, x13, [sp, #16 * 6]
    ldp x14, x15, [sp, #16 * 7]
    ldp x16, x17, [sp, #16 * 8]
    ldp x18, x19, [sp, #16 * 9]
    ldp x20, x21, [sp, #16 * 10]
    ldp x22, x23, [sp, #16 * 11]
    ldp x24, x25, [sp, #16 * 12]
    ldp x26, x27, [sp, #16 * 13]

    // 恢复 ELR
    ldr x0, [sp, #16 * 15 + 16] // 256
    msr ELR_EL1, x0

    // 恢复 SPSR
    ldr x0, [sp, #16 * 15 + 24] // 264
    msr SPSR_EL1, x0


    ldp x28, x29, [sp, #16 * 14]
    ldr x30, [sp, #16 * 15] // 240

    // SP
    add sp, sp, #288

    eret

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
