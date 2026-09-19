#ifndef __EXCEPTION_CPP__
#define __EXCEPTION_CPP__

#include <driver/uart/uart.h>
#include <kernel/exception.h>
#include <kernel/syscall.h>

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
        context->elr += 4;
    }
}

void exception_sync_handler(ExceptionContext *context) {
    uart_puts("Sync Exception!\n");

    uint64_t ec = context->esr >> 26;
    switch (ec) {
        case 0x15:
            syscall_handler(context);
            break;
        case 0x3C:
            
            break;
        default:
            uart_puts("Unknown sync exception!\n");
    }

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