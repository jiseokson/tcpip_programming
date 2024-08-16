#include "msg.h"

static op_func_t extract_op_func(msg_header_t msg_header);
static opr_t op_plus(opr_t a, opr_t b);
static opr_t op_sub(opr_t a, opr_t b);
static opr_t op_mul(opr_t a, opr_t b);
static opr_t op_div(opr_t a, opr_t b);
static opr_t op_none(opr_t a, opr_t b);

opr_t eval_msg(unsigned char *msg)
{
    msg_header_t msg_header = *(msg_header_t *)msg;
    opr_t *oprs = (opr_t *)(msg + sizeof(msg_header_t));
    op_func_t op_func = extract_op_func(msg_header);
    opr_t result;

    result = oprs[0];
    for (msg_opr_cnt_t i = 1; i < msg_header.opr_cnt; ++i)
        result = op_func(result, oprs[i]);
    return result;
}

static op_func_t extract_op_func(msg_header_t msg_header)
{
    switch (msg_header.op)
    {
    case '+':
        return op_plus;
    case '-':
        return op_sub;
    case '*':
        return op_mul;
    case '/':
        return op_div;
    }
    return op_none;
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

static opr_t op_none(opr_t a, opr_t b)
{
    (void)a, (void)b;
    return 0;
}
