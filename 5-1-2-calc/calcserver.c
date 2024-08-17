#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include "msg.h"

#define BUFFER_SIZE 1024

int main(int argc, char *argv[])
{
    int serv_sock, clnt_sock;
    struct sockaddr_in serv_addr, clnt_addr;
    socklen_t clnt_addr_size;

    unsigned char rcv_buffer[BUFFER_SIZE] = {0,};
    unsigned char snd_buffer[BUFFER_SIZE] = {0,};

    msg_t rcv_msg, snd_msg;
    opr_t result;

    if (argc != 2)
    {
        fprintf(stderr, "Usage: calcserver PORT\n");
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

    rcv_msg = recv_msg(clnt_sock, rcv_buffer, BUFFER_SIZE);
    result = eval_msg(rcv_msg);
    snd_msg = make_result_msg(snd_buffer, result);
    write(clnt_sock, &snd_msg, snd_msg.size);

    close(clnt_sock);
    close(serv_sock);

    return 0;
}
