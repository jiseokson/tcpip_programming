#include <stdio.h>

#include "msg.h"

msg_header_t make_msg_header(unsigned char buffer[], msg_opr_cnt_t opr_cnt, msg_op_t op)
{
    msg_size_t msg_size = sizeof(opr_t) * opr_cnt;

    return *(msg_header_t *)buffer = (msg_header_t){msg_size, opr_cnt, op};
}

msg_header_t make_result_msg(unsigned char buffer[], opr_t result)
{
    *(opr_t *)(buffer + sizeof(msg_header_t)) = result;
    return make_msg_header(buffer, 1, '=');
}

msg_header_t make_msg_prompt(unsigned char buffer[])
{
    msg_opr_cnt_t opr_cnt;
    msg_op_t op;
    opr_t *oprs;

    printf("opr cnt: ");
    scanf("%hhd", &opr_cnt);
    fflush(stdin);

    printf("op(+, -, *, /): ");
    scanf("%c", &op);
    fflush(stdin);

    oprs = (int *)(buffer + sizeof(msg_header_t));
    for (int i = 0; i < opr_cnt; ++i)
    {
        printf("op %d: ", i + 1);
        scanf("%d", oprs + i);
        fflush(stdin);
    }

    return make_msg_header(buffer, opr_cnt, op);
}
