#include <driver/uart/uart.h>

extern "C" void kernel_main()
{
    uart_puts("XuYuanOS!\n");

    // 手动触发一个异常
    asm volatile("brk #0");

    uart_puts("After exception\n");

    while (true) {
        asm volatile("wfe");
    }
}