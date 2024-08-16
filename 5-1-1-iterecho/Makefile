CC := clang
CFLAGS := -Wall -Wextra -Werror -c
LDFLAGS := -Wall -Wextra -Werror

ECHOSERVER_OBJS := echoserver.o error_handling.o
ECHOCLIENT_OBJS := echoclient.o error_handling.o

.PHONY: all clean

all: echoserver echoclient

clean:
	rm echoserver echoclient *.o

echoserver: $(ECHOSERVER_OBJS)
	$(CC) $(LDFLAGS) $^ -o $@

echoclient: $(ECHOCLIENT_OBJS)
	$(CC) $(LDFLAGS) $^ -o $@

%.o: %.c %.h
	$(CC) $(CFLAGS) $< -o $@
