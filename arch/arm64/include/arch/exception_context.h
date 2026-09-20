#pragma once

#include <stdint.h>


struct ExceptionContext
{
    uint64_t x[31];

    uint64_t sp;    // 248
    uint64_t elr;   // 256
    uint64_t spsr;  // 264
    uint64_t esr;   // 272
};
// 31*8 + 8*4 = 248 + 32 = 280 bytes
static_assert(sizeof(ExceptionContext) == 280);
