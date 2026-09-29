#pragma once
#include <stdint.h>

namespace arch {

    constexpr uint64_t PTE_VALID  = 1ULL << 0;              // 有效位，表示该页表项有效
    constexpr uint64_t PTE_TABLE  = 1ULL << 1;              // 中间级：指向下一级表
    constexpr uint64_t PTE_PAGE   = 1ULL << 1;              // 最后一级：4KB 页
    constexpr uint64_t PTE_ADDR_MASK = 0x0000FFFFFFFFF000;  // 页表项中物理地址的掩码，低 12 位用于标志位

    constexpr uint64_t PTE_ATTR_NORMAL = 0ULL << 2;         // MAIR index 0
    constexpr uint64_t PTE_ATTR_DEVICE = 1ULL << 2;         // MAIR index 1
    constexpr uint64_t PTE_AP_USER     = 1ULL << 6;         // 用户态可访问
    constexpr uint64_t PTE_AP_RO       = 1ULL << 7;         // 只读
    constexpr uint64_t PTE_AF          = 1ULL << 10;        // 访问标志，表示该页已被访问过
    constexpr uint64_t PTE_PXN         = 1ULL << 53;        // 执行禁止位，表示该页不可执行
    constexpr uint64_t PTE_UXN         = 1ULL << 54;        // 用户态执行禁止位，表示该页在用户态不可执行，但是在内核态可执行

    constexpr int ENTRIES = 512;                            // 每个页表有 512 个条目

    // 四个级别的索引提取
    inline int pgd_idx(uint64_t va) { return (va >> 39) & 0x1FF; }  // 页全局目录索引
    inline int pud_idx(uint64_t va) { return (va >> 30) & 0x1FF; }  // 页上级目录索引
    inline int pmd_idx(uint64_t va) { return (va >> 21) & 0x1FF; }  // 页中间目录索引
    inline int pte_idx(uint64_t va) { return (va >> 12) & 0x1FF; }  // 页表项索引

    uint64_t create_page_table();   // 创建新的页表
    void map_page(uint64_t root, uint64_t va, uint64_t pa, uint64_t arm64_attrs); // 映射虚拟地址到物理地址
    uint64_t virt_to_phys(uint64_t root, uint64_t va);  // 虚拟地址转换为物理地址

    // VmFlags -> ARM64 属性位 的翻译，架构层的核心职责
    uint64_t translate_flags(uint32_t vm_flags);    // 翻译虚拟内存标志为 ARM64 属性位

}


#define IS_VALID_PTE(pte) ((pte) & PTE_VALID) // 判断页表项是否有效