#include <stdint.h>
#include <cstddef>

#include <kernel/syscall.h>
#include <kernel/exception.h>

#include <driver/uart/uart.h>

// 系统调用处理函数
int syscall_handler(ExceptionContext *context) {
    // x8: 系统调用号
    uint64_t syscall_number = context->x[8];
    // x0: 第一个参数
    uint64_t arg0 = context->x[0];
    // x1: 第二个参数
    uint64_t arg1 = context->x[1];
    // x2: 第三个参数
    uint64_t arg2 = context->x[2];
    
    switch (static_cast<SyscallNumber>(syscall_number)) {
        case SyscallNumber::WRITE:
        {
            // 假设 arg0 是缓冲区地址，arg1 是长度
            char *buffer = reinterpret_cast<char *>(arg0);
            size_t length = static_cast<size_t>(arg1);
            for (size_t i = 0; i < length; ++i) {
                uart_putc(buffer[i]);
            }
            // 返回写入的字节数
            context->x[0] = length;
            break;
        }
            
        default:
            uart_puts("Unknown syscall number!\n");
            context->x[0] = -1; // 返回错误码
            return -1;
    }
    return 0;
}