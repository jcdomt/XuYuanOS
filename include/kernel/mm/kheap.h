#pragma once

#include <utils/types.h>

#include <kernel/mm/vmm.h>


#define KERNEL_HEAP_START 0xFFFF800000000000

#define DataMemory(block) (reinterpret_cast<void*>(reinterpret_cast<uint64_t>(block) + sizeof(Block)))
#define BlockDataSize(block) (block->size * PAGE_SIZE)

namespace mm::kheap {

    constexpr size_t N = 16; // 初始堆大小，单位为页
    constexpr int KERNEL_PAGE_RW = VM_READ | VM_WRITE | VM_KERNEL; 

    struct Block {
        size_t size;    // 这个快拥有几页
        bool free;
        Block* next;
    };

    extern Block *head;
    // 堆底：堆现在扩散到哪了
    extern U64 heap_end;

    void init();

    void* alloc(size_t size);
    void free(void* ptr);
}
