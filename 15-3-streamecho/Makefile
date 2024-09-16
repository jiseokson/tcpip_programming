CC := clang
CFLAGS := -Wall -Wextra -Werror -g -c
LDFLAGS := -Wall -Wextra -Werror

ECHOSERVER_OBJS := echoserver.o
ECHOCLIENT_OBJS := echoclient.o

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
