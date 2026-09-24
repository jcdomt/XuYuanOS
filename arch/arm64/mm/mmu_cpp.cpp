#include <stdint.h>

#include <board/qemu_virt.h>

#define U64 uint64_t

extern "C" __attribute__((noreturn)) void arch_enable_mmu(U64 low, U64 high);

namespace {
    constexpr U64 RAM_START = U64(MEM_START);
    constexpr U64 RAM_SIZE = U64(TOTAL_MEMORY_SIZE);

    // 使用 2MB 的块映射内存
    constexpr U64 BLOCK_SIZE = 2 * 1024 * 1024;

    // 对于设备内存，我们暂时定义设备内存仅为 QEMU 的 PL011 UART 寄存器的内存范围
    constexpr U64 DEVICE_START = U64(PL011_BASE);

    // 内存半高地址 (+0xFFFF000000000000)
    constexpr U64 MEMORY_HALF_HIGH = U64(PHYS_OFFSET);
    // 内核半高地址
    constexpr U64 KERNEL_VBASE = U64(MEMORY_HALF_HIGH + KERNEL_LOAD_ADDR);

    // ARM64 页表项的属性
    /*
        bit 0       Valid
        bit 1       Table / Block
        bits 4:2    AttrIndx
        bits 7:6    AP
        bits 9:8    SH
        bit 10      AF
        bit 11      nG
        bit 53      PXN
        bit 54      UXN
    */
    constexpr U64 DESC_VALID = 1ULL << 0;
    constexpr U64 DESC_TABLE = 1ULL << 1;
    constexpr U64 DESC_IDX(U64 i) { return (i) << 2; }
    constexpr U64 DESC_SH_INNER = 3ULL << 8;
    constexpr U64 DESC_AF = 1ULL << 10;
    constexpr U64 DESC_PXN = 1ULL << 53;
    constexpr U64 DESC_UXN = 1ULL << 54;

    // 正常内存区域
    constexpr U64 NORMAL_BLOCK = DESC_VALID | DESC_AF | DESC_SH_INNER | DESC_IDX(0) | DESC_UXN;
    // 设备内存区域
    constexpr U64 DEVICE_BLOCK = DESC_VALID | DESC_AF | DESC_IDX(1) | DESC_PXN | DESC_UXN;
    // 表项
    constexpr U64 TABLE_DESC = DESC_VALID | DESC_TABLE;

    // MMU表
    // 每一项 512 * 8 bytes = 4KB
    // 放入 .boot.data
    #define __BOOT_DATA __attribute__((section(".boot.data"), aligned(4096)))
    __BOOT_DATA U64 pgd_0[512] = {0};
    __BOOT_DATA U64 pgd_1[512] = {0};
    __BOOT_DATA U64 pud_low[512] = {0};
    __BOOT_DATA U64 pud_high[512] = {0};    // 内核专用内存区域
    __BOOT_DATA U64 pmd_device[512] = {0};         // 低地址设备内存区域
    __BOOT_DATA U64 pmd_ram_low[512] = {0};         // 低地址 RAM identity mapping 使用
    __BOOT_DATA U64 pmd_ram_high[512] = {0};        // 高地址 RAM identity mapping 使用
    #define L0_LOW pgd_0
    #define L0_HIGH pgd_1
    #define L1_LOW pud_low
    #define L1_HIGH pud_high
    #define L2_DEVICE pmd_device
    #define L2_RAM_LOW pmd_ram_low
    #define L2_RAM_HIGH pmd_ram_high
    #undef __BOOT_DATA

    constexpr U64 idx(U64 addr, U64 level) {
        return (addr >> (39 - level * 9)) & 0x1FF;
    }

}   // namespace

extern "C" __attribute__((noreturn,section(".boot.text"))) void boot_setup_mmu() {
    // 低地址 RAM 映射，这款这块区域负责存储
    const U64 ram_block_count = RAM_SIZE / BLOCK_SIZE;
    for (U64 i = 0; i < ram_block_count; ++i) {
        const U64 addr = RAM_START + i * BLOCK_SIZE;
        L2_RAM_LOW[idx(addr, 2)] = addr | NORMAL_BLOCK;
    }
    L1_LOW[idx(RAM_START, 1)] = U64(L2_RAM_LOW) | TABLE_DESC;

    // 低地址设备内存映射
    L2_DEVICE[idx(DEVICE_START, 2)] = DEVICE_START | DEVICE_BLOCK;
    L1_LOW[idx(DEVICE_START, 1)] = U64(L2_DEVICE) | TABLE_DESC;
    // 顺便往高地址也映射一份，方便内核访问
    L1_HIGH[idx(DEVICE_START, 1)] = U64(L2_DEVICE) | TABLE_DESC;

    // 高半内核内存映射
    // 对于高半地址 0xFFFF000040080000，L0=0, L1=1, L2=0
    // 这里和低半地址的映射类似，只是使用了高半地址的 L1 表
    for (U64 i = 0; i < ram_block_count; ++i) {
        const U64 addr = RAM_START + i * BLOCK_SIZE;
        L2_RAM_HIGH[idx(addr, 2)] = addr | NORMAL_BLOCK;
    }
    // 将这块区域映射到高半地址的 L1 表中
    L1_HIGH[idx(KERNEL_LOAD_ADDR, 1)] = U64(L2_RAM_HIGH) | TABLE_DESC;

    // 最后建立两个L1 和 L0 的映射关系
    L0_LOW[idx(RAM_START, 0)] = U64(L1_LOW) | TABLE_DESC;
    L0_HIGH[idx(KERNEL_VBASE, 0)] = U64(L1_HIGH) | TABLE_DESC;

    arch_enable_mmu(U64(L0_LOW), U64(L0_HIGH));

    __builtin_unreachable();
}

#undef L0
#undef L1_LOW
#undef L1_HIGH
#undef L2_DEVICE
#undef L2_RAM_LOW
#undef L2_RAM_HIGH
#undef U64