ENTRY(_start)

SECTIONS
{
    /*
        QEMU 会把内存映射到 0x40000000
        把内核放在 0x40080000 处，留出 8MB 的空间给 QEMU
    */
    . = 0x40080000;

    .text : ALIGN(4096)
    {
        KEEP(*(.text.boot))
        *(.text*)
    }

    .rodata : ALIGN(4096)
    {
        *(.rodata*)
    }

    .data : ALIGN(4096)
    {
        *(.data*)
    }

    /* bss 存储全局、静态变量 */
    .bss : ALIGN(4096)
    {
        __bss_start = .;

        *(.bss*)
        *(COMMON)

        __bss_end = .;
    }

    /* 内核栈空间 */
    . = ALIGN(4096);

    stack_bottom = .;
    . += 16384;
    stack_top = .;

    /* 用户态程序 */
    .user_text : ALIGN(4096)
    {
        __user_text_start = .;
        *(.user_text*)
        __user_text_end = .;
    }

    /* 用户栈空间 */
    user_stack_bottom = .;
    . += 16384;
    user_stack_top = .;
}