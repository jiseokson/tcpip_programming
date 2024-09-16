CC := clang
CFLAGS := -Wall -Wextra -Werror -c
LDFLAGS := -Wall -Wextra -Werror

.PHONY: all clean

all: fdtost

clean:
	rm -f fdtost *.o

fdtost: fdtost.o
	$(CC) $(LDFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) $^ -o $@
