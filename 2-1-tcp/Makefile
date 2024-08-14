CC := clang
CFLAGS := -Wall -Wextra -Werror -c
LDFLAGS := -Wall -Wextra -Werror

SERVER_OBJS := tcp_server.o error_handling.o
CLIENT_OBJS := tcp_client.o error_handling.o

.PHONY: all clean

all: tcp_server tcp_client

tcp_server: $(SERVER_OBJS)
	$(CC) $(LDFLAGS) $(SERVER_OBJS) -o $@

tcp_client: $(CLIENT_OBJS)
	$(CC) $(LDFLAGS) $(CLIENT_OBJS) -o $@

%.o: %.c %.h
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f tcp_server tcp_client *.o
