#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void on_timeout(int signo)
{
    if (signo == SIGALRM)
        puts("Time out!");
    alarm(2);
}

void on_key(int signo)
{
    if (signo == SIGINT)
        puts("Ctrl + C is pressed");
}

int main()
{
    signal(SIGALRM, on_timeout);
    signal(SIGINT, on_key);

    alarm(2);
    for (int i = 0; i < 3; ++i)
    {
        puts("waiting...");
        sleep(5000);
    }

    return 0;
}
