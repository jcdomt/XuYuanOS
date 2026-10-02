#include <kernel/mm/pmm.h>
#include <board/qemu_virt.h>
#include <stddef.h>

// 暂时简单粗暴：任何 new 都按整页分配
void *operator new(size_t size)
{
    (void)size;
    return reinterpret_cast<void *>(mm::pmm::alloc_page() + PHYS_OFFSET);
}

void operator delete(void *) noexcept {}
void operator delete(void *, size_t) noexcept {}