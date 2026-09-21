#pragma once

#include <stdint.h>

namespace mm {
    namespace pmm {
        constexpr uint64_t PAGE_SIZE = 4096; // 4KB

        void init(uint64_t mem_start, uint64_t mem_end, uint64_t reserved_end);
        
        uint64_t alloc_page();
        void free_page(uint64_t addr);
        uint64_t free_page_count();
    }
}


#define BITMAP_SETBIT(bitmap, index) ((bitmap)[(index) / 8] |= (1 << ((index) % 8)))
#define BITMAP_CLEARBIT(bitmap, index) ((bitmap)[(index) / 8] &= ~(1 << ((index) % 8)))
#define BITMAP_TESTBIT(bitmap, index) ((bitmap)[(index) / 8] & (1 << ((index) % 8)))