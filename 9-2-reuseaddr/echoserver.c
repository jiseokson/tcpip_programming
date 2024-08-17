#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUFFER_SIZE 1024

int main(int argc, char *argv[])
{
    int serv_sock, clnt_sock, opt;
    struct sockaddr_in serv_addr, clnt_addr;
    socklen_t clnt_addr_size, optsize;

    unsigned char *rcv_buffer[BUFFER_SIZE] = {0,};
    ssize_t rcv_size = 0;

    if (argc != 2)
        exit(1);

    serv_sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serv_sock == -1)
    {
        perror("socket()");
        exit(1);
    }

    /*
     * 4-way handshake 에서 처음 FIN 요청했을 때 time-wait 없도록 설정함 (즉시 동일한 포트 이용 가능하도록)
     */
    opt = 1;
    setsockopt(serv_sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    optsize = sizeof(opt);
    if (getsockopt(serv_sock, SOL_SOCKET, SO_REUSEADDR, &opt, &optsize) == -1)
    {
        perror("getsockopt()");
        exit(1);
    }
    printf("SOL_SOCKET::SO_REUSEADDR = %d\n", opt);

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

    printf("Connect with %s:%d\n", inet_ntoa(clnt_addr.sin_addr), ntohs(clnt_addr.sin_port));

    while ((rcv_size = read(clnt_sock, rcv_buffer, BUFFER_SIZE)))
    {
        if (rcv_size == -1)
        {
            perror("read()");
            exit(1);
        }
        write(clnt_sock, rcv_buffer, rcv_size);
    }

    close(clnt_sock);
    close(serv_sock);

    return 0;
}
