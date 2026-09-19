#ifndef __EXCEPTION_CPP__
#define __EXCEPTION_CPP__

#include <driver/uart/uart.h>
#include <kernel/exception.h>


extern "C" void exception_sync_handler(ExceptionContext *context) {
    uart_puts("Sync Exception!\n");

    uart_puts("ESR_EL1: ");
    uart_puthex(context->esr);
    uart_puts("\nELR_EL1: ");
    uart_puthex(context->elr);
    uart_puts("\nSPSR_EL1: ");
    uart_puthex(context->spsr);
    uart_puts("\n");

    // 手动修改异常返回地址，跳过异常指令
    context->elr += 4;
}

extern "C" void exception_irq_handler()
{
    uart_puts("IRQ Exception!\n");

    while (true) {
        asm volatile("wfe");
    }
}


extern "C" void exception_fiq_handler()
{
    uart_puts("FIQ Exception!\n");

    while (true) {
        asm volatile("wfe");
    }
}

extern "C" void exception_serror_handler()
{
    uart_puts("SError Exception!\n");

    while (true) {
        asm volatile("wfe");
    }
}

#endif // __EXCEPTION_CPP__