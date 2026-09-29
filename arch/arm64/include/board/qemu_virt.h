#pragma once

// QEMU virt 机器 (cortex-a53) 板级定义
#define QEMU_VIRT

#define PL011_BASE 0x09000000

// 内存开始地址
#define MEM_START 0x40000000
// 系统总内存
#define TOTAL_MEMORY_SIZE 0x20000000 // 512 M
// 内核偏移常量
#define PHYS_OFFSET 0xFFFF000000000000
// 内核初始地址
#define KERNEL_LOAD_ADDR 0x40080000