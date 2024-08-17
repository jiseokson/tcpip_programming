CC := clang
CFLAGS := -Wall -Wextra -Werror -c
LDFLAGS := -Wall -Wextra -Werror

GETHOSTNAME_OBJS := gethostname.o

.PHONY: all clean

all: gethostname

clean:
	rm -f  *.o

gethostname: $(GETHOSTNAME_OBJS)
	$(CC) $(LDFLAGS) $(GETHOSTNAME_OBJS) -o $@

%.o: %.c
	$(CC) $(CFLAGS) $< -o $@

