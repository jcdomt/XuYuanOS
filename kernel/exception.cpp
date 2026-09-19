#ifndef __EXCEPTION_CPP__
#define __EXCEPTION_CPP__

#include <driver/uart/uart.h>
#include <kernel/exception.h>

enum class ExceptionType : uint64_t{
    SYNC = 0,
    IRQ = 1,
    FIQ = 2,
    SERROR = 3
};

extern "C" void exception_dispatch(ExceptionType type, ExceptionContext *context);
void exception_sync_handler(ExceptionContext *context);
void exception_irq_handler(ExceptionContext *context);
void exception_fiq_handler(ExceptionContext *context);
void exception_serror_handler(ExceptionContext *context);

extern "C" void exception_dispatch(ExceptionType type, ExceptionContext *context)
{
    switch (type) {
    case ExceptionType::SYNC:
        exception_sync_handler(context);
        break;
    case ExceptionType::IRQ:
        exception_irq_handler(context);
        break;
    case ExceptionType::FIQ:
        exception_fiq_handler(context);
        break;
    case ExceptionType::SERROR:
        exception_serror_handler(context);
        break;
    default:
        uart_puts("Unknown exception type!\n");
        while (true) {
            asm volatile("wfe");
        }
    }
}

void exception_sync_handler(ExceptionContext *context) {
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

void exception_irq_handler(ExceptionContext *context)
{
    uart_puts("IRQ Exception!\n");

    context->elr += 4;
}

void exception_fiq_handler(ExceptionContext *context)
{
    uart_puts("FIQ Exception!\n");

    context->elr += 4;
}

void exception_serror_handler(ExceptionContext *context)
{
    uart_puts("SError Exception!\n");

    context->elr += 4;
}

#endif // __EXCEPTION_CPP__