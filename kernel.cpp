#include "uart.h"

extern "C" void kernel_main()
{
    uart_puts("Kernel Main!\n");

    // 手动触发一个异常
    asm volatile("brk #0");

    uart_puts("This should not execute\n");

    while (true) {
        asm volatile("wfe");
    }
}