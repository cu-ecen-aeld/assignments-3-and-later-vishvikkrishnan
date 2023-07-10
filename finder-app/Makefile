TARGET := writer

SRCS := writer.c

CC := gcc
CFLAGS := -Wall -Werror -g3

ifeq ($(CROSS_COMPILE),aarch64-none-linux-gnu-)
	CC := $(CROSS_COMPILE)$(CC)
endif

.PHONY: all
all: $(TARGET)
	
$(TARGET):	
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS)

.PHONY: clean
clean:
	rm -f $(TARGET) *.o