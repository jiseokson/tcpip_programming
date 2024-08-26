#include <stdio.h>
#include <unistd.h>
#include <sys/uio.h>

#define BUFFER_SIZE 1024

int main()
{
    ssize_t rd_size;
    char buffer1[BUFFER_SIZE] = {0,};
    char buffer2[BUFFER_SIZE] = {0,};
    struct iovec vec[2] = {
        {buffer1, 5},
        {buffer2, BUFFER_SIZE},
    };

    rd_size = readv(STDIN_FILENO, vec, 2);
    printf("Read %zd byte\n", rd_size);
    printf("1st buffer: \'%s\'\n", buffer1);
    printf("2nd buffer: \'%s\'\n", buffer2);
    return 0;
}
