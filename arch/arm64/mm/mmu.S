.section .boot.text

.global arch_enable_mmu
arch_enable_mmu:
    // x0 = ttbr0_el1
    // x1 = ttbr1_el1

    // 设置页表根寄存器
    msr ttbr0_el1, x0
    msr ttbr1_el1, x1
    // 暂时两个槽位的页表都指向同一个根页表
    isb

    // MAIR
    // 0 槽位：系统地址
    // 1 槽位：设备地址
    // MAIR: [Device][Normal]
    ldr x1, =0x04FF
    msr mair_el1, x1
    isb

    // TCR
    // TCR: [T0SZ][T1SZ][TG0][TG1][SH0][SH1][ORGN0][ORGN1][IRGN0][IRGN1]
    // 使用 4KB 页，4级页表，虚拟地址空间大小为 48 位
    // T0SZ = 64 - 48 = 16
    // T1SZ = 64 - 48 = 16
    // TG0 = 00 (4KB)
    // TG1 = 10 (4KB)
    // SH0 = 11 (Inner Shareable)
    // SH1 = 11 (Inner Shareable)
    // ORGN0 = 01 (Write-Back Read-Allocate Write-Allocate Cacheable)
    // ORGN1 = 01 (Write-Back Read-Allocate Write-Allocate Cacheable)
    // IRGN0 = 01 (Write-Back Read-Allocate Write-Allocate Cacheable)
    // IRGN1 = 01 (Write-Back Read-Allocate Write-Allocate Cacheable)
    // TCR_EL1 = 0x2B5103510
    ldr x1, =0x2B5103510
    msr tcr_el1, x1
    isb

    dsb sy

    // 开 MMU
    mrs x1, sctlr_el1
    orr x1, x1, #1  // 设置 M 位，启用
    // orr x1, x1, #(1 << 2)  // 设置 C 位，启用缓存
    // orr x1, x1, #(1 << 12) // 设置 I 位
    msr sctlr_el1, x1
    isb

    // 启动 MMU 后，CPU 已经在使用虚拟地址访问内存了
    // 我们对内核做了初始映射 和 高半映射，所以此时初始区域的代码仍然是恒等映射的，仍然可以继续执行
    // 然后我们应当跳转到高半区的内核入口点去继续执行
    ldr x0, =high_entry
    br x0

.size arch_enable_mmu, . - arch_enable_mmu

.section .text
.global arch_switch_page_tables
arch_switch_page_tables:
    // x0 = 新的 TTBR0 根（空表）, x1 = 新的 TTBR1 根（内核表）
    dsb sy
    msr ttbr0_el1, x0
    msr ttbr1_el1, x1
    isb
    tlbi vmalle1
    dsb sy
    isb
    ret
.size arch_switch_page_tables, . - arch_switch_page_tables