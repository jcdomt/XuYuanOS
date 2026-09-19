SRCS := $(wildcard *.cpp)
OBJS := $(SRCS:.cpp=.o)

# 子目录
SUBDIRS := $(dir $(wildcard */Makefile))

.PHONY: all clean

all: $(OBJS)
	@for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir; \
	done

%.o: %.cpp
	$(CXX) $(CFLAGS) -MMD -MP -c $< -o $@

%.o: %.S
	$(CC) $(ASFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(OBJS:.o=.d)
	@for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir clean; \
	done