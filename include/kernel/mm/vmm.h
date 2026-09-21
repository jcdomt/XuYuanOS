#pragma once
// 虚拟内存管理

#include <stdint.h>

// 权限标志，描述一块内存的可读、可写、可执行等属性
enum VmFlags : uint32_t {
    VM_READ = 1 << 0,   // 可读
    VM_WRITE = 1 << 1,  // 可写
    VM_EXEC = 1 << 2,   // 可执行
    VM_USER = 1 << 3,   // 用户态可访问
    VM_KERNEL = 1 << 4, // 内核态可访问
    VM_DEVICE = 1 << 5, // MMIO 设备内存
};

namespace mm {
    namespace vmm {
        // 创建新的页表
        uint64_t create_page_table();
        // 映射虚拟地址到物理地址
        void map_page(uint64_t root, uint64_t virt_addr, uint64_t phys_addr, uint32_t flags);
        // 虚拟地址转换为物理地址
        uint64_t  virt_to_phys(uint64_t root, uint64_t virt_addr);
    }
}