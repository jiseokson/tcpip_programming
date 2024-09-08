#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUFFER_SIZE 1024

int main(int argc, char *argv[])
{
    int sock;
    struct sockaddr_in addr;
    ssize_t rcv_size;
    char buffer[BUFFER_SIZE] = {0,};

    if (argc != 2)
    {
        fprintf(stderr, "Usage: receiver port\n");
        return 1;
    }

    sock = socket(PF_INET, SOCK_DGRAM, 0);
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(atoi(argv[1]));

    if (bind(sock, (struct sockaddr*)&addr, sizeof(addr)) == -1)
    {
        perror("bind()");
        return 1;
    }

    while (1)
    {
        rcv_size = recvfrom(sock, buffer, BUFFER_SIZE - 1, 0, NULL, 0);
        if (rcv_size < 0)
            break;
        buffer[rcv_size] = 0;
        fputs(buffer, stdout);
    }

    close(sock);
    
    return 0;
}
