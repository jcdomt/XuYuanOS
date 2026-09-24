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

#define U64 uint64_t

extern "C" void kernel_main()
{
    extern char __kernel_start[];
    extern char __kernel_end[];
    uint64_t mem_start = reinterpret_cast<uint64_t>((uint64_t)MEM_START);
    uint64_t mem_end = mem_start + TOTAL_MEMORY_SIZE;
    // uint64_t kernel_start = reinterpret_cast<uint64_t>(__kernel_start);
    uint64_t kernel_end = reinterpret_cast<uint64_t>(__kernel_end);
    extern char __text_start[];
    extern char __text_end[];
    U64 text_start = reinterpret_cast<U64>(__text_start);
    U64 text_end = reinterpret_cast<U64>(__text_end);
    extern char __rodata_start[];
    extern char __rodata_end[];
    U64 rodata_start = reinterpret_cast<U64>(__rodata_start);
    U64 rodata_end = reinterpret_cast<U64>(__rodata_end);
    extern char __data_start[];
    extern char __data_end[];
    U64 data_start = reinterpret_cast<U64>(__data_start);
    U64 data_end = reinterpret_cast<U64>(__data_end);

    // 先初始化内存管理器
    mm::pmm::init(mem_start, mem_end, kernel_end - PHYS_OFFSET);

    // 建立内核 VMM 表
    uint64_t kroot = mm::vmm::create_page_table();
    for (uint64_t addr = mem_start; addr < mem_end; addr += 0x1000) {
        mm::vmm::map_page(kroot, addr + PHYS_OFFSET, addr, VM_READ | VM_WRITE | VM_KERNEL);
    }
    auto map_sect = [&](uint64_t s, uint64_t e, uint32_t f) {
        for (uint64_t addr = s; addr < e; addr += 0x1000) {
            mm::vmm::map_page(kroot, addr, addr - PHYS_OFFSET, f);
        }
    };
    map_sect(text_start, text_end, VM_READ | VM_WRITE | VM_KERNEL | VM_EXEC);
    map_sect(rodata_start, rodata_end, VM_READ | VM_KERNEL);
    map_sect(data_start, data_end, VM_READ | VM_WRITE | VM_KERNEL);
    mm::vmm::map_page(kroot, PL011_BASE + PHYS_OFFSET, PL011_BASE, VM_READ | VM_WRITE | VM_KERNEL);
    U64 uroot = mm::vmm::create_page_table();
    arch::switch_page_tables(uroot, kroot);

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

    int a = 1;
    output_driver->write("In kernel, &a = ");
    output_driver->puthex(reinterpret_cast<uint64_t>(&a));
    output_driver->write("\n");
}

#undef U64