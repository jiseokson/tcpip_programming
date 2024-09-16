CC := clang
CFLAGS := -Wall -Wextra -Werror -c
LDFLAGS := -Wall -Wextra -Werror

TARGETS := server client

SERVER_OBJS := server.o
CLIENT_OBJS := client.o

.PHONY: all clean

all: $(TARGETS)

clean:
	rm -f $(TARGETS) *.o

server: $(SERVER_OBJS)
	$(CC) $(LDFLAGS) $^ -o $@

client: $(CLIENT_OBJS)
	$(CC) $(LDFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) $< -o $@
