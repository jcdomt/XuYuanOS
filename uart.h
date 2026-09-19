#ifndef __UART_H__
#define __UART_H__

int uart_puts(const char *s);
int uart_putc(char c);

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

#endif // __UART_H__