#include <driver/char_driver.h>

// 静态存储期对象，无构造函数，由 BSS 清零保证初始状态
static DriverClass char_driver_class;

DriverClass& CharDeviceDriver::Class()
{
    return char_driver_class;
}

CharDeviceDriver* CharDeviceDriver::Default()
{
    return (CharDeviceDriver*)char_driver_class.GetDefault();
}

size_t CharDeviceDriver::write(const char* s)
{
    size_t len = 0;
    while (s[len])
        ++len;
    return write(s, len);
}

size_t CharDeviceDriver::puthex(unsigned long value)
{
    char buf[18];
    buf[0] = '0';
    buf[1] = 'x';
    for (int i = 0; i < 16; ++i) {
        unsigned long digit = (value >> ((15 - i) * 4)) & 0xF;
        buf[2 + i] = digit < 10 ? (char)('0' + digit) : (char)('A' + digit - 10);
    }
    return write(buf, sizeof(buf));
}
