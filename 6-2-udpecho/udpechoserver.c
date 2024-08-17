#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUFFER_SIZE 1024

int main(int argc, char *argv[])
{
    int sock;
    struct sockaddr_in serv_addr, clnt_addr;
    socklen_t clnt_addr_size;

    uint8_t buffer[BUFFER_SIZE] = {0,};
    ssize_t recv_size;

    if (argc != 2)
    {
        fprintf(stderr, "Usage: echoserver PORT\n");
        exit(1);
    }

    sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock == -1)
    {
        perror("socket()");
        exit(1);
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    if (bind(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
    {
        perror("bind()");
        exit(1);
    }

    while (1)
    {
        clnt_addr_size = sizeof(clnt_addr);
        recv_size = recvfrom(sock, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&clnt_addr, &clnt_addr_size);
        if (recv_size == -1)
            break;
        sendto(sock, buffer, recv_size, 0, (struct sockaddr *)&clnt_addr, clnt_addr_size);
    }

    close(sock);

    return 0;
}
