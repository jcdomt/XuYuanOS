#include <arch/exception_context.h>

#include <kernel/exception.h>
#include <driver/char_driver.h>

void exception_sync_handler(ExceptionContext *ctx)
{
    CharDeviceDriver *out = CharDeviceDriver::Default();

    out->write("Sync Exception!\n");

    out->write("Exception Class: ");
    out->puthex(ctx->esr >> 26);
    out->write("\n");
    out->write("Instruction Specific Syndrome: ");
    out->puthex(ctx->esr & 0xFFFFFF);
    out->write("\n");
}

void exception_irq_handler()
{
    CharDeviceDriver::Default()->write("IRQ Exception!\n");
}

void exception_fiq_handler()
{
    CharDeviceDriver::Default()->write("FIQ Exception!\n");
}

void exception_serror_handler()
{
    CharDeviceDriver::Default()->write("SError Exception!\n");
}
