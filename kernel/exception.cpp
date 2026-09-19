#include <kernel/exception.h>
#include <driver/uart/uart.h>

void exception_sync_handler()
{
    uart_puts("Sync Exception!\n");
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
