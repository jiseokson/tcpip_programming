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

```c
ssize_t rcv_size = 0, total_rcv_size = 0, expected_rcv_size = -1;
unsigned char rcv_buffer[BUFFER_SIZE] = {0,};

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
```
메시지의 초반을 수신한 직후에는 `size` 필드를 조사해 수신되기로 기대되는 전체 크기 `expected_rcv_size`를 설정한다.
누적된 수신 크기 `total_rcv_size`가 `expected_rcv_size`에 도달할 때까지 데이터를 수신해 `rcv_buffer`에 저장한다.

```c
unsigned char rcv_buffer[BUFFER_SIZE] = {0,};
opr_t *oprs;

rcv_msg_header = *(msg_header_t *)rcv_buffer;
oprs = (int *)(rcv_buffer + sizeof(msg_header_t));
```
수신된 데이터로부터 메시지 헤더와 피연산자를 추출한다.

```c
unsigned char snd_buffer[BUFFER_SIZE] = {0,};
opr_t result;

*(msg_header_t *)snd_buffer = (msg_header_t){1, 1, '='};
*(opr_t *)(snd_buffer + sizeof(msg_header_t)) = result;
```
계산된 결과를 이용해 응답할 메시지를 완성한다.

## Client

```c
unsigned char snd_buffer[BUFFER_SIZE] = {0,};
msg_header_t snd_msg_header;
opr_t *oprs;

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
```
사용자 입력으로부터 요청 메시지를 완성한다. 완성된 요청 메시지는 서버로 전송된다.

```c
unsigned char rcv_buffer[BUFFER_SIZE] = {0,};
opr_t result;

result = *(opr_t *)(rcv_buffer + sizeof(msg_header_t));
printf("Result from calc server: %d\n", result);
```
요청에 대한 응답을 수신한 후 이로부터 결과를 추출한다.
