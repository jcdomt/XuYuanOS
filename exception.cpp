#include "uart.h"

static inline unsigned long read_esr_el1() {
    unsigned long value;
    asm volatile("mrs %0, esr_el1" : "=r"(value));
    return value;
}

static inline unsigned long read_elr_el1() {
    unsigned long value;
    asm volatile("mrs %0, elr_el1" : "=r"(value));
    return value;
}

static inline unsigned long read_spsr_el1() {
    unsigned long value;
    asm volatile("mrs %0, spsr_el1" : "=r"(value));
    return value;
}

extern "C" void exception_sync_handler() {
    uart_puts("Sync Exception!\n");

    unsigned long esr = read_esr_el1();
    unsigned long elr = read_elr_el1();
    unsigned long spsr = read_spsr_el1();

    // 暂时只是为了让程序能编译
    (void)esr;
    (void)elr;
    (void)spsr;

     while (true) {
        asm volatile("wfe");
    }
}

extern "C"
void exception_irq_handler()
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