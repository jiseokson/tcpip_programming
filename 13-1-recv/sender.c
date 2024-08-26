#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int main(int argc, char *argv[])
{
    int sock;
    struct sockaddr_in serv_addr;
    char *message = "This is from sender";
    size_t msg_size = strlen(message);

    if (argc != 3)
    {
        fprintf(stderr, "Usage: %s ip port\n", basename(argv[0]));
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
        return  1;
    }

    send(sock, message, msg_size, 0);
    close(sock);

    return 0;
}
