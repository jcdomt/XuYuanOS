ENTRY(_user_start)
SECTIONS {
    . = 0x40000000;   /* USER_BASE，低半任意选一块 */
    .text   : { *(.text*) }
    .rodata : { 
        *(.rodata*)
    }
    .data   : { *(.data*) }
    .bss    : { *(.bss*) *(COMMON) }
}