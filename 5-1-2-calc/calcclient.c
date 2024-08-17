#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include "msg.h"

#define BUFFER_SIZE 1024

int main(int argc, char *argv[])
{
    int sock;
    struct sockaddr_in serv_addr;

    unsigned char snd_buffer[BUFFER_SIZE] = {0,};
    unsigned char rcv_buffer[BUFFER_SIZE] = {0,};

    msg_t snd_msg, rcv_msg;
    opr_t result;

    if (argc != 3)
    {
        fprintf(stderr, "Usage: calcclient IP PORT\n");
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

    snd_msg = make_msg_prompt(snd_buffer);
    write(sock, &snd_msg, snd_msg.size);

    rcv_msg = recv_msg(sock, rcv_buffer, BUFFER_SIZE);
    result = eval_msg(rcv_msg);
    printf("Result from calc server: %d\n", result);

    close(sock);

    return 0;
}
