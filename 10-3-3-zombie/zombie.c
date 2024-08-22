#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

void read_childproc(int signo)
{
    int status;
    pid_t pid;

    if (signo != SIGCHLD) return;
    pid = waitpid(-1, &status, WNOHANG);
    if (WIFEXITED(status))
        printf("Dead child proc(%d) sent: %d\n", pid, status);
}

void fork_and_exit(int sec, int status)
{
    pid_t pid = fork();

    if (pid == -1)
    {
        perror("fork()");
        exit(1);
    }
    if (pid == 0)
    {
        printf("I\'m child proc(%d)!\n", getpid());
        sleep(sec);
        exit(status);
    }
    printf("Create child proc: %d\n", pid);
}

int main()
{
    struct sigaction act;

    act.__sigaction_u.__sa_handler = read_childproc;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    sigaction(SIGCHLD, &act, 0);

    fork_and_exit(10, 12);
    fork_and_exit(10, 24);

    for (int i = 0; i < 5; ++i)
    {
        puts("wait...");
        sleep(5);
    }

    return 0;
}
