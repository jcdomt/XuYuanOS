#ifndef __UART_H__
#define __UART_H__

int uart_puts(const char *s);
int uart_putc(char c);

int uart_puthex(unsigned long value);

#endif // __UART_H__