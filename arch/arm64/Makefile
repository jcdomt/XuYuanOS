SRCS := $(wildcard *.S)
OBJS := $(SRCS:.S=.o)

# 子目录
SUBDIRS := $(wildcard */)

.PHONY: all clean

all: $(OBJS)
	@for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir; \
	done

%.o: %.S
	$(CC) $(ASFLAGS) -c $< -o $@

%.o: %.cpp
	$(CXX) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)
	@for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir clean; \
	done