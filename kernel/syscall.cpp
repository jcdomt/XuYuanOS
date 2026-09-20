#include <stdint.h>
#include <cstddef>

#include <kernel/syscall.h>

#include <driver/char_driver.h>

// 架构无关的系统调用分发
int64_t syscall_dispatch(uint64_t nr, uint64_t arg0, uint64_t arg1, uint64_t arg2)
{
    (void)arg2;

    CharDeviceDriver *out = CharDeviceDriver::Default();

    switch (static_cast<SyscallNumber>(nr)) {
        case SyscallNumber::WRITE:
        {
            // arg0 是缓冲区地址，arg1 是长度
            const char *buffer = reinterpret_cast<const char *>(arg0);
            size_t length = static_cast<size_t>(arg1);
            // 返回写入的字节数
            return static_cast<int64_t>(out->write(buffer, length));
        }

        default:
            out->write("Unknown syscall number!\n");
            return -1;
    }
}
