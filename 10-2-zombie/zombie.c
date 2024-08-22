#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
    int status;
    pid_t pid;

    pid = fork();
    if (pid == -1)
    {
        perror("fork()");
        exit(1);
    }
    else if (pid == 0)
    {
        return 3;
    }
    printf("Child proc: %d\n", pid);

    pid = fork();
    if (pid == -1)
    {
        perror("fork()");
        exit(1);
    }
    else if (pid == 0)
    {
        return 7;
    }
    printf("Child proc: %d\n", pid);

    wait(&status);
    if (WIFEXITED(status))
        printf("Childreturn: %d\n", WEXITSTATUS(status));
    wait(&status);
    if (WIFEXITED(status))
        printf("Childreturn: %d\n", WEXITSTATUS(status));

    return 0;
}