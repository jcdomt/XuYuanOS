#pragma once

static inline void arch_breakpoint(void)
{
    asm volatile("brk #0");
}

static inline void arch_wait_for_interrupt(void)
{
    asm volatile("wfe");
}
