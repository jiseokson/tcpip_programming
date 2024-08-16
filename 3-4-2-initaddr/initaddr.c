#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

int main()
{
    int sock;
    struct sockaddr_in serv_addr;

    /*
     * INADDR_ANY => 자동으로 IP 할당, 여러 IP 중 PORT 번호만 일치하면 수신
     */
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = 8080;

    sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock == -1)
        exit(1);

    // 소켓에 주소 연결
    if(bind(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
        exit(1);

    return 0;
}
