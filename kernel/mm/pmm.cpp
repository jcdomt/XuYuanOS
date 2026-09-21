// 物理页帧分配器
// 初版先使用 bitmap 来管理物理页帧的分配情况，后续可以考虑使用 buddy system 来优化内存分配效率

#include <kernel/string.h>
#include <kernel/utils/math.h>

#include <kernel/mm/pmm.h>

static uint8_t *bitmap = nullptr; // 位图，1 表示已分配，0 表示空闲
static uint64_t total_pages = 0; // 总页数
static uint64_t base_addr = 0; // 物理内存起始地址
static uint64_t next_free_hint = 0; // 下一个空闲页的索引

// 物理页帧分配器初始化
// reserved_end 是内核保留内存的结束地址，物理页帧分配器不会分配保留内存中的页帧
void mm::pmm::init(uint64_t mem_start, uint64_t mem_end, uint64_t reserved_end) {
    (void)mem_start;

    base_addr = (reserved_end + PAGE_SIZE - 1) & ~(PAGE_SIZE - 1); // 对齐到页边界
    total_pages = (mem_end - base_addr) / PAGE_SIZE;

    // bitmap 自身在内核空间的 最前端，直接标记为已分配
    bitmap = reinterpret_cast<uint8_t *>(base_addr);
    // 计算 bitmap 所需的字节数
    uint64_t bitmap_size = CEIL_DIV(total_pages, 8); // 计算 bitmap 所需的字节数，+7 是为了向上取整
    memset(bitmap, 0xFF, bitmap_size); // 初始化为已分配

    // 标记保留内存范围内的页为已分配
    uint64_t bitmap_pages = CEIL_DIV(bitmap_size, PAGE_SIZE);
    for (uint64_t i = bitmap_pages; i < total_pages; ++i) {
        BITMAP_CLEARBIT(bitmap, i);   // 其余页标为空闲
    }
}

// 分配一个物理页帧，返回物理地址
uint64_t mm::pmm::alloc_page() {
    for (uint64_t i = next_free_hint; i < total_pages; ++i) {
        if (!BITMAP_TESTBIT(bitmap, i)) { // 如果该页是空闲的
            BITMAP_SETBIT(bitmap, i);
            next_free_hint = i + 1; // 更新下一个空闲页的索引
            return base_addr + i * PAGE_SIZE; // 返回物理地址
        }
    }
    // 如果没有找到空闲页，从头开始查找
    for (uint64_t i = 0; i < next_free_hint; ++i) {
        if (!BITMAP_TESTBIT(bitmap, i)) { // 如果该页是空闲的
            BITMAP_SETBIT(bitmap, i);
            next_free_hint = i + 1; // 更新下一个空闲页的索引
            return base_addr + i * PAGE_SIZE; // 返回物理地址
        }
    }
    return 0; // 如果没有找到空闲页，返回 0
}

// 释放一个物理页帧
void mm::pmm::free_page(uint64_t addr) {
    if (addr < base_addr || addr >= base_addr + total_pages * PAGE_SIZE) {
        return; // 地址不在管理范围内，直接返回
    }
    uint64_t index = (addr - base_addr) / PAGE_SIZE;
    BITMAP_CLEARBIT(bitmap, index);
    if (index < next_free_hint) {
        next_free_hint = index; // 更新下一个空闲页的索引
    }
}

// 获取空闲页的数量
uint64_t mm::pmm::free_page_count() {
    uint64_t count = 0;
    for (uint64_t i = 0; i < total_pages; ++i) {
        if (!BITMAP_TESTBIT(bitmap, i)) {
            ++count;
        }
    }
    return count;
}