#include "msg.h"

#include <stdio.h>

static op_func_t extract_op_func(msg_op_t op);
static opr_t op_plus(opr_t a, opr_t b);
static opr_t op_sub(opr_t a, opr_t b);
static opr_t op_mul(opr_t a, opr_t b);
static opr_t op_div(opr_t a, opr_t b);
static opr_t op_none(opr_t a, opr_t b);

opr_t eval_msg(msg_t msg)
{
    op_func_t op_func = extract_op_func(msg.op);
    opr_t result;

    result = msg.oprs[0];
    for (msg_opr_cnt_t i = 1; i < msg.opr_cnt; ++i)
        result = op_func(result, msg.oprs[i]);
    return result;
}

static op_func_t extract_op_func(msg_op_t op)
{
    switch (op)
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
