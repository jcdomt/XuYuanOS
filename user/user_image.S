.section .user_image, "a"
.align 12
.global __user_image_start
__user_image_start:
.incbin "user/user.bin"
.global __user_image_end
__user_image_end: