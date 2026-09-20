#pragma once

#include <arch/exception_context.h>

static inline void arch_breakpoint(void)
{
    asm volatile("brk #0");
}

static inline void arch_wait_for_interrupt(void)
{
    asm volatile("wfe");
}

extern "C" void arch_jump_with_context(ExceptionContext *ctx);

static inline void arch_jump_to_user_mode(uint64_t entry_point, uint64_t user_stack)
{
    ExceptionContext ctx{};
    ctx.elr = entry_point;
    ctx.sp = user_stack;
    ctx.spsr = 0x0; // M[3:0]=0b0000 -> EL0t，DAIF 全部不屏蔽
    arch_jump_with_context(&ctx);
}