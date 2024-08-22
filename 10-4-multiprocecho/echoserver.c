#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/wait.h>

#define BUFFER_SIZE 1024

void read_chlidproc(int signo);

int main(int argc, char *argv[])
{
    int serv_sock, clnt_sock, opt;
    struct sockaddr_in serv_addr, clnt_addr;
    socklen_t clnt_addr_size;

    pid_t pid;
    struct sigaction act;

    char *rcv_buffer[BUFFER_SIZE] = {0,};
    ssize_t rcv_size;

    // check argument validation
    if (argc != 2)
    {
        fprintf(stderr, "Usage: echoserver PORT\n");
        return 1;
    }

    // set singal handler for SIGCHLD
    act.__sigaction_u.__sa_handler = read_chlidproc;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    sigaction(SIGCHLD, &act, 0);

    // get server socket
    serv_sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serv_sock == -1)
    {
        perror("socket()");
        return 1;
    }

    // set server address
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    // set server socket has no `time-wait`
    opt = 1;
    setsockopt(serv_sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    // bind server socket and address
    if (bind(serv_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
    {
        perror("bind()");
        return 1;
    }

    // set listening queue
    if (listen(serv_sock, 5) == -1)
    {
        perror("listen()");
        return 1;
    }

    while (1)
    {
        // accept new client connection
        clnt_addr_size = sizeof(clnt_addr);
        clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_addr, &clnt_addr_size);
        if (clnt_sock == -1)
        {
            // perror("accept()");
            continue;
        }
        printf("new client connected: %s:%hu\n", inet_ntoa(clnt_addr.sin_addr), clnt_addr.sin_port);

        // fork new echo process
        pid = fork();
        if (pid == -1)
        {
            perror("fork()");
            return 1;
        }

        // do echoing if echo process
        if (pid == 0)
        {
            close(serv_sock);
            while ((rcv_size = read(clnt_sock, rcv_buffer, BUFFER_SIZE)))
            {
                if (rcv_size == -1)
                {
                    perror("read()");
                    return 1;
                }
                write(clnt_sock, rcv_buffer, rcv_size);
            }
            close(clnt_sock);
            printf("client disconnected: %s:%hu\n", inet_ntoa(clnt_addr.sin_addr), clnt_addr.sin_port);
            return 0;
        }

        close(clnt_sock);
    }

    close(serv_sock);

    return 0;
}

void read_chlidproc(int signo)
{
    pid_t pid;
    int status;

    if (signo != SIGCHLD) return;
    pid = waitpid(-1, &status, WNOHANG);
    printf("cleared remote proc id: %d\n", pid);
}
