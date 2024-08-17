CC := clang
CFLAGS := -Wall -Wextra -Werror -c
LDFLAGS := -Wall -Wextra -Werror

GETSOCKOPT_OBJS := getsockopt.o

.PHONY: all clean

all: getsockopt

clean:
	rm -f getsockopt *.o

getsockopt: $(GETSOCKOPT_OBJS)
	$(CC) $(LDFLAGS) $(GETSOCKOPT_OBJS) -o $@

%.o: %.c
	$(CC) $(CFLAGS) $< -o $@
