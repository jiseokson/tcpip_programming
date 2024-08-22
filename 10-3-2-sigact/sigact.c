#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void on_timeout(int signo)
{
    if (signo == SIGALRM)
        puts("Timeout");
    alarm(2);
}

int main()
{
    struct sigaction act;

    act.__sigaction_u.__sa_handler = on_timeout;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    sigaction(SIGALRM, &act, 0);

    alarm(2);

    for (int i = 0; i < 3; ++i)
    {
        puts("wait...");
        sleep(100);
    }

    return 0;
}
