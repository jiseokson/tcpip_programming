CC := clang
CFLAGS := -Wall -Wextra -Werror -c
LDFLAGS := -Wall -Wextra -Werror

TARGETS := sender recver
SENDER_OBJS := sender.o
RECVER_OBJS := recver.o

.PHONY: all clean

all: $(TARGETS)

clean:
	rm -f $(TARGETS) *.o

sender: $(SENDER_OBJS)
	$(CC) $(LDFLAGS) $^ -o $@

recver: $(RECVER_OBJS)
	$(CC) $(LDFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) $< -o $@
