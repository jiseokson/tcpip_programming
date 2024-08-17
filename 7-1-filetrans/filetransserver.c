#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUFFER_SIZE 1024

int main(int argc, char *argv[])
{
    int serv_sock, clnt_sock;
    struct sockaddr_in serv_addr, clnt_addr;
    socklen_t clnt_addr_size;

    FILE *file;
    unsigned char file_buffer[BUFFER_SIZE] = {0,};
    unsigned char recv_buffer[BUFFER_SIZE] = {0,};
    ssize_t read_size = 0, recv_size = 0;

    if (argc != 2)
    {
        fprintf(stderr, "Usage: filetransserver PORT\n");
        exit(1);
    }

    serv_sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serv_sock == -1)
    {
        perror("socket()");
        exit(1);
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    if (bind(serv_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
    {
        perror("bind()");
        exit(1);
    }

    if (listen(serv_sock, 5) == -1)
    {
        perror("listen()");
        exit(1);
    }

    clnt_addr_size = sizeof(clnt_addr);
    clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_addr, &clnt_addr_size);
    if (clnt_sock == -1)
    {
        perror("accept()");
        exit(1);
    }

    file = fopen("server_data.txt", "rb");
    if (!file)
    {
        perror("fopen()");
        exit(1);
    }

    while ((read_size = fread(file_buffer, 1, BUFFER_SIZE, file)))
    {
        write(clnt_sock, file_buffer, read_size);
    }
    fclose(file);

    shutdown(clnt_sock, SHUT_WR);
    recv_size = read(clnt_sock, recv_buffer, BUFFER_SIZE);
    if (recv_size == -1)
    {
        perror("read()");
        exit(1);
    }
    printf("Message from client: \'%s\'\n", recv_buffer);

    close(clnt_sock);
    close(serv_sock);

    return 0;
}
