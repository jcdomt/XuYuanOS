#include <kernel/string.h>

// freestanding 环境下编译器仍可能生成 memset/memcpy 调用，需自行提供

extern "C" void *memset(void *dest, int c, size_t n)
{
    unsigned char *d = static_cast<unsigned char *>(dest);
    for (size_t i = 0; i < n; ++i) {
        d[i] = static_cast<unsigned char>(c);
    }
    return dest;
}

extern "C" void *memcpy(void *dest, const void *src, size_t n)
{
    unsigned char *d = static_cast<unsigned char *>(dest);
    const unsigned char *s = static_cast<const unsigned char *>(src);
    for (size_t i = 0; i < n; ++i) {
        d[i] = s[i];
    }
    return dest;
}

extern "C" char *strcpy(char *dest, const char *src)
{
    char *d = dest;
    while ((*d++ = *src++)) {
    }
    return dest;
}

