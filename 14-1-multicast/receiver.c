#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUFFER_SIZE 1024

int main(int argc, char *argv[])
{
    int recv_sock;
    char rcv_buffer[BUFFER_SIZE] = {0,};
    ssize_t rcv_size;

    struct sockaddr_in addr;
    struct ip_mreq join_adr;

    if (argc != 3)
    {
        fprintf(stderr, "Usage: receiver ip port\n");
        return 1;
    }

    recv_sock = socket(PF_INET, SOCK_DGRAM, 0);
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(atoi(argv[2]));

    if (bind(recv_sock, (struct sockaddr *)&addr, sizeof(addr)) == -1)
    {
        perror("bind()");
        return 1;
    }

    join_adr.imr_multiaddr.s_addr = inet_addr(argv[1]);
    join_adr.imr_interface.s_addr = htonl(INADDR_ANY);

    setsockopt(recv_sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, (void*)&join_adr, sizeof(join_adr));

    while (1)
    {
        rcv_size = recvfrom(recv_sock, rcv_buffer, BUFFER_SIZE - 1, 0, NULL, 0);
        if (rcv_size < 0) break;
        rcv_buffer[rcv_size] = 0;
        fputs(rcv_buffer, stdout);
    }

    close(recv_sock);

    return 0;
}
