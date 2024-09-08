CC := clang
CFLAGS := -Wall -Wextra -Werror -c
LDFLAGS := -Wall -Wextra -Werror

SENDER_OBJS := sender.o
RECEIVER_OBJS := receiver.o

.PHONY: all clean

all: sender receiver

clean:
	rm -f sender receiver *.o

sender: $(SENDER_OBJS)
	$(CC) $(LDFLAGS) $^ -o $@

receiver: $(RECEIVER_OBJS)
	$(CC) $(LDFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) $< -o $@
