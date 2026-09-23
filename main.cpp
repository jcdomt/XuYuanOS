#include <arch/arch.h>
#include <board/qemu_virt.h>

#include <kernel/mm/pmm.h>
#include <kernel/mm/vmm.h>

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

#define __USE_TEST__
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
    extern char __user_text_start[];
    extern char __user_text_end[];
    uint64_t user_text_start = reinterpret_cast<uint64_t>(__user_text_start);
    uint64_t user_text_end = reinterpret_cast<uint64_t>(__user_text_end);
    extern char __user_stack_top[];
    uint64_t user_stack_top = reinterpret_cast<uint64_t>(__user_stack_top);

    mm::pmm::init(mem_start, mem_end, kernel_end);

    // 初始化 MMU
    uint64_t boot_root = mm::vmm::create_page_table();
    // 恒等映射内核所在的物理内存区域
   // 1. 内核区：内核权限
    for (uint64_t pa = kernel_start & ~0xFFF; pa < user_text_start; pa += 0x1000)
        mm::vmm::map_page(boot_root, pa, pa, VM_READ|VM_WRITE|VM_EXEC|VM_KERNEL);
    // 2. 用户区：用户权限（text 可执行只读，data/栈可读写不可执行）
    for (uint64_t pa = user_text_start; pa < user_text_end; pa += 0x1000)
        mm::vmm::map_page(boot_root, pa, pa, VM_READ|VM_EXEC|VM_USER);
    for (uint64_t pa = user_text_end; pa < (uint64_t)user_stack_top; pa += 0x1000)
        mm::vmm::map_page(boot_root, pa, pa, VM_READ|VM_WRITE|VM_USER);
    // 3. 其余物理内存（bitmap、页表）：内核权限
    for (uint64_t pa = (uint64_t)user_stack_top; pa < mem_end; pa += 0x1000)
        mm::vmm::map_page(boot_root, pa, pa, VM_READ|VM_WRITE|VM_KERNEL);
    // 临时映射下 UART0 的 MMIO 寄存器，便于输出调试信息
    mm::vmm::map_page(boot_root, PL011_BASE, PL011_BASE, VM_READ | VM_WRITE | VM_KERNEL | VM_DEVICE);
    // 启用 MMU
    mm::vmm::enable_mmu(boot_root);


    // 初始化各个模块注册在 init_array 的全局构造函数
    call_static_constructors();

    // 统一初始化所有通过 DRIVER_INIT 注册的驱动
    driver_init_all();

    CharDeviceDriver *output_driver = CharDeviceDriver::Default();
    output_driver->write("Hello XuYuanOS!\n");

    test();

    // 链接脚本符号必须用其地址（声明为数组可避免误用）
    // extern char __user_text_start[];
    // extern char user_stack_top[];
    // output_driver->write("Jump to user mode...\n");
    // output_driver->write("User text start: ");
    // output_driver->puthex(reinterpret_cast<unsigned long>(__user_text_start));
    // output_driver->write("\n");
    // jump_to_user_mode(reinterpret_cast<uint64_t>(__user_text_start),
    //                   reinterpret_cast<uint64_t>(user_stack_top));

    while (true) {
        arch::wait_for_interrupt();
    }
}

void test() {
#ifndef __USE_TEST__
    return;
#endif //__USE_TEST__
    // 测试 PMM
    uint64_t page1 = mm::pmm::alloc_page();
    uint64_t page2 = mm::pmm::alloc_page();

    CharDeviceDriver *output_driver = CharDeviceDriver::Default();
    output_driver->write("Allocated pages: ");
    output_driver->puthex(page1);
    output_driver->write(", ");
    output_driver->puthex(page2);
    output_driver->write("\n");

    // 测试 VMM
    uint64_t root = mm::vmm::create_page_table();   // 获得一页 4KB
    output_driver->write("page table root: ");
    output_driver->puthex(root);   // 应非 0，且 4KB 对齐（低 12 位为 0）
    output_driver->write("\n");

    uint64_t r = mm::vmm::virt_to_phys(root, 0x12345000);
    output_driver->write("virt_to_phys(0x12345000) = ");
    output_driver->puthex(r);   // 应为 0，尚未映射
    output_driver->write("\n");

    mm::vmm::map_page(root, 0x12345000, 0x400000000, VM_READ | VM_WRITE | VM_KERNEL);

    r = mm::vmm::virt_to_phys(root, 0x12345000);
    output_driver->write("virt_to_phys(0x12345000) = ");
    output_driver->puthex(r);   // 应为 0x400000000
    output_driver->write("\n");

    r = mm::vmm::virt_to_phys(root, 0x12345001);
    output_driver->write("virt_to_phys(0x12345001) = ");
    output_driver->puthex(r);   // 应为 0x400000001
    output_driver->write("\n");

}