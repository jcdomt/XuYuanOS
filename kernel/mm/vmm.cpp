#include <stdint.h>

#include <arch/vmm.h>
#include <arch/mmu.h>

#include <kernel/mm/vmm.h>

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

int mm::vmm::enable_mmu(uint64_t root_table_phys_addr) {
    return arch::enable_mmu(root_table_phys_addr);
}