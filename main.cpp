#include <arch/arch.h>

#include <kernel/mm/pmm.h>

#include <driver/driver.h>
#include <driver/char_driver.h>
#include <kernel/usermode.h>

// 链接脚本导出的 C++ 全局构造函数表
typedef void (*InitFunc)();
extern InitFunc __init_array_start[];
extern InitFunc __init_array_end[];

static void call_static_constructors()
{
    for (InitFunc *f = __init_array_start; f != __init_array_end; ++f)
        (*f)();
}

void test();

extern "C" void kernel_main()
{
    // 先初始化内存管理器
    extern char __mem_start[];
    extern char __kernel_start[];
    extern char __kernel_end[];
    uint64_t mem_start = reinterpret_cast<uint64_t>(__mem_start);
    uint64_t mem_end = mem_start + TOTAL_MEMORY_SIZE;
    uint64_t kernel_start = reinterpret_cast<uint64_t>(__kernel_start);
    uint64_t kernel_end = reinterpret_cast<uint64_t>(__kernel_end);
    mm::pmm_init(mem_start, mem_end, kernel_end);

    // 初始化各个模块注册在 init_array 的全局构造函数
    call_static_constructors();

    // 统一初始化所有通过 DRIVER_INIT 注册的驱动
    driver_init_all();

    CharDeviceDriver *output_driver = CharDeviceDriver::Default();
    output_driver->write("Hello XuYuanOS!\n");

    test();

    // 链接脚本符号必须用其地址（声明为数组可避免误用）
    extern char __user_text_start[];
    extern char user_stack_top[];
    output_driver->write("Jump to user mode...\n");
    output_driver->write("User text start: ");
    output_driver->puthex(reinterpret_cast<unsigned long>(__user_text_start));
    output_driver->write("\n");
    jump_to_user_mode(reinterpret_cast<uint64_t>(__user_text_start),
                      reinterpret_cast<uint64_t>(user_stack_top));

    while (true) {
        arch_wait_for_interrupt();
    }
}

void test() {
    uint64_t page1 = mm::alloc_page();
    uint64_t page2 = mm::alloc_page();

    CharDeviceDriver *output_driver = CharDeviceDriver::Default();
    output_driver->write("Allocated pages: ");
    output_driver->puthex(page1);
    output_driver->write(", ");
    output_driver->puthex(page2);
    output_driver->write("\n");
}