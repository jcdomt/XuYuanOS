#pragma once

#include <stdint.h>

enum class ExceptionType : uint64_t{
    SYNC = 0,
    IRQ = 1,
    FIQ = 2,
    SERROR = 3
};

// 通用异常处理钩子，由 arch 层解码后调用
void exception_sync_handler();
void exception_irq_handler();
void exception_fiq_handler();
void exception_serror_handler();
