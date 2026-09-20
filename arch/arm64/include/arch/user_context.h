#pragma once

#include <stdint.h>

struct UserContext {
    uint64_t x[31];

    uint64_t sp;
    uint64_t pc;
    uint64_t pstate;
};
// 8*31 + 8*3 = 272
static_assert(sizeof(UserContext) == 272);