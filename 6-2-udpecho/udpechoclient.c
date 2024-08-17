#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUFFER_SIZE 1024

static void trim_buffer(uint8_t buffer[]);

int main(int argc, char *argv[])
{
    int sock;
    struct sockaddr_in serv_addr;

    uint8_t input_buffer[BUFFER_SIZE] = {0,};
    uint8_t recv_buffer[BUFFER_SIZE] = {0,};
    ssize_t recv_size;

    struct sockaddr_in addr;
    socklen_t addr_size;

    if (argc != 3)
    {
        fprintf(stderr, "Usage: echoclient IP PORT\n");
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
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    // connected UDP socket
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
    {
        perror("connect()");
        exit(1);
    }

    while (1)
    {
        printf("Message(\'q\' to quit): ");
        fgets((char *)input_buffer, BUFFER_SIZE, stdin);
        trim_buffer(input_buffer);

        if (!strcmp((char *)input_buffer, "q"))
            break;

        // unconnected UDP socket => sendto 를 통해 매번 도착지의 주소를 전달
        // sendto(sock, input_buffer, strlen((char *)input_buffer), 0, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
        // connect UDP socket => 등록된 도착지로 전달
        write(sock, input_buffer, strlen((char *)input_buffer));
        addr_size = sizeof(addr);
        recv_size = recvfrom(sock, recv_buffer, BUFFER_SIZE - 1, 0, (struct sockaddr *)&addr, &addr_size);
        recv_buffer[recv_size] = '\0';

        printf("Echo from server(%d bytes): \'%s\'\n", (int32_t)recv_size, (char *)recv_buffer);
    }

    close(sock);

    return 0;
}

static void trim_buffer(uint8_t buffer[])
{
    size_t len = strlen((char *)buffer);

    if (len > 0 && buffer[len - 1] == '\n')
        buffer[len - 1] = '\0';
}
