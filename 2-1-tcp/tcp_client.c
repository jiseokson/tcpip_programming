#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include "error_handling.h"

#define BUFFER_SIZE 256

int main(int argc, char *argv[])
{
    int sock, rcvd_size, total_rcvd_size = 0, i = 0;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE] = {0,};

    // check arguments
    if (argc != 3)
    {
        fprintf(stderr, "Usage: %s IP PORT\n", argv[0]);
        exit(1);
    }

    // create socket
    sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock == -1)
    {
        error_handling("socket() error");
    }

    // set server address
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    // connect to server
    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
    {
        error_handling("connect() error");
    }

    // receive data from server
    while ((rcvd_size = read(sock, buffer + i, 1)))
    {
        if (rcvd_size == -1)
        {
            error_handling("read() error");
        }
        total_rcvd_size += rcvd_size;
        ++i;
    }

    printf("Message from server(%d bytes received): \'%s\'\n", total_rcvd_size, buffer);
    close(sock);

    return 0;
}
