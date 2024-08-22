CC := clang
CFLAGS := -Wall -Wextra -Werror -c
LDFLAGS := -Wall -Wextra -Werror

TARGET := sigact
OBJS := $(TARGET).o

.PHONY: all clean

all: $(TARGET)

clean:
	rm -f $(TARGET) *.o
	
$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) $< -o $@
