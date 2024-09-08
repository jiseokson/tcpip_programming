#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUFFER_SIZE 1024

int main(int argc, char *argv[])
{
    int send_sock;
    struct sockaddr_in mul_adr;
    const int time_live = 64;
    FILE *fp;
    char buf[BUFFER_SIZE] = {0,};

    if (argc != 3)
    {
        fprintf(stderr, "Usage: sender ip port\n");
        return 1;
    }

    send_sock = socket(PF_INET, SOCK_DGRAM, 0);
    memset(&mul_adr, 0, sizeof(mul_adr));
    mul_adr.sin_family = AF_INET;
    mul_adr.sin_addr.s_addr = inet_addr(argv[1]);
    mul_adr.sin_port = htons(atoi(argv[2]));

    setsockopt(send_sock, IPPROTO_IP, IP_MULTICAST_TTL, (void*)&time_live, sizeof(time_live));
    if ((fp = fopen("news.txt", "r")) == NULL)
    {
        perror("fopen()");
        return 1;
    }

    while (!feof(fp))
    {
        fgets(buf, BUFFER_SIZE, fp);
        sendto(send_sock, buf, strlen(buf), 0, (struct sockaddr *)&mul_adr, sizeof(mul_adr));
        sleep(2);
    }

    fclose(fp);
    close(send_sock);

    return 0;
}
