#pragma once

// 两数相除向上取整
#define CEIL_DIV(a, b) (((a) + (b) - 1) / (b))

// 向上对齐
#define align_up(x, align) (((x) + (align) - 1) & ~((align) - 1))