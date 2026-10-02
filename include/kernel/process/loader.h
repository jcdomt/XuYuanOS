#pragma once

#include <stdint.h>
#include <utils/types.h>

constexpr U64 USER_BASE = 0x40000000;   // 用户态程序加载基址
constexpr U64 USER_STACK_TOP = 0x80000000;   // 用户态程序栈顶地址

int load_and_enter_user(uint64_t user_image_start, uint64_t user_image_end);