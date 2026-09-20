#include <driver/driver.h>

// 链接脚本导出的驱动初始化函数表边界
extern DriverInitFunc __driver_init_start[];
extern DriverInitFunc __driver_init_end[];

void driver_init_all()
{
    for (DriverInitFunc *f = __driver_init_start; f != __driver_init_end; ++f)
        (*f)();
}

void DeviceDriver::setName(const char* n)
{
    int i = 0;
    for (; i < (int)sizeof(name) - 1 && n[i]; ++i)
        name[i] = n[i];
    name[i] = '\0';
}

int DriverClass::Register(DeviceDriver* driver)
{
    if (driver == nullptr || count >= MAX_DEVICES)
        return -1;

    devices[count] = driver;
    return count++;
}

DeviceDriver* DriverClass::Get(int index)
{
    if (index < 0 || index >= count)
        return nullptr;
    return devices[index];
}

DeviceDriver* DriverClass::GetDefault()
{
    if (count == 0)
        return nullptr;
    return devices[default_index];
}

bool DriverClass::SetDefault(int index)
{
    if (index < 0 || index >= count)
        return false;
    default_index = index;
    return true;
}
