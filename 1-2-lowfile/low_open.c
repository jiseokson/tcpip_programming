#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include "error_handling.h"

int main()
{
    int fd;
    char buffer[] = "Let\'s go!";

    fd = open("./data.txt", O_CREAT|O_WRONLY|O_TRUNC);
    if (fd == -1)
    {
        error_handling("open() error");
    }
    printf("file descriptor: %d\n", fd);

    if (write(fd, buffer, sizeof(buffer) - 1) == -1)
    {
        error_handling("write() error");
    }

    close(fd);
    return 0;
}
