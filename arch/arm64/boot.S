.section .text.boot
.global _start
.type _start, %function

_start:
    // 设置栈
    ldr x0, =stack_top
    mov sp, x0

    // 设置异常向量表
    ldr x0, =exception_vector
    msr VBAR_EL1, x0
    isb

    // 清零 BSS 段
    ldr x0, =__bss_start
    ldr x1, =__bss_end
1:
    cmp x0, x1
    b.hs 2f
    str xzr, [x0], #8
    b 1b
2:

    // 进入 C++
    bl kernel_main

1:
    wfe
    b 1b

.size _start, . - _start