#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "msg.h"

void recv_msg(int sock, unsigned char buffer[], const size_t buffer_size)
{
    ssize_t rcv_size = 0, total_rcv_size = 0, expected_rcv_size = -1;

    while (expected_rcv_size == -1 || total_rcv_size < expected_rcv_size)
    {
        rcv_size = read(sock, buffer + total_rcv_size, buffer_size);
        total_rcv_size += rcv_size;
        if (rcv_size == -1)
        {
            perror("read()");
            exit(1);
        }
        if (expected_rcv_size == -1 && total_rcv_size > 0)
            expected_rcv_size = *(msg_size_t *)buffer;
    }
}
