#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

static const char *get_socktype_name(int socktype)
{
    switch (socktype)
    {
    case SOCK_STREAM:
        return "SOCK_STREAM";
    case SOCK_DGRAM:
        return "SOCK_DGRAM";
    }
    return "Unknown";
}

int main()
{
    int tcp_sock, udp_sock;
    int socktype;
    socklen_t optlen;

    tcp_sock = socket(PF_INET, SOCK_STREAM, 0);
    udp_sock = socket(PF_INET, SOCK_DGRAM, 0);
    if (tcp_sock == -1 || udp_sock == -1)
    {
        perror("socket()");
        exit(1);
    }

    optlen = sizeof(socktype);
    if (getsockopt(tcp_sock, SOL_SOCKET, SO_TYPE, &socktype, &optlen) == -1)
    {
        perror("getsockopt");
        exit(1);
    }
    printf("TCP socket type: %s\n", get_socktype_name(socktype));

    optlen = sizeof(socktype);
    if (getsockopt(udp_sock, SOL_SOCKET, SO_TYPE, &socktype, &optlen) == -1)
    {
        perror("getsockopt()");
        exit(1);
    }
    printf("UDP socket type: %s\n", get_socktype_name(socktype));

    close(tcp_sock);
    close(udp_sock);

    return 0;
}
