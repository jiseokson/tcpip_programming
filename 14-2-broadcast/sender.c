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
    struct sockaddr_in br_addr;
    FILE *fp;
    char buffer[1024] = {0,};
    int so_brd = 1;

    if (argc != 3)
    {
        fprintf(stderr, "Usage: sender ip port\n");
        return 1;
    }

    sock = socket(PF_INET, SOCK_DGRAM, 0);
    memset(&br_addr, 0, sizeof(br_addr));
    br_addr.sin_family = AF_INET;
    br_addr.sin_addr.s_addr = inet_addr(argv[1]);
    br_addr.sin_port = htons(atoi(argv[2]));

    setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (void*)&so_brd, sizeof(so_brd));
    if ((fp = fopen("news.txt", "r")) == NULL)
    {
        perror("fopen()");
        return 1;
    }

    while (!feof(fp))
    {
        memset(buffer, 0, BUFFER_SIZE);
        fgets(buffer, BUFFER_SIZE, fp);
        sendto(sock, buffer, strlen(buffer), 0, (struct sockaddr*)&br_addr, sizeof(br_addr));
        sleep(2);
    }

    close(sock);
    fclose(fp);

    return 0;
}
