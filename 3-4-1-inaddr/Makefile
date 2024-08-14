CC := clang
CFLAGS := -Wall -Wextra -Werror -c
LDFLAGS := -Wall -Wextra -Werror

INADDR_OBJS := inaddr.o

.PHONY: all clean

all: inaddr

inaddr: $(INADDR_OBJS)
	$(CC) $(LDFLAGS) $^ -o $@

%.o: %.c %.h
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f inaddr *.o
