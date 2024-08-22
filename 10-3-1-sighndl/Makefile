CC := clang
CFLAGS := -Wall -Wextra -Werror -c
LDFLAGS := -Wall -Wextra -Werror

OBJS := sighndl.o

.PHONY: all clean

all: sighndl

clean:
	rm -f sighndl *.o
	
sighndl: $(OBJS)
	$(CC) $(LDFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) $< -o $@
