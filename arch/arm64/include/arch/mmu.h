#pragma once

#include <stdint.h>

extern "C" int arch_mmu_enable(uint64_t root_table_phys_addr);  // 汇编函数，启用 MMU

namespace arch {
    static inline int enable_mmu(uint64_t root_table_phys_addr) {
        return arch_mmu_enable(root_table_phys_addr);
    };  // 启用 MMU，传入页表根物理地址
}