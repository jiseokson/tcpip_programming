CC := clang
CFLAGS := -Wall -Wextra -Werror -c
LDFLAGS := -Wall -Wextra -Werror

SERVER_OBJS := hello_server.o error_handling.o
CLIENT_OBJS := hello_client.o error_handling.o

.PHONY: all clean

all: hello_server hello_client

hello_server: $(SERVER_OBJS)
	$(CC) $(LDFLAGS) $(SERVER_OBJS) -o $@

hello_client: $(CLIENT_OBJS)
	$(CC) $(LDFLAGS) $(CLIENT_OBJS) -o $@

%.o: %.c %.h
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f hello_server hello_client *.o
