ARCH ?= arm64

include arch/$(ARCH)/toolchain.mk

CROSS_COMPILE ?= aarch64-linux-gnu-

CC      := $(CROSS_COMPILE)gcc
CXX     := $(CROSS_COMPILE)g++
LD      := $(CROSS_COMPILE)ld
OBJCOPY := $(CROSS_COMPILE)objcopy

TOP := $(CURDIR)

CFLAGS := \
	-ffreestanding \
	-fno-builtin \
	-fno-stack-protector \
	-fno-pie \
	-fno-asynchronous-unwind-tables \
	-fno-exceptions \
	-fno-rtti \
	-mgeneral-regs-only \
	-O2 \
	-Wall \
	-Wextra \
	-I$(TOP)/include \
	-I$(TOP)/arch/$(ARCH)/include

ASFLAGS := \
	-ffreestanding

LDFLAGS := \
	-T linker.ld \
	-nostdlib

export TOP
export ARCH
export CC CXX LD OBJCOPY
export CFLAGS ASFLAGS

# 当前目录中的源文件
CPP_SRCS := $(wildcard *.cpp)
ASM_SRCS := $(wildcard *.S)

OBJS := \
	$(CPP_SRCS:.cpp=.o) \
	$(ASM_SRCS:.S=.o)

# 自动发现所有包含 Makefile 的子目录
SUBDIRS := $(dir $(wildcard */Makefile))

.PHONY: all clean run $(SUBDIRS)

all: kernel.elf kernel.bin

# --------------------------------------------------
# 当前目录源码编译
# --------------------------------------------------

%.o: %.cpp
	$(CXX) $(CFLAGS) -MMD -MP -c $< -o $@

%.o: %.S
	$(CC) $(ASFLAGS) -c $< -o $@

# --------------------------------------------------
# 递归构建所有子目录
# --------------------------------------------------

$(SUBDIRS):
	$(MAKE) -C $@

# --------------------------------------------------
# Kernel
# --------------------------------------------------

kernel.elf: $(OBJS) $(SUBDIRS)
	$(LD) $(LDFLAGS) \
		$(OBJS) \
		$(shell find . -mindepth 2 -name '*.o') \
		-o $@

kernel.bin: kernel.elf
	$(OBJCOPY) -O binary $< $@

# --------------------------------------------------
# Clean
# --------------------------------------------------

clean:
	rm -f $(OBJS) $(OBJS:.o=.d) *.elf *.bin
	@for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir clean; \
	done

# --------------------------------------------------
# Run
# --------------------------------------------------

run: kernel.elf
	$(QEMU) $(QEMU_FLAGS) -kernel kernel.elf

-include $(OBJS:.o=.d)