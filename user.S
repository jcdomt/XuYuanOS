.section .user_text, "ax"
.global user_test

user_test:
    // 注意：CurrentEL 只允许 EL1+ 读取，EL0 读它会陷入内核 (EC=0x00)
    // 能在 EL0 跑这条 svc 并成功返回，本身就说明处于用户态

    ldr x0, =user_message
    mov x1, #user_message_len
    // 暂时假设 x2 = 0
    mov x2, #0
    // syscall number = write
    mov x8, #1
    svc #0

echo_loop:
    // 读取一个字符到可写缓冲区
    ldr x0, =user_buffer
    mov x1, #1
    // syscall number = read
    mov x8, #0
    svc #0

    // 将读到的内容重新输出
    ldr x0, =user_buffer
    mov x1, #1
    // syscall number = write
    mov x8, #1
    svc #0
    b echo_loop


user_message:
    .ascii "Hello from user mode!\n"
user_message_len = . - user_message

.section .user_data, "aw"
user_buffer:
    .skip 64
