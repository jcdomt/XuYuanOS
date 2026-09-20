#pragma once

#include <cstddef>

extern "C" void *memset(void *dest, int c, size_t n);
extern "C" void *memcpy(void *dest, const void *src, size_t n);
extern "C" char *strcpy(char *dest, const char *src);
