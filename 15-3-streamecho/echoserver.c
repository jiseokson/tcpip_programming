#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <libgen.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUFFER_SIZE 1024

int main(int argc, char *argv[])
{
    int serv_sock, clnt_sock, opt;
    struct sockaddr_in serv_addr, clnt_addr;
    socklen_t clnt_addr_size;

    FILE *rfp, *wfp;

    char rcv_buffer[BUFFER_SIZE] = {0,};

    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s port\n", basename(argv[0]));
        return 1;
    }

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if (serv_sock == -1)
    {
        perror("socket()");
        return 1;
    }

    opt = 1;
    setsockopt(serv_sock, SOL_SOCKET, SO_REUSEADDR, (void*)&opt, sizeof(opt));

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    if (bind(serv_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
    {
        perror("bind()");
        return 1;
    }

    if (listen(serv_sock, 5) == -1)
    {
        perror("listen()");
        return 1;
    }

    clnt_addr_size = sizeof(clnt_addr);
    clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_addr, &clnt_addr_size);
    if (clnt_sock == -1)
    {
        perror("accept()");
        return 1;
    }
    
    wfp = fdopen(clnt_sock, "w");
    rfp = fdopen(clnt_sock, "r");

    while (!feof(rfp))
    {
        fgets(rcv_buffer, BUFFER_SIZE, rfp);
        fputs(rcv_buffer, wfp);
        fputs(rcv_buffer, stdout);
        fflush(wfp);
        fflush(stdout);
    }

    fclose(wfp);
    fclose(rfp);

    close(serv_sock);

    return 0;
}
