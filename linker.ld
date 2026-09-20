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
        *(.data.rel.ro*)
    }

    .data : ALIGN(4096)
    {
        *(.data*)
    }

    /* C++ 全局构造函数表 */
    .init_array : ALIGN(8)
    {
        __init_array_start = .;
        KEEP(*(SORT(.init_array.*)))
        KEEP(*(.init_array))
        __init_array_end = .;
    }

    /* 驱动初始化函数表：由各驱动通过 DRIVER_INIT 注册 */
    .driver_init : ALIGN(8)
    {
        __driver_init_start = .;
        KEEP(*(SORT(.driver_init.*)))
        KEEP(*(.driver_init))
        __driver_init_end = .;
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

    /* 用户态数据（可写） */
    .user_data : ALIGN(4096)
    {
        *(.user_data*)
    }

    /* 用户栈空间 */
    user_stack_bottom = .;
    . += 16384;
    user_stack_top = .;
}