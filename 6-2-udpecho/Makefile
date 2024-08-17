CC := clang
CFLAGS := -Wall -Wextra -Werror -c
LDFLAGS := -Wall -Wextra -Werror

UDPSERVER_OBJS := udpechoserver.o
UDPCLIENT_OBJS := udpechoclient.o

.PHONY: all clean

all: udpechoserver udpechoclient

clean:
	rm -f udpechoserver udpechoclient *.o

udpechoserver: $(UDPSERVER_OBJS)
	$(CC) $(LDFLAGS) $(UDPSERVER_OBJS) -o $@

udpechoclient: $(UDPCLIENT_OBJS)
	$(CC) $(LDFLAGS) $(UDPCLIENT_OBJS) -o $@

%.o: %.c
	$(CC) $(CFLAGS) $< -o $@
