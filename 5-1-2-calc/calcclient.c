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

    unsigned char rcv_buffer[BUFFER_SIZE] = {0,};
    unsigned char snd_buffer[BUFFER_SIZE] = {0,};
    ssize_t rcv_size = 0, total_rcv_size = 0, expected_rcv_size = -1;

    msg_header_t snd_msg_header;
    opr_t *oprs, result;

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

    printf("opr cnt: ");
    scanf("%hhd", &snd_msg_header.opr_cnt);
    fflush(stdin);
    printf("op(+, -, *, /): ");
    scanf("%c", &snd_msg_header.op);
    fflush(stdin);
    snd_msg_header.size = sizeof(msg_header_t) + sizeof(opr_t) * snd_msg_header.opr_cnt;

    *(msg_header_t *)snd_buffer = snd_msg_header;

    oprs = (int *)(snd_buffer + sizeof(msg_header_t));
    for (int i = 0; i < snd_msg_header.opr_cnt; ++i)
    {
        printf("op %d: ", i + 1);
        scanf("%d", oprs + i);
        fflush(stdin);
    }

    write(sock, snd_buffer, snd_msg_header.size);

    while (expected_rcv_size == -1 || total_rcv_size < expected_rcv_size)
    {
        rcv_size = read(sock, rcv_buffer + total_rcv_size, BUFFER_SIZE);
        total_rcv_size += rcv_size;
        if (rcv_size == -1)
        {
            perror("read()");
            exit(1);
        }
        if (expected_rcv_size == -1 && total_rcv_size > 0)
            expected_rcv_size = *(msg_size_t *)rcv_buffer;
    }

    result = eval_msg(rcv_buffer);
    printf("Result from calc server: %d\n", result);

    close(sock);

    return 0;
}
