#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/time.h>

#define BUFFER_SIZE 1024

int main(int argc, char *argv[])
{
    int serv_sock, clnt_sock, fd_max;
    struct sockaddr_in serv_addr, clnt_addr;
    struct timeval timeout;
    socklen_t clnt_addr_size;
    fd_set reads, cpy_reads;

    char rcv_buffer[BUFFER_SIZE]  ={0,};
    ssize_t rcv_size;

    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s port\n", basename(argv[0]));
        return 1;
    }

    serv_sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serv_sock == -1)
    {
        perror("socket()");
        return 1;
    }

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

    FD_ZERO(&reads);
    FD_SET(serv_sock, &reads);
    fd_max = serv_sock;

    while (1)
    {
        int fd;

        cpy_reads = reads;
        timeout.tv_sec = 5;
        timeout.tv_usec = 5000;

        fd = select(fd_max + 1, &cpy_reads, NULL, NULL, &timeout);
        if (fd == -1)
        {
            perror("select()");
            break;
        }
        if (fd == 0)
            continue;

        for (int i = 0; i <= fd_max; ++i)
        {
            if (!FD_ISSET(i, &cpy_reads)) continue;

            /* new connection request */
            if (i == serv_sock)
            {
                clnt_addr_size = sizeof(clnt_addr);
                clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_addr, &clnt_addr_size);
                if (clnt_sock == -1)
                {
                    perror("accept()");
                    return 1;
                }
                FD_SET(clnt_sock, &reads);
                if (fd_max < clnt_sock) fd_max = clnt_sock;
                printf("New connection: %s:%hu\n", inet_ntoa(clnt_addr.sin_addr), clnt_addr.sin_port);
            }
            /* echo */
            else
            {
                clnt_sock = i;
                rcv_size = read(clnt_sock, rcv_buffer, BUFFER_SIZE);
                if (rcv_size == 0)
                {
                    FD_CLR(clnt_sock, &reads);
                    close(clnt_sock);
                }
                else
                {
                    write(clnt_sock, rcv_buffer, rcv_size);
                }
            }
        }
    }

    close(serv_sock);

    return 0;
}

