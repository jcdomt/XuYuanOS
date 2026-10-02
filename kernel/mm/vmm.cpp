#include <stdint.h>

#include <arch/vmm.h>

#include <kernel/mm/vmm.h>
#include <kernel/mm/pmm.h>

uint64_t mm::vmm::create_page_table() {
    return arch::create_page_table();
}

void mm::vmm::map_page(uint64_t root, uint64_t virt_addr, uint64_t phys_addr, uint32_t flags) {
    uint64_t arm64_attrs = arch::translate_flags(flags);
    arch::map_page(root, virt_addr, phys_addr, arm64_attrs);
}

uint64_t mm::vmm::virt_to_phys(uint64_t root, uint64_t virt_addr) {
    return arch::virt_to_phys(root, virt_addr);
}

uint64_t mm::vmm::alloc_virt_memory(uint64_t start, uint64_t size, uint32_t flags) {
    // 计算需要的页数
    uint64_t page_root = create_page_table();
    // 以 4KB 为单位分配虚拟内存
    for (uint64_t offset = 0; offset < size; offset += _4KB) {
        // 分配一个物理页
        uint64_t pa = mm::pmm::alloc_page();
        if (!pa) {
            // @TODO: 分配失败，清理已分配的页表和物理页
        }
        // 映射虚拟地址到物理地址
        map_page(page_root, start + offset, pa, flags);
    }
    return page_root;
}