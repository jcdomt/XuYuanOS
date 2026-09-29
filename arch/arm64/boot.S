.section .boot.text
.global _start
.type _start, %function

_start:
    // boot stack 向低地址增长，所以从 top 开始
    ldr x0, =__boot_stack_top
    mov sp, x0

    // 建立页表并开启 MMU
    bl boot_setup_mmu

1:
    wfe
    b 1b

.size _start, . - _start


// 高半区继续执行
// MMU 已开启
.section .text
.global high_entry
.type high_entry, %function

high_entry:
    // 切换到正式内核栈
    ldr x0, =stack_top
    mov sp, x0

    // 清零 BSS
    ldr x0, =__bss_start
    ldr x1, =__bss_end

1:
    cmp x0, x1
    b.hs 2f

    str xzr, [x0], #8
    b 1b

2:
    // 设置异常向量表
    ldr x0, =exception_vector
    msr vbar_el1, x0
    isb

    // 进入 C/C++ 内核
    bl kernel_main

1:
    wfe
    b 1b