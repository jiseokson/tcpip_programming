#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUFFER_SIZE 1024

int main(int argc, char *argv[])
{
    int serv_sock, clnt_sock;
    struct sockaddr_in serv_addr, clnt_addr;
    socklen_t clnt_addr_size;

    char rcv_buffer[BUFFER_SIZE] = {0,};
    ssize_t rcv_size = 0;

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
    serv_addr.sin_addr.s_addr = INADDR_ANY;
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
    
    clnt_addr_size = sizeof(clnt_addr);
    clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_addr, &clnt_addr_size);
    if (clnt_sock == -1)
    {
        perror("accept()");
        return 1;
    }

    while (1)
    {
        rcv_size = recv(clnt_sock, rcv_buffer, BUFFER_SIZE, MSG_PEEK | MSG_DONTWAIT);
        if (rcv_size > 0) break;
    }

    printf("Buffering %zd byte\n", rcv_size);

    recv(clnt_sock, rcv_buffer, BUFFER_SIZE, 0);
    printf("Buffered data: \'%s\'\n", rcv_buffer);

    close(clnt_sock);
    close(serv_sock);

    return 0;
}
