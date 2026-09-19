#include <arch/arch.h>
#include <driver/uart/uart.h>

extern "C" void kernel_main()
{
    uart_puts("XuYuanOS!\n");

    // 手动触发一个异常
    arch_breakpoint();

    uart_puts("After exception\n");

    while (true) {
        arch_wait_for_interrupt();
    }
}
