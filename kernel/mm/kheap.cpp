
#include <board/qemu_virt.h>

#include <utils/types.h>
#include <utils/math.h>

#include <kernel/mm/pmm.h>
#include <kernel/mm/kheap.h>
#include <kernel/mm/mm.h>
using namespace mm;

namespace mm::kheap {
    Block *head = nullptr;
    U64 heap_end = KERNEL_HEAP_START;
}

void expand_heap(size_t size);

void mm::kheap::init()
{
    expand_heap(N * PAGE_SIZE);
    head = reinterpret_cast<Block*>(KERNEL_HEAP_START);
    head->size = N;
    head->free = true;
    head->next = nullptr;
}

void expand_heap(size_t size)
{
    size_t pages = CEIL_DIV(size, PAGE_SIZE);
    for (size_t i = 0; i < pages; ++i) {
        uint64_t phys = pmm::alloc_page();
         vmm::map_page(
            vmm::g_kroot,
            kheap::heap_end,
            phys,
            kheap::KERNEL_PAGE_RW
        );
        kheap::heap_end += PAGE_SIZE;
    }
}

void *mm::kheap::alloc(size_t size)
{
    size = align_up(size, N);
    for (Block* curr = head; curr; curr = curr->next) {
        // 找到一个空闲且足够大的块
        if (curr->free && BlockDataSize(curr) >= size) {
            // 分裂：剩下的空间还够放一个头+数据，就切成两块
            // 如果分配后剩下的空间足够放一个头+16字节，我们就认为可以分裂
            if (BlockDataSize(curr) - size >= sizeof(Block) + 16) {
                // 创建一个新的块头，放在当前块的后面
                Block* new_block = reinterpret_cast<Block*>((char*)curr + sizeof(Block) + size);
                new_block->size = (BlockDataSize(curr) - size - sizeof(Block)) / PAGE_SIZE;
                new_block->free = true;
                new_block->next = curr->next;
                curr->next = new_block;
                curr->size = size;
                curr->free = false;
                return DataMemory(curr);
            } else {
                // 不够分裂，就直接分配整个块
                curr->free = false;
                return DataMemory(curr);
            }
        }
    }

    // 没有找到合适的块，扩展堆
    expand_heap(size + sizeof(Block));
    return alloc(size); // 递归调用，尝试再次分配
}

void mm::kheap::free(void* ptr)
{
    Block* curr = (Block*)ptr - 1;   // 头就在数据前面（等价于 ptr - sizeof(Block)）
    curr->free = true;

    // 向后合并：下一块空闲就吞掉
    if (curr->next && curr->next->free) {
        curr->size += sizeof(Block) + curr->next->size;
        curr->next = curr->next->next;
    }
    // 向前合并：找到前驱，前驱空闲就让它吞掉自己
    // （遍历链表找 next == b 的那个）
    for (Block* prev = head; prev; prev = prev->next) {
        if (prev->next == curr && prev->free) {
            prev->size += sizeof(Block) + curr->size;
            prev->next = curr->next;
            break;
        }
    }
}

void* operator new(size_t sz)               { return mm::kheap::alloc(sz); }
void* operator new[](size_t sz)             { return mm::kheap::alloc(sz); }
void  operator delete(void* p) noexcept     { mm::kheap::free(p); }
void  operator delete[](void* p) noexcept   { mm::kheap::free(p); }
void  operator delete(void* p, size_t) noexcept     { mm::kheap::free(p); }
void  operator delete[](void* p, size_t) noexcept   { mm::kheap::free(p); }