#ifndef __UART_CPP__
#define __UART_CPP__

#include <driver/uart/uart.h>

int uart_puts(const char *s)
{
    volatile unsigned int *uart =
        (volatile unsigned int *)0x09000000;

    while (*s) {
        *uart = *s++;
    }

    return 0;
}

int uart_putc(char c)
{
    volatile unsigned int *uart =
        (volatile unsigned int *)0x09000000;

    *uart = c;

    return 0;
}

int uart_puthex(unsigned long value)
{
    uart_puts("0x");

    for (int i = 15; i >= 0; --i) {
        unsigned long digit = (value >> (i * 4)) & 0xF;

        if (digit < 10)
            uart_putc('0' + digit);
        else
            uart_putc('A' + digit - 10);
    }

    return 0;
}

#endif // __UART_CPP__