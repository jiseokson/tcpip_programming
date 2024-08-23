#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/wait.h>

#define BUFFER_SIZE 1024

static void read_routine(int sock);
static void write_routine(int sock);

int main(int argc, char *argv[])
{
    int sock;
    struct sockaddr_in serv_addr;

    pid_t pid;

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

    pid = fork();
    if (pid == -1)
    {
        perror("fork()");
        close(sock);
        return 1;
    }

    if (pid == 0) read_routine(sock);
    else write_routine(sock);

    close(sock);

    return 0;
}

static void read_routine(int sock)
{
    char rcv_buffer[BUFFER_SIZE] = {0,};
    ssize_t rcv_size = 0;

    while ((rcv_size = read(sock, rcv_buffer, BUFFER_SIZE)))
    {
        if (rcv_size == -1)
        {
            perror("read()");
            return;
        }
        write(STDOUT_FILENO, rcv_buffer, rcv_size);
        write(STDOUT_FILENO, "\n", 1);
    }
}

static void write_routine(int sock)
{
    char input_buffer[BUFFER_SIZE] = {0,};
    ssize_t input_size = 0;

    while (1)
    {
        fgets(input_buffer, BUFFER_SIZE, stdin);
        input_size = strlen(input_buffer);
        if (input_size > 0 && input_buffer[input_size - 1] == '\n')
        {
            input_buffer[input_size - 1] = '\0';
            --input_size;
        }
        if (!strcmp(input_buffer, "q"))
        {
            return;
        }
        write(sock, input_buffer, input_size);
    }
}
