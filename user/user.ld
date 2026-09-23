ENTRY(_user_start)
SECTIONS {
    . = 0x0000000000400000;   /* USER_BASE，低半任意选一块 */
    .text   : { *(.text*) }
    .rodata : { 
        *(.rodata*)
        *(.user_image)
    }
    .data   : { *(.data*) }
    .bss    : { *(.bss*) *(COMMON) }
}