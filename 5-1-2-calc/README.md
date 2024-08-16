# Calc server / client

사칙연산을 요청하고 이의 결과를 받아볼 수 있는 계산기 서버와 클라이언트를 구현했다.
<img width="1249" alt="image" src="https://github.com/user-attachments/assets/993e787c-9e9a-419f-832a-2007debcd55c">

## Application protocol

서버와 클라이언트는 다음과 같은 형식의 메시지를 주고 받는다.
```
 0        8        16       24                                  56
+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+
|  size  |  #opr  |   op   |               opr 1               |               opr 2               ~
+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+
```
- `size`는 메시지의 전체 크기 바이트이다. 1 Byte 부호있는 정수형 인코딩을 사용한다.
- `#opr`은 연산에 포함되는 전체 피연산자의 개수이다. 1 Byte 부호있는 정수형 인코딩을 사용한다.
- `op`는 연산에 적용되는 연산자이다. 1 Byte ASCII 인코딩을 사용하며 `'+'`, `'-'`, `'*'`, `'/'`, `'='` 중 하나이다.
- 이후로는 `#opr`에 명시된 피연산자 개수만큼의 피연산자가 위치한다. 피연산자는 4 Byte 부호있는 정수형 인코딩을 이용한다.

`2 + 3`의 연산은 다음과 같은 요청으로 표현된다.
```
 0        8        16       24                                  56
+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+
|   11   |    2   |   '+'  |                 2                 |                 3                 |
+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+
   size     #opr      op                   opr 1                               opr 2               
```

이에 대한 결과로 서버는 다음과 같은 메시지를 응답한다.
```
 0        8        16       24
+--------+--------+--------+--------+--------+--------+--------+
|    7   |    1   |   '='  |                 5                 |
+--------+--------+--------+--------+--------+--------+--------+
   size     #opr      op                   opr 1
```

서버와 클라이언트 구현체는 다음과 같은 메시지 헤더 형식을 공유한다.

```c
typedef int32_t opr_t;

typedef int8_t msg_size_t;
typedef int8_t msg_opr_cnt_t;
typedef int8_t msg_op_t;

typedef struct {
    msg_size_t size;
    msg_opr_cnt_t opr_cnt;
    msg_op_t op;
} msg_header_t;
```

## Server
`main()` 함수의 동작을 살펴보자.
```c
recv_msg(clnt_sock, rcv_buffer, BUFFER_SIZE);
result = eval_msg(rcv_buffer);
make_result_msg(snd_buffer, result);

write(clnt_sock, snd_buffer, sizeof(msg_header_t) + sizeof(opr_t));
```
`recv_msg()` 함수로 클라이언트로부터 메시지를 수신한 뒤 `eval_msg()` 함수로 결과를 계산한다. 이후 `make_result_msg()` 함수를 이용해 응답 메시지를 완성한다.

이제 세부 함수의 동작을 살펴보자.
```c
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
```
메시지의 초반을 수신한 직후에는 `size` 필드를 조사해 수신되기로 기대되는 전체 크기 `expected_rcv_size`를 설정한다.
누적된 수신 크기 `total_rcv_size`가 `expected_rcv_size`에 도달할 때까지 데이터를 수신해 `rcv_buffer`에 저장한다.

```c
typedef opr_t (*op_func_t)(opr_t, opr_t);
static op_func_t extract_op_func(msg_header_t msg_header);

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
```
수신된 데이터로부터 메시지 헤더와 피연산자를 추출한 뒤 결과를 계산한다.

## Client
`main()` 함수의 동작을 살펴보자.
```c
snd_msg_header = make_msg_prompt(snd_buffer);
write(sock, snd_buffer, snd_msg_header.size);

recv_msg(sock, rcv_buffer, BUFFER_SIZE);
result = eval_msg(rcv_buffer);
printf("Result from calc server: %d\n", result);
```
`make_msg_prompt()` 함수를 이용해 사용자의 입력으로부터 요청 메시지를 완성한다. 요청 메시지는 함수의 인자인 `snd_buffer`에 저장된다.
`recv_msg()` 함수를 통해 수신된 메시지는 다시 `eval_msg()` 함수에 전달되고 결과를 계산한다.

`eval_msg()` 함수는 서버 구현에 이용된 함수와 같은 함수이다.
