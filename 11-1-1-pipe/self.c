#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

int main()
{
    pid_t pid;
    int fds[2];
    char *message = "Message from child";
    char buffer[BUFFER_SIZE] = {0,};
    ssize_t read_size;

    if (pipe(fds) == -1)
    {
        perror("pipe()");
        return 1;
    }

    pid = fork();
    if (pid == -1)
    {
        perror("fork()");
        close(fds[0]);
        close(fds[1]);
        return 1;
    }

    if (pid == 0)
    {
        close(fds[0]);
        write(fds[1], message, strlen(message));
        close(fds[1]);
        return 0;
    }
    else
    {
        close(fds[1]);
        read_size = read(fds[0], buffer, BUFFER_SIZE);
        close(fds[0]);
        buffer[read_size] = '\0';
        printf("Parent received: \'%s\'\n", buffer);
    }

    return 0;
}
