#pragma once

#include <arch/exception_context.h>

extern "C" void arch_jump_with_context(ExceptionContext *ctx);

namespace arch {

    // 触发一个断点异常，通常用于调试
    static inline void breakpoint(void)
    {
        asm volatile("brk #0");
    }

    // 等待中断，进入低功耗状态，直到有中断发生
    static inline void wait_for_interrupt(void)
    {
        asm volatile("wfe");
    }

    // 跳转到用户态执行，entry_point 是用户程序入口地址，user_stack 是用户栈顶地址
    static inline void jump_to_user_mode(uint64_t entry_point, uint64_t user_stack)
    {
        ExceptionContext ctx{};
        ctx.elr = entry_point;
        ctx.sp = user_stack;
        ctx.spsr = 0x0; // M[3:0]=0b0000 -> EL0t，DAIF 全部不屏蔽
        arch_jump_with_context(&ctx);
    }
}
