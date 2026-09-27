# user/Makefile —— 把用户程序编译成独立二进制，并内嵌进内核

USER_ELF := user.elf
USER_BIN := user.bin

# user.S 用独立的链接脚本，链接到用户虚拟地址
LDFLAGS := -T user.ld -nostdlib

.PHONY: all clean

all: $(USER_BIN) user_image.o

$(USER_ELF): user.S user.ld
	$(CC) $(ASFLAGS) -c user.S -o user_link.obj      # ★ 不叫 .o
	$(LD) $(LDFLAGS) user_link.obj -o $@

user_image.o: user_image.S $(USER_BIN)
	$(CC) $(ASFLAGS) -c user_image.S -o $@

$(USER_BIN): $(USER_ELF)
	$(OBJCOPY) -O binary $< $@

clean:
	rm -f user.o $(USER_ELF) $(USER_BIN) user_image.o