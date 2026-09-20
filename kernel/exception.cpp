#include <arch/exception_context.h>

#include <kernel/exception.h>
#include <driver/uart/uart.h>

void exception_sync_handler(ExceptionContext *ctx)
{
    uart_puts("Sync Exception!\n");

    uart_puts("Exception Class: ");
    uart_puthex(ctx->esr >> 26);
    uart_puts("\n");
    uart_puts("Instruction Specific Syndrome: ");
    uart_puthex(ctx->esr & 0xFFFFFF);
    uart_puts("\n");
}

void exception_irq_handler()
{
    uart_puts("IRQ Exception!\n");
}

void exception_fiq_handler()
{
    uart_puts("FIQ Exception!\n");
}

void exception_serror_handler()
{
    uart_puts("SError Exception!\n");
}
