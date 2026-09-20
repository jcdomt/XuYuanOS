#include <arch/exception_context.h>
#include <kernel/syscall.h>

// AArch64 系统调用 ABI: x8 = 调用号, x0-x2 = 参数, x0 = 返回值
void arch_syscall_handler(ExceptionContext *context)
{
    int64_t ret = syscall_dispatch(context->x[8],
                                   context->x[0],
                                   context->x[1],
                                   context->x[2]);

    context->x[0] = static_cast<uint64_t>(ret);
}
