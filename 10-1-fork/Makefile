CC := clang
CFLAGS := -Wall -Wextra -Werror -c
LDFLAGS := -Wall -Wextra -Werror

OBJS := forktest.o

.PHONY: all clean

all: forktest

clean:
	rm -f forktest *.o
	
forktest: $(OBJS)
	$(CC) $(LDFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) $< -o $@
