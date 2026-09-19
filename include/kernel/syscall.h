#pragma once
#include <stdint.h>

enum class SyscallNumber : uint64_t {
    WRITE = 1,
};

// 架构无关的系统调用分发；寄存器读写由 arch 层完成
int64_t syscall_dispatch(uint64_t nr, uint64_t arg0, uint64_t arg1, uint64_t arg2);
