#include <arch/arch.h>

#include <utils/types.h>
#include <utils/string.h>
#include <board/qemu_virt.h>

#include <kernel/process/process.h>
#include <kernel/process/loader.h>
#include <kernel/mm/vmm.h>
#include <kernel/mm/pmm.h>
#include <kernel/mm/mm.h>
using namespace mm;

Process *Process::create_process(PID_T parent_pid) {
    Process *process = new Process();
    process->pid = pid_generator.generate();
    process->ppid = parent_pid;
    process->set_state(PROCESS_STATE_READY);
    return process;
}

bool Process::load(U64 user_image_start, U64 user_image_end) {
    // 加载用户态程序
    U64 uroot = this->read(user_image_start, user_image_end);
    if (!uroot) {
        return false;
    }
    this->set_uroot(uroot);

    // 分配栈空间
    U64 spa = this->alloc_stack(uroot);
    if (!spa) {
        return false;
    }

    if (this->get_format_type() == PROCESS_FORMAT_ELF) {
        // @TODO: 解析 ELF 文件，获取入口点
        return false;
    } else if (this->get_format_type() == PROCESS_FORMAT_BIN) {
        // 二进制格式，入口点为 USER_BASE
        this->set_entry_point(USER_BASE);
    } else {
        return false; // 不支持的格式
    }

    return true;
}

U64 Process::read(U64 user_image_start, U64 user_image_end) {
    // 读取用户态程序实体
    U64 image_size = user_image_end - user_image_start;
    if (image_size == 0) {
        return 0;
    }

    U64 uroot = vmm::create_page_table();
    for (U64 offset = 0; offset < image_size; offset += PAGE_SIZE) {
        U64 phys_page = pmm::alloc_page();
        if (!phys_page) {
            // @TODO: 分配失败，清理已分配的页表和物理页
            return 0;
        }

        U64 chunk_size = (image_size - offset) < PAGE_SIZE ? (image_size - offset) : PAGE_SIZE;
        void *kva = (void *)(phys_page + PHYS_OFFSET);
        // memset(kva, 0, PAGE_SIZE);
        memcpy(kva, (void *)(user_image_start + offset), chunk_size);
        vmm::map_page(uroot, USER_BASE + offset, phys_page, VM_READ | VM_WRITE | VM_USER | VM_EXEC);
    }

    return uroot;
}

U64 Process::alloc_stack(U64 uroot) {
    // 分配用户态栈空间
    U64 spa = pmm::alloc_page();
    if (!spa) {
        return 0;
    }
    memset((void *)(spa + PHYS_OFFSET), 0, 0x1000);
    vmm::map_page(uroot, USER_STACK_TOP - 0x1000, spa, VM_READ | VM_WRITE | VM_USER);
    return spa;
}

void Process::start() {
    // 切换到用户态执行程序
    arch::enter_user_mode(this->get_uroot(), this->get_entry_point(), USER_STACK_TOP);
}