#pragma once

#include <stdint.h>

struct ExceptionContext
{
    uint64_t x[31];

    uint64_t sp;
    uint64_t elr;
    uint64_t spsr;
    uint64_t esr;
};
// 31*8 + 8*4 = 248 + 32 = 280 bytes
static_assert(sizeof(ExceptionContext) == 280);