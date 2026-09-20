#include <arch/exception_context.h>
#include <kernel/exception.h>

#include <driver/char_driver.h>

void arch_syscall_handler(ExceptionContext *context);

extern "C" void exception_dispatch(ExceptionType type, ExceptionContext *context);

static void sync_handler(ExceptionContext *context);

extern "C" void exception_dispatch(ExceptionType type, ExceptionContext *context)
{
    switch (type) {
    case ExceptionType::SYNC:
        sync_handler(context);
        break;
    case ExceptionType::IRQ:
        exception_irq_handler();
        context->elr += 4;
        break;
    case ExceptionType::FIQ:
        exception_fiq_handler();
        context->elr += 4;
        break;
    case ExceptionType::SERROR:
        exception_serror_handler();
        context->elr += 4;
        break;
    default:
        ((CharDeviceDriver*)CharDeviceDriver::Class().GetDefault())->write("Unknown exception type!\n");        context->elr += 4;
    }
}

static void sync_handler(ExceptionContext *context)
{
    exception_sync_handler(context);

    uint64_t ec = context->esr >> 26;
    switch (ec) {
        case 0x15: // SVC (AArch64): ELR 已指向 svc 的下一条指令，无需跳过
            arch_syscall_handler(context);
            break;
        case 0x3C: // BRK (AArch64): ELR 指向 brk 指令本身，需跳过
            context->elr += 4;
            break;
        default:
        CharDeviceDriver::Default()->write("Unknown sync exception!\n");
        CharDeviceDriver::Default()->puthex(context->elr);
        CharDeviceDriver::Default()->write("\n");
            context->elr += 4; // 跳过出错指令，避免死循环
    }
}
