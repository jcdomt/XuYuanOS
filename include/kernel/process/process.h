#pragma once

#include <utils/types.h>
// #include <mutex>

#define PROCESS_LOAD_BASE 0x40000000

// 程序状态定义
enum ProcessState {
    PROCESS_STATE_STOPPED = -1, // 停止
    PROCESS_STATE_RUNNING = 0, // 正在运行
    PROCESS_STATE_READY = 1,   // 就绪
    PROCESS_STATE_BLOCKED = 2, // 阻塞
};

// 程序格式类型定义
enum ProcessFormatType {
    PROCESS_FORMAT_ELF = 0, // ELF 格式
    PROCESS_FORMAT_BIN = 1, // 二进制格式
};

typedef U32 PID_T; // 进程 ID 类型

// 气死我了，锁还要自己实现
// @TODO: 以后再实现一个简单的锁
static struct PidGenerator {
    PID_T next_pid;
    PidGenerator() : next_pid(1) {}
    PID_T generate() { 
        // lock.lock();
        next_pid++;
        // lock.unlock();
        return next_pid;
     }
    // std::mutex lock; // 保护 next_pid 的互斥锁
} pid_generator;

class Process {
    public:
    private:
        PID_T pid; // 进程 ID
        PID_T ppid; // 父进程 ID
        ProcessState state; // 进程状态
        U64 uroot; // 用户态页表根地址
        U64 entry_point; // 用户态程序入口点
        ProcessFormatType format_type; // 程序格式类型

    public:
        static Process *create_process(PID_T parent_pid = 0);

        ProcessState set_state(ProcessState new_state) {
            ProcessState old_state = state;
            state = new_state;
            return old_state;
        }
        ProcessState get_state() const { return state; }
        PID_T get_pid() const { return pid; }
        PID_T get_ppid() const { return ppid; }
        void set_uroot(U64 root) { uroot = root; }
        U64 get_uroot() const { return uroot; }
        void set_entry_point(U64 entry) { entry_point = entry; }
        U64 get_entry_point() const { return entry_point; }
        ProcessFormatType get_format_type() const { return format_type; }
        ProcessFormatType set_format_type(ProcessFormatType type) { 
            ProcessFormatType old_type = format_type;
            format_type = type;
            return old_type;
        }

        // 开始执行程序
        bool load(U64 user_image_start, U64 user_image_end);
        void start();
        
    private:
        // 读入程序实体
        U64 read(U64 user_image_start, U64 user_image_end);
        // 分配栈空间
        U64 alloc_stack(U64 uroot);
        
};