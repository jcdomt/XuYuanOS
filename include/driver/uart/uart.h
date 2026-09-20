#ifndef __UART_H__
#define __UART_H__

#include <driver/char_driver.h>

class UartDriver : public CharDeviceDriver {
public:
    int init() override;
    size_t read(char* buffer, size_t size) override;
    size_t write(const char* buffer, size_t size) override;
};

#endif // __UART_H__
