#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUFFER_SIZE 1024

ssize_t get_trimed_input(char *buffer, const size_t buffer_size, FILE *file);

int main(int argc, char *argv[])
{
    int sock;
    struct sockaddr_in serv_addr;

    char input_buffer[BUFFER_SIZE] = {0,};
    char rcv_buffer[BUFFER_SIZE] = {0,};
    ssize_t input_size;

    if (argc != 3)
    {
        fprintf(stderr, "Usage: echoclient IP PORT\n");
        return 1;
    }

    sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == -1)
    {
        perror("socket()");
        return 1;
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
    {
        perror("connect()");
        return 1;
    }

    while (1)
    {
        ssize_t rcv_size = 0, total_rcv_size = 0;

        input_size = get_trimed_input(input_buffer, BUFFER_SIZE, stdin);

        if (!strcmp(input_buffer, "q"))
            break;

        write(sock, input_buffer, input_size);

        while (total_rcv_size < input_size)
        {
            rcv_size = read(sock, rcv_buffer + total_rcv_size, BUFFER_SIZE);
            if (rcv_size == -1)
            {
                perror("read()");
                return 1;
            }
            total_rcv_size += rcv_size;

            write(STDIN_FILENO, rcv_buffer + total_rcv_size - rcv_size, rcv_size);
            putchar('\n');
        }
    }

    close(sock);

    return 0;
}

ssize_t get_trimed_input(char *buffer, const size_t buffer_size, FILE *file)
{
    size_t len;

    fgets(buffer, buffer_size, file);
    len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n')
    {
        buffer[len - 1] = '\0';
        --len;
    }
    return len;
}
