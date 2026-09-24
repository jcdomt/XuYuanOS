# user/Makefile —— 把用户程序编译成独立二进制，并内嵌进内核

USER_ELF := user.elf
USER_BIN := user.bin

# user.S 用独立的链接脚本，链接到用户虚拟地址
LDFLAGS := -T user.ld -nostdlib

.PHONY: all clean

# all: $(USER_BIN) user_image.o

# 暂时不参与编译
all: 

# 1. 汇编 + 链接成独立 ELF
$(USER_ELF): user.S user.ld
	$(CC) $(ASFLAGS) -c user.S -o user.o
	$(LD) $(LDFLAGS) user.o -o $@

# 2. 抽取成纯二进制
$(USER_BIN): $(USER_ELF)
	$(OBJCOPY) -O binary $< $@

# 3. 把二进制内嵌为对象（.incbin），供内核链接
#    user_image.o 会被顶层 Makefile 的 find 收集并链接进 kernel.elf
user_image.o: user_image.S $(USER_BIN)
	$(CC) $(ASFLAGS) -c user_image.S -o $@

clean:
	rm -f user.o $(USER_ELF) $(USER_BIN) user_image.o