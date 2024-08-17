#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUFFER_SIZE 1024

static void trim_lineend(char *buffer, size_t *len);

int main(int argc, char *argv[])
{
    int sock;
    struct sockaddr_in serv_addr;

    char snd_buffer[BUFFER_SIZE] = {0,};
    char rcv_buffer[BUFFER_SIZE] = {0,};
    ssize_t rcv_size = 0, total_rcv_size = 0, expected_rcv_size = 0;

    if (argc != 3)
        exit(1);

    sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == -1)
    {
        perror("socket()");
        exit(1);
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
    {
        perror("connect()");
        exit(1);
    }

    while (1)
    {
        printf("Message(\'q\' to quit): ");
        fgets(snd_buffer, BUFFER_SIZE, stdin);

        expected_rcv_size = strlen(snd_buffer);
        trim_lineend(snd_buffer, (size_t *)&expected_rcv_size);

        if (!strcmp(snd_buffer, "q"))
            break;

        write(sock, snd_buffer, expected_rcv_size);

        while (total_rcv_size < expected_rcv_size)
        {
            rcv_size = read(sock, rcv_buffer, BUFFER_SIZE - 1);
            if (rcv_size == -1)
            {
                perror("read()");
                exit(1);
            }
            total_rcv_size += rcv_size;
            rcv_buffer[rcv_size] = '\0';
            printf("Message from echo(%zd byte): \'%s\'\n", rcv_size, rcv_buffer);
        }
    }

    close(sock);

    return 0;
}

static void trim_lineend(char *buffer, size_t *len)
{
    if (*len > 0 && buffer[*len - 1] == '\n')
    {
        buffer[*len - 1] = '\0';
        --len;
    }
}
