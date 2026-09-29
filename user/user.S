.section .text
.global _user_start
_user_start:
    ldr x0, =user_message
    mov x1, #user_message_len
    mov x2, #0
    mov x8, #1            // write
    svc #0
echo_loop:
    ldr x0, =user_buffer
    mov x1, #1
    mov x8, #0            // read
    svc #0
    ldr x0, =user_buffer
    mov x1, #1
    mov x8, #1            // write
    svc #0
    b echo_loop

user_message:
    .ascii "Hello from user mode!\n"
user_message_len = . - user_message

.section .data
user_buffer:
    .skip 64