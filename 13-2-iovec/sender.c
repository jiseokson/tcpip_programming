#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/uio.h>

int main()
{
    char *msg1 = "This is 1st message\n";
    char *msg2 = "This is 2nd message\n";
    struct iovec vec[2] = {
        {msg1, strlen(msg1)},
        {msg2, strlen(msg2)},
    };
    ssize_t wr_size;

    wr_size = writev(STDIN_FILENO, vec, sizeof(vec) / sizeof(struct iovec));
    printf("Write %zd byte\n", wr_size);

    return 0;
}
