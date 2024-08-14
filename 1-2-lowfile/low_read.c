#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include "error_handling.h"

#define BUFFER_SIZE 256

int main()
{
    int fd;
    char buffer[BUFFER_SIZE] = {0,};

    if ((fd = open("./data.txt", O_RDONLY)) == -1)
    {
        error_handling("open() error");
    }

    if (read(fd, buffer, sizeof(buffer) - 1) == -1)
    {
        error_handling("read() error");
    }

    printf("file data: \'%s\'\n", buffer);
    close(fd);
    return 0;
}
