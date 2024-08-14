CC := clang
CFLAGS := -Wall -Wextra -Werror -c
LDFLAGS := -Wall -Wextra -Werror

OBJS := endianess_conv.o

.PHONY: all clean

all: endianess_conv

endianess_conv: $(OBJS)
	$(CC) $(LDFLAGS) $^ -o $@

%.o: %.c %.h
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f endianess_conv *.o
