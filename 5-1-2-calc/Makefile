CC := clang
CFLAGS := -Wall -Wextra -Werror -c
LDFLAGS := -Wall -Wextra -Werror

CALC_SERVER_OBJS := calcserver.o recv_msg.o eval_msg.o make_msg.o
CALC_CLIENT_OBJS := calcclient.o recv_msg.o eval_msg.o make_msg.o
MSG_HDRS := msg.h

.PHONY: all clean

all: calcserver calcclient

clean:
	rm -f calcserver calcclient *.o

calcserver: $(CALC_SERVER_OBJS) $(MSG_HDRS)
	$(CC) $(LDFLAGS) $(CALC_SERVER_OBJS) -o $@

calcclient: $(CALC_CLIENT_OBJS) $(MSG_HDRS)
	$(CC) $(LDFLAGS) $(CALC_CLIENT_OBJS) -o $@

%.o: %.c
	$(CC) $(CFLAGS) $< -o $@
