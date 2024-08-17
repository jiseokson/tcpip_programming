CC := clang
CFLAGS := -Wall -Wextra -Werror -c
LDFLAGS := -Wall -Wextra -Werror

FILETRANSSERVER_OBJS := filetransserver.o
FILETRANSCLIENT_OBJS := filetransclient.o

.PHONY: all clean

all: filetransserver filetransclient

clean:
	rm -f filetransserver filetransclient *.o

filetransserver: $(FILETRANSSERVER_OBJS)
	$(CC) $(LDFLAGS) $(FILETRANSSERVER_OBJS) -o $@

filetransclient: $(FILETRANSCLIENT_OBJS)
	$(CC) $(LDFLAGS) $(FILETRANSCLIENT_OBJS) -o $@

%.o: %.c
	$(CC) $(CFLAGS) $< -o $@
