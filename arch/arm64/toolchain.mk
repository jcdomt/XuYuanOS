CROSS_COMPILE := aarch64-linux-gnu-

QEMU      := qemu-system-aarch64
QEMU_FLAGS := -M virt -cpu cortex-a53 -m 512M -nographic
