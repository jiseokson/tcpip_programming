#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int gval = 10;

int main()
{
    pid_t pid;
    int lval = 20;

    printf("Init value: %d, %d\n", gval, lval);

    pid = fork();
    if (pid == -1)
    {
        perror("fork()");
        exit(1);
    }
    else if (pid == 0)
    {
        gval += 2, lval += 2;
    }
    else
    {
        gval -= 2, lval -= 2;
    }

    if (pid == 0)
    {
        printf("Child proc: %d, %d\n", gval, lval);
    }
    else
    {
        printf("Parent proc: %d, %d\n", gval, lval);
    }

    return 0;
}
