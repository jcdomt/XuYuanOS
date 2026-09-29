SRCS := $(wildcard *.cpp)
OBJS := $(SRCS:.cpp=.o)

# 子目录
SUBDIRS := $(wildcard */)

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