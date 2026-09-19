#pragma once
#include <stdint.h>
#include <kernel/exception.h>

enum class SyscallNumber : uint64_t {
    WRITE = 1,
};

int syscall_handler(ExceptionContext *context);