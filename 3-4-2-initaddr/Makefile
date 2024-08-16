CC := clang
CFLAGS := -Wall -Wextra -Werror -c
LDFLAGS := -Wall -Wextra -Werror

INITADDR_OBJS := initaddr.o

.PHONY: all clean

all: initaddr

initaddr: $(INITADDR_OBJS)
	$(CC) $(LDFLAGS) $^ -o $@

%.o: %.c %.h
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f initaddr *.o
