#include <arch/arch.h>
 
#include <driver/uart/uart.h>
#include <kernel/usermode.h>


extern "C" void kernel_main()
{
    uart_puts("XuYuanOS!\n");

    // 链接脚本符号必须用其地址（声明为数组可避免误用）
    extern char __user_text_start[];
    extern char user_stack_top[];
    uart_puts("Jump to user mode...");
    uart_puthex(reinterpret_cast<uint64_t>(__user_text_start));
    uart_puts("\n");
    jump_to_user_mode(reinterpret_cast<uint64_t>(__user_text_start),
                      reinterpret_cast<uint64_t>(user_stack_top));


    while (true) {
        arch_wait_for_interrupt();
    }
}
