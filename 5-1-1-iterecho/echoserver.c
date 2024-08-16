#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include "error_handling.h"

#define BUFFER_SIZE 1024

int main(int argc, char *argv[])
{
    int serv_sock, clnt_sock;
    struct sockaddr_in serv_addr, clnt_addr;
    socklen_t clnt_addr_size;

    char buffer[BUFFER_SIZE] = {0,};
    ssize_t rcvd_size;

    if (argc != 2)
        error_handling("Usage: echoserver PORT");

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if (serv_sock == -1)
        error_handling("socket() error");

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    if (bind(serv_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
        error_handling("bind() error");

    if(listen(serv_sock, 5) == -1)
        error_handling("listen() error");

    for (int conn_cnt = 1; conn_cnt <= 5; ++conn_cnt)
    {
        clnt_addr_size = sizeof(clnt_addr);
        clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_addr, &clnt_addr_size);
        if (clnt_sock == -1)
            error_handling("accept() error");
        printf("Connect with #%d(%s:%d)\n", conn_cnt, inet_ntoa(clnt_addr.sin_addr), clnt_addr.sin_port);

        while ((rcvd_size = read(clnt_sock, buffer, BUFFER_SIZE - 1)))
        {
            if (rcvd_size == -1)
                break;

            buffer[rcvd_size] = '\0';
            printf("Message from #%d(%d bytes received): \'%s\'\n", conn_cnt, (int32_t)rcvd_size, buffer);

            write(clnt_sock, buffer, rcvd_size);
        }

        close(clnt_sock);
        printf("Connection with #%d(%s:%d) is terminated\n", conn_cnt, inet_ntoa(clnt_addr.sin_addr), clnt_addr.sin_port);
    }

    close(serv_sock);

    return 0;
}
