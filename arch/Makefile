ARCH ?= arm64

OBJS := $(wildcard *.o)

# 只构建当前架构
SUBDIRS := $(ARCH)/

.PHONY: all clean

all: $(OBJS)
	@for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir; \
	done

%.o: %.cpp
	$(CXX) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)
	@for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir clean; \
	done