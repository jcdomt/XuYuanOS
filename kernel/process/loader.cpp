// 用户态程序加载器

#include <arch/arch.h>
#include <board/qemu_virt.h>

#include <stdint.h>

#include <string.h>
#include <kernel/mm/pmm.h>
#include <kernel/mm/vmm.h>
#include <kernel/process/loader.h>

#define U64 uint64_t

int load_and_enter_user(U64 user_image_start, U64 user_image_end)
{
    // 计算用户态程序大小
    U64 size = user_image_end - user_image_start;

    // 每个用户态程序都要有自己的页表
    U64 uroot = mm::vmm::create_page_table();
    for(U64 off = 0; off < size; off += 0x1000) {
        U64 pa = mm::pmm::alloc_page();
        void *kva = (void *)(pa + PHYS_OFFSET);
        memset(kva, 0, 0x1000);
        // 如果是最后一个块，可能不足 4KB
        U64 chunk = (size - off) < 0x1000 ? (size - off) : 0x1000;
        memcpy(kva, (void *)(user_image_start + off), chunk);
        mm::vmm::map_page(uroot, USER_BASE + off, pa, VM_READ | VM_WRITE | VM_USER | VM_EXEC);
    }

    // 用户栈
    U64 spa = mm::pmm::alloc_page();
    memset((void *)(spa + PHYS_OFFSET), 0, 0x1000);
    mm::vmm::map_page(uroot, USER_STACK_TOP - 0x1000, spa, VM_READ | VM_WRITE | VM_USER);

    arch::enter_user_mode(uroot, USER_BASE, USER_STACK_TOP);
    
    return 0; // 不会返回
}

#undef U64