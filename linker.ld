ENTRY(_start)

KERNEL_VBASE = 0xFFFF000000000000;

SECTIONS
{
    __mem_start = 0x40000000;

    /* 引导段：VMA = LMA = 物理 */
    . = 0x40080000;
    __boot_start = .;
    .boot : {
        KEEP(*(.boot.text))        /* _start 必须排最前 */
        . = ALIGN(16);
        KEEP(*(.boot.data))        /* 静态页表 */
        . = ALIGN(16);
        __boot_stack_bottom = .;
        . += 0x4000;               /* 16KB 引导栈 */
        __boot_stack_top = .;
    }
    . = ALIGN(4096);
    __boot_end = .;

    /* 内核主体：高半 VMA，LMA 物理连续 */
    . = . + KERNEL_VBASE;
    __kernel_start = .;

    /* 代码实际放在 0x40084000，但是链接时认为它运行在 0xFFFF000040084000 */
    .text : AT(ADDR(.text) - KERNEL_VBASE) {
        __text_start = .;
        *(.text*)
        __text_end = .;
    }
    .rodata ALIGN(4096) : AT(ADDR(.rodata) - KERNEL_VBASE) {
        __rodata_start = .;
        *(.rodata*)
        *(.data.rel.ro*)
        __rodata_end = .;
    }
    .data ALIGN(4096) : AT(ADDR(.data) - KERNEL_VBASE) {
        __data_start = .;
        *(.data*)
        __data_end = .;
    }
    .init_array ALIGN(8) : AT(ADDR(.init_array) - KERNEL_VBASE) {
        __init_array_start = .;
        KEEP(*(SORT(.init_array.*)))
        KEEP(*(.init_array))
        __init_array_end = .;
    }
    .driver_init ALIGN(8) : AT(ADDR(.driver_init) - KERNEL_VBASE) {
        __driver_init_start = .;
        KEEP(*(SORT(.driver_init.*)))
        KEEP(*(.driver_init))
        __driver_init_end = .;
    }
    .bss ALIGN(4096) : AT(ADDR(.bss) - KERNEL_VBASE) {
        __bss_start = .;
        *(.bss*)
        *(COMMON)
        __bss_end = .;
    }

    . = ALIGN(4096);
    stack_bottom = .;
    . += 0x4000;
    stack_top = .;

    __kernel_end = .;

    /DISCARD/ : {
        *(.eh_frame)
        *(.eh_frame_hdr)
        *(.comment)
    }
}
