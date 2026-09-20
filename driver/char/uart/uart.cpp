#include <driver/uart/uart.h>
#include <board/qemu_virt.h>

static inline void uart_raw_putc(char c)
{
    volatile unsigned int *uart =
        (volatile unsigned int *)PL011_BASE;

    *uart = c;
}

// 对 UartDriver
int UartDriver::init()
{
    // 这里暂时不实现硬件初始化功能
    setName("uart");
    CharDeviceDriver::Class().Register(this);
    return 0;
}

size_t UartDriver::read(char* buffer, size_t size)
{
    // 这里暂时不实现读取功能
    (void)buffer;
    (void)size;
    return 0;
}

size_t UartDriver::write(const char* buffer, size_t size)
{
    for (size_t i = 0; i < size; ++i)
        uart_raw_putc(buffer[i]);

    return size;
}

// 驱动实例：静态存储期，生命周期与内核相同
static UartDriver uart_driver;

static int uart_driver_entry()
{
    return uart_driver.init();
}

// 注册到 .driver_init 段，由 driver_init_all() 统一初始化
DRIVER_INIT(uart_driver_entry);
