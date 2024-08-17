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
    struct sockaddr_in serv_addr;

    FILE *file;
    unsigned char recv_buffer[BUFFER_SIZE] = {0,};
    ssize_t recv_size = 0;

    if (argc != 3)
    {
        fprintf(stderr, "Usage: filetranclient IP PORT\n");
        exit(1);
    }

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

    file = fopen("recv_data.txt", "wb");
    while ((recv_size = read(sock, recv_buffer, BUFFER_SIZE)))
    {
        if (recv_size == -1)
        {
            perror("read()");
            exit(1);
        }
        fwrite(recv_buffer, 1, recv_size, file);
    }
    fclose(file);

    write(sock, "THANK YOU", strlen("THANK YOU") + 1);

    close(sock);

    return 0;
}
