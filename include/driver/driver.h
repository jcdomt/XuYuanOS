#pragma once

// 驱动初始化函数类型
typedef int (*DriverInitFunc)();

// 将驱动初始化函数注册到 .driver_init 段，
// 内核启动时由 driver_init_all() 统一调用，无需在 main 中显式调用
#define DRIVER_INIT(fn) \
    static DriverInitFunc __driver_init_##fn \
        __attribute__((section(".driver_init"), used)) = fn

// 遍历 .driver_init 段，调用所有已注册的驱动初始化函数
void driver_init_all();

// 设备实例的统一抽象：一个 DeviceDriver 代表一个具体设备
// 注意：不提供虚析构，驱动对象生命周期与内核相同，不要通过基类指针 delete
class DeviceDriver {
public:
    virtual int init() = 0;

    const char* getName() const { return name; }

protected:
    void setName(const char* n);

private:
    char name[32];
};

// 一类驱动的注册表：每个驱动类（字符设备、块设备...）各自持有一个实例
// 无构造函数、无成员初始值，依赖 BSS 清零，可在构造函数执行前安全使用
class DriverClass {
public:
    static const int MAX_DEVICES = 16;

    int Register(DeviceDriver* driver);
    DeviceDriver* Get(int index);
    DeviceDriver* GetDefault();
    bool SetDefault(int index);
    int Count() const { return count; }

private:
    DeviceDriver* devices[MAX_DEVICES];
    int count;
    int default_index;
};
