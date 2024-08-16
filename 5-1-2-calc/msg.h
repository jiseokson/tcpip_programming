#pragma once

#include <sys/types.h>

typedef int32_t opr_t;

typedef int8_t msg_size_t;
typedef int8_t msg_opr_cnt_t;
typedef int8_t msg_op_t;

typedef opr_t (*op_func_t)(opr_t, opr_t);

typedef struct {
    msg_size_t size;
    msg_opr_cnt_t opr_cnt;
    msg_op_t op;
} msg_header_t;

void recv_msg(int sock, unsigned char buffer[], const size_t buffer_size);
opr_t eval_msg(unsigned char *msg);
msg_header_t make_msg_prompt(unsigned char buffer[]);
msg_header_t make_result_msg(unsigned char buffer[], opr_t result);
