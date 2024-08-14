CC := clang
CFLAGS := -Wall -Wextra -Werror -c
LDFLAGS := -Wall -Wextra -Werror

LOW_OPEN_OBJS := low_open.o error_handling.o
LOW_READ_OBJS := low_read.o error_handling.o

.PHONY: all clean

all: low_open low_read

low_open: $(LOW_OPEN_OBJS)
	$(CC) $(LDFLAGS) $^ -o $@

low_read: $(LOW_READ_OBJS)
	$(CC) $(LDFLAGS) $^ -o $@

%.o: %.c %.h
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f low_open low_read *.o
