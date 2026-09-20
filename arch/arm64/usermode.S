.global arch_jump_with_context
.type arch_jump_with_context, %function

arch_jump_with_context:
    // x0 = ExceptionContext *

    // SP_EL0: offset 248
    ldr x1, [x0, #248]
    msr sp_el0, x1

    // ELR_EL1: offset 256
    ldr x1, [x0, #256]
    msr elr_el1, x1

    // SPSR_EL1: offset 264
    ldr x1, [x0, #264]
    msr spsr_el1, x1

    // x0 保存着参数栈指针，所以要最后修改
    ldp x2,  x3,  [x0, #0]      // x0,  x1
    ldp x4,  x5,  [x0, #16]     // x2,  x3
    ldp x6,  x7,  [x0, #32]     // x4,  x5
    ldp x8,  x9,  [x0, #48]     // x6,  x7
    ldp x10, x11, [x0, #64]     // x8,  x9
    ldp x12, x13, [x0, #80]     // x10, x11
    ldp x14, x15, [x0, #96]     // x12, x13
    ldp x16, x17, [x0, #112]    // x14, x15
    ldp x18, x19, [x0, #128]    // x16, x17
    ldp x20, x21, [x0, #144]    // x18, x19
    ldp x22, x23, [x0, #160]    // x20, x21
    ldp x24, x25, [x0, #176]    // x22, x23
    ldp x26, x27, [x0, #192]    // x24, x25
    ldp x28, x29, [x0, #208]    // x26, x27
    ldr x30, [x0, #240]
    ldp x0, x1, [x0, #0]

    // EL1 -> EL0
    eret

.size arch_jump_with_context, . - arch_jump_with_context