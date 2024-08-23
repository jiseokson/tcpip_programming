#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <errno.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/wait.h>

#define BUFFER_SIZE 1024

static void read_childproc(int signo);

int main(int argc, char *argv[])
{
    int serv_sock, clnt_sock, opt;
    struct sockaddr_in serv_addr, clnt_addr;
    socklen_t clnt_addr_size;

    pid_t logger_pid, echo_pid;
    int fds[2];

    struct sigaction act;

    act.__sigaction_u.__sa_handler = read_childproc;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    sigaction(SIGCHLD, &act, 0);

    if (argc != 3)
    {
        fprintf(stderr, "Usage: echoserver PORT LOG\n");
        return 1;
    }

    serv_sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serv_sock == -1)
    {
        perror("socket()");
        return 1;
    }

    opt = 1;
    setsockopt(serv_sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    if (bind(serv_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
    {
        perror("bind()");
        return 1;
    }

    if (listen(serv_sock, 5) == -1)
    {
        perror("listen()");
        return 1;
    }

    if (pipe(fds) == -1)
    {
        perror("pipe()");
        return 1;
    }

    logger_pid = fork();
    if (logger_pid == -1)
    {
        perror("fork()");
        close(serv_sock);
        close(fds[0]);
        close(fds[1]);
        return 1;
    }

    /* logger */
    if (logger_pid == 0)
    {
        int file = open(argv[2], O_WRONLY | O_TRUNC);
        char rcv_buffer[BUFFER_SIZE] = {0,};
        ssize_t rcv_size;

        close(serv_sock);
        close(fds[1]);
        while ((rcv_size = read(fds[0], rcv_buffer, BUFFER_SIZE)))
        {
            if (rcv_size == -1)
            {
                perror("read()");
                close(fds[0]);
                close(file);
                return 1;
            }
            write(file, rcv_buffer, rcv_size);
        }
        close(fds[0]);
        close(file);
        return 0;
    }

    close(fds[0]);

    while (1)
    {
        clnt_addr_size = sizeof(clnt_addr);
        clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_addr, &clnt_addr_size);
        if (clnt_sock == -1)
        {
            // if (errno == EINTR) break;
            continue;
        }

        printf("New client connection: %s:%hu\n", inet_ntoa(clnt_addr.sin_addr), clnt_addr.sin_port);

        echo_pid = fork();
        if (echo_pid == -1)
        {
            perror("fork()");
            close(serv_sock);
            close(clnt_sock);
            close(fds[1]);
            return 1;
        }

        /* echo process */
        if (echo_pid == 0)
        {
            char rcv_buffer[BUFFER_SIZE] = {0,};
            ssize_t rcv_size;

            close(serv_sock);
            while ((rcv_size = read(clnt_sock, rcv_buffer, BUFFER_SIZE)))
            {
                if (rcv_size == -1)
                {
                    perror("read()");
                    close(clnt_sock);
                    close(fds[1]);
                    return 1;
                }
                write(clnt_sock, rcv_buffer, rcv_size);
                write(fds[1], rcv_buffer, rcv_size);
            }
            close(clnt_sock);
            close(fds[1]);
            printf("Client connection terminated: %s:%hu\n", inet_ntoa(clnt_addr.sin_addr), clnt_addr.sin_port);
            return 0;
        }

        close(clnt_sock);
    }

    close(serv_sock);
    close(fds[1]);

    return 0;
}

static void read_childproc(int signo)
{
    if (signo != SIGCHLD) return;
    waitpid(-1, NULL, WNOHANG);
}
