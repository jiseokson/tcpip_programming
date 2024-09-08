CC := clang
CFLAGS := -Wall -Wextra -Werror -c
LDFLAGS := -Wall -Wextra -Werror

.PHONY: all clean

all: cpy

clean:
	rm -f cpy *.o

syscpy: cpy.o
	$(CC) $(LDFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) $^ -o $@
