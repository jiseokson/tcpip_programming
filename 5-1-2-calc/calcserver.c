#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include "msg_header.h"

#define BUFFER_SIZE 1024

static opr_t op_plus(opr_t a, opr_t b);
static opr_t op_sub(opr_t a, opr_t b);
static opr_t op_mul(opr_t a, opr_t b);
static opr_t op_div(opr_t a, opr_t b);

int main(int argc, char *argv[])
{
    int serv_sock, clnt_sock;
    struct sockaddr_in serv_addr, clnt_addr;
    socklen_t clnt_addr_size;

    unsigned char rcv_buffer[BUFFER_SIZE] = {0,};
    unsigned char snd_buffer[BUFFER_SIZE] = {0,};
    msg_header_t rcv_msg_header;
    ssize_t rcv_size = 0, total_rcv_size = 0, expected_rcv_size = -1;

    op_func_t op_func;
    opr_t *oprs, result;

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

    while (expected_rcv_size == -1 || total_rcv_size < expected_rcv_size)
    {
        rcv_size = read(clnt_sock, rcv_buffer + total_rcv_size, BUFFER_SIZE);
        if (rcv_size == -1)
        {
            perror("read()");
            exit(1);
        }
        if (expected_rcv_size == -1 && total_rcv_size > 0)
            expected_rcv_size = *(msg_size_t *)rcv_buffer;
        total_rcv_size += rcv_size;
    }
    
    rcv_msg_header = *(msg_header_t *)rcv_buffer;
    oprs = (int *)(rcv_buffer + sizeof(msg_header_t));
    
    switch (rcv_msg_header.op)
    {
    case '+':
        op_func = op_plus;
        break;
    case '-':
        op_func = op_sub;
        break;
    case '*':
        op_func = op_mul;
        break;
    case '/':
        op_func = op_div;
        break;
    }

    result = oprs[0];
    for (int i = 1; i < rcv_msg_header.opr_cnt; ++i)
    {
        result = op_func(result, oprs[i]);
    }

    *(msg_header_t *)snd_buffer = (msg_header_t){1, 1, '='};
    *(opr_t *)(snd_buffer + sizeof(msg_header_t)) = result;

    write(clnt_sock, snd_buffer, sizeof(msg_header_t) + sizeof(opr_t));

    close(clnt_sock);
    close(serv_sock);

    return 0;
}

static opr_t op_plus(opr_t a, opr_t b)
{
    return a + b;
}

static opr_t op_sub(opr_t a, opr_t b)
{
    return a - b;
}

static opr_t op_mul(opr_t a, opr_t b)
{
    return a * b;
}

static opr_t op_div(opr_t a, opr_t b)
{
    return a / b;
}
