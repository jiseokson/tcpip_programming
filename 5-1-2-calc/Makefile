CC := clang
CFLAGS := -Wall -Wextra -Werror -c
LDFLAGS := -Wall -Wextra -Werror

CALC_SERVER_OBJS := calcserver.o eval_msg.c
CALC_CLIENT_OBJS := calcclient.o eval_msg.c

.PHONY: all clean

all: calcserver calcclient

clean:
	rm -f calcserver calcclient *.o

calcserver: $(CALC_SERVER_OBJS) msg.h
	$(CC) $(LDFLAGS) $(CALC_SERVER_OBJS) -o $@

calcclient: $(CALC_CLIENT_OBJS) msg.h
	$(CC) $(LDFLAGS) $(CALC_CLIENT_OBJS) -o $@

%.o: %.c %.h
	$(CC) $(CFLAGS) $< -o $@
