// 字符设备的统一抽象
#pragma once

#include <cstddef>

#include <driver/driver.h>

class CharDeviceDriver : public DeviceDriver {
public:
    virtual size_t read(char* buffer, size_t size) = 0;
    virtual size_t write(const char* buffer, size_t size) = 0;

    // 便捷接口：写以 '\0' 结尾的字符串
    size_t write(const char* s);
    // 便捷接口：以 0x 前缀输出十六进制
    size_t puthex(unsigned long value);

    // 字符设备驱动类的注册表（整个类共享一份）
    static DriverClass& Class();
    // 默认字符设备，未注册任何设备时返回 nullptr
    static CharDeviceDriver* Default();
};
