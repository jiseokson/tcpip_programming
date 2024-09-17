CC := gcc
CFLAGS := -Wall -Wextra -Werror -c
LDFLAGS := -Wall -Wextra -Werror

TARGETS := echoserver echoclient
SERVER_OBJS := echoserver.o
CLIENT_OBJS := echoclient.o

.PHONY: all clean

all: $(TARGETS)

clean:
	rm -f $(TARGETS) *.o

echoserver: $(SERVER_OBJS)
	$(CC) $(LDFLAGS) $^ -o $@

echoclient: $(CLIENT_OBJS)
	$(CC) $(LDFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) $< -o $@
