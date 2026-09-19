CROSS_COMPILE ?= aarch64-linux-gnu-

CC      := $(CROSS_COMPILE)gcc
CXX     := $(CROSS_COMPILE)g++
LD      := $(CROSS_COMPILE)ld
OBJCOPY := $(CROSS_COMPILE)objcopy

CFLAGS := \
	-ffreestanding \
	-fno-builtin \
	-fno-stack-protector \
	-fno-pie \
	-fno-asynchronous-unwind-tables \
	-fno-exceptions \
	-fno-rtti \
	-O2 \
	-Wall \
	-Wextra

ASFLAGS := \
	-ffreestanding

LDFLAGS := \
	-T linker.ld \
	-nostdlib

OBJS := boot.o kernel.o

.PHONY: all clean run

all: kernel.elf kernel.bin

boot.o: boot.S
	$(CC) $(ASFLAGS) -c $< -o $@

kernel.o: kernel.cpp
	$(CXX) $(CFLAGS) -c $< -o $@

kernel.elf: $(OBJS) linker.ld
	$(LD) $(LDFLAGS) $(OBJS) -o $@

kernel.bin: kernel.elf
	$(OBJCOPY) -O binary $< $@

clean:
	rm -f *.o *.elf *.bin

run: kernel.elf
	qemu-system-aarch64 \
		-M virt \
		-cpu cortex-a53 \
		-m 512M \
		-nographic \
		-kernel kernel.elf