#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define BUFFER_SIZE 1024

int main()
{
    int     fds1[2], fds2[2], status;
    char    buffer[BUFFER_SIZE] = {0,};
    pid_t   pid;
    ssize_t read_size;

    if (pipe(fds1) == -1) {
        perror("pipe()");
        return 1;
    }
    if (pipe(fds2) == -1) {
        perror("pipe()");
        return 1;
    }

    pid = fork();
    if (pid == -1) {
        perror("fork()");
        return 1;
    }

    if (pid == 0) {
        char *message = "this is child speaking!!";

        close(fds1[1]);
        close(fds2[0]);

        write(fds2[1], message, strlen(message));

        read_size = read(fds1[0], buffer, BUFFER_SIZE);
        buffer[read_size] = '\0';
        printf("Child received: \'%s\'\n", buffer);

        close(fds1[0]);
        close(fds2[1]);

        return 0;
    } else {
        char *message = "this is parent speaking";

        close(fds1[0]);
        close(fds2[1]);

        write(fds1[1], message, strlen(message));

        read_size = read(fds2[0], buffer, BUFFER_SIZE);
        buffer[read_size] = '\0';
        printf("Parent received: \'%s\'\n", buffer);

        close(fds1[1]);
        close(fds2[0]);

        wait(&status);
    }

    return 0;
}
