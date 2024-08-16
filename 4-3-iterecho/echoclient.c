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
    int sock;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE] = {0,};
    char rcv_buffer[BUFFER_SIZE] = {0,};
    ssize_t input_size = 0, rcvd_size = 0;

    if (argc != 3)
        error_handling("Usage: echoclient IP PORT");

    sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock == -1)
        error_handling("socket() error");

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
        error_handling("connect() error");

    while (1)
    {
        printf("Message(\'q\' to quit): ");
        fgets(buffer, BUFFER_SIZE, stdin);

        // line ending ('\n') => '\0'
        input_size = strlen(buffer);
        if (input_size > 0 && buffer[input_size - 1] == '\n')
        {
            buffer[input_size - 1] = '\0';
            input_size -= 1;
        }

        if (!strcmp(buffer, "q"))
            break;

        write(sock, buffer, input_size);

        rcvd_size = read(sock, rcv_buffer, BUFFER_SIZE - 1);
        if (rcvd_size == -1)
            error_handling("read() error");

        rcv_buffer[rcvd_size] = '\0';
        printf("Echo from server(%d bytes received): %s\n", (int32_t)rcvd_size, rcv_buffer);
    }

    close(sock);

    return 0;
}

