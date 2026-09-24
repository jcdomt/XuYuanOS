#include <arch/vmm.h>

#include <board/qemu_virt.h>

#include <kernel/mm/pmm.h>
#include <kernel/string.h>

#include <kernel/mm/vmm.h>

using namespace arch;

struct PageTable {
    uint64_t entries[ENTRIES];
};

constexpr size_t PAGE_TABLE_SIZE = sizeof(PageTable);

static PageTable *ptr(uint64_t phys_addr) {
    return reinterpret_cast<PageTable *>(phys_addr + PHYS_OFFSET);
}

uint64_t arch::create_page_table() {
    uint64_t pa = mm::pmm::alloc_page();
    if (pa) {
        // 分配到的页表页需要清零，避免残留数据影响页表的正确性
        memset(ptr(pa), 0, PAGE_TABLE_SIZE);
    }
    return pa;
}

// 取下一级页表的物理地址，如果不存在则创建新的页表
static PageTable *next_level(PageTable *t, int idx) {
    if (!IS_VALID_PTE(t->entries[idx])) {
        uint64_t pa = mm::pmm::alloc_page();
        if (!pa) {
            return nullptr; // 分配失败
        }
        memset(ptr(pa), 0, PAGE_TABLE_SIZE);
        // 标记这一项为有效，并指向新分配的页表
        t->entries[idx] = pa | PTE_VALID | PTE_TABLE;
    }
    return ptr(t->entries[idx] & PTE_ADDR_MASK);
}

// 将虚拟地址 va 映射到物理地址 pa，arm64_attrs 是 ARM64 页表项的属性位
void arch::map_page(uint64_t root, uint64_t va, uint64_t pa, uint64_t arm64_attrs) {
    PageTable *pgd = ptr(root);
    // 根据虚拟地址的不同级别索引，逐级获取页表，如果不存在则创建新的页表
    PageTable *pud = next_level(pgd, pgd_idx(va));
    PageTable *pmd = next_level(pud, pud_idx(va));
    PageTable *pte = next_level(pmd, pmd_idx(va));

    // 设置页表项，标记为有效，并设置物理地址和属性
    pte->entries[pte_idx(va)] = (pa & PTE_ADDR_MASK) | PTE_VALID | PTE_PAGE | arm64_attrs;
}

// 将虚拟地址 va 转换为物理地址，返回物理地址，如果不存在映射则返回 0
uint64_t arch::virt_to_phys(uint64_t root, uint64_t va) {
    PageTable *pgd = ptr(root);
    if (!IS_VALID_PTE(pgd->entries[pgd_idx(va)])) {
        return 0; // 页全局目录项无效
    }
    PageTable *pud = ptr(pgd->entries[pgd_idx(va)] & PTE_ADDR_MASK);
    if (!IS_VALID_PTE(pud->entries[pud_idx(va)])) {
        return 0; // 页上级目录项无效
    }
    PageTable *pmd = ptr(pud->entries[pud_idx(va)] & PTE_ADDR_MASK);
    if (!IS_VALID_PTE(pmd->entries[pmd_idx(va)])) {
        return 0; // 页中间目录项无效
    }
    PageTable *pte = ptr(pmd->entries[pmd_idx(va)] & PTE_ADDR_MASK);
    if (!IS_VALID_PTE(pte->entries[pte_idx(va)])) {
        return 0; // 页表项无效
    }
    return (pte->entries[pte_idx(va)] & PTE_ADDR_MASK) | (va & 0xFFF); // 返回物理地址
}

uint64_t arch::translate_flags(uint32_t vm_flags) {
    uint64_t attrs = 0;
    if (vm_flags & VM_READ) {
        // ARM64 默认可读，无需额外设置
    }
    if (vm_flags & VM_WRITE) {
        attrs |= PTE_AF; // 设置访问标志，表示该页已被访问过
    } else {
        attrs |= PTE_AF | PTE_AP_RO; // 设置访问标志和只读位，表示该页已被访问过且只读
    }
    if (vm_flags & VM_EXEC) {
        // ARM64 默认可执行，无需额外设置
    } else {
        attrs |= PTE_PXN | PTE_UXN; // 设置执行禁止位，表示该页不可执行
    }
    if (vm_flags & VM_USER) {
        attrs |= PTE_AP_USER; // 用户态可访问
        if (vm_flags & VM_EXEC) {
            attrs |= PTE_PXN; // 内核态不可执行，防止用户态代码被内核执行
        }
    } else {
        attrs &= ~PTE_AP_USER; // 内核态可访问
    }
    if (vm_flags & VM_DEVICE) {
        attrs |= PTE_ATTR_DEVICE; // MMIO 设备内存
    } else {
        attrs |= PTE_ATTR_NORMAL; // 普通内存
    }
    return attrs;
}