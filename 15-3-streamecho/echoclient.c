#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUFFER_SIZE 1024

int main(int argc, char *argv[])
{
    int sock;
    struct sockaddr_in serv_addr;
    FILE *rfp, *wfp;

    char input_buffer[BUFFER_SIZE] = {0,};
    char rcv_buffer[BUFFER_SIZE] = {0,};

    if (argc != 3)
    {
        fprintf(stderr, "Usage: %s ip port\n", basename(argv[1]));
        return 1;
    }

    sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock == -1)
    {
        perror("socket()");
        return 1;
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
    {
        perror("connect()");
        return 1;
    }

    wfp = fdopen(sock, "w");
    rfp = fdopen(sock, "r");

    while (1)
    {
        fgets(input_buffer, BUFFER_SIZE, stdin);
        
        if (!strcmp(input_buffer, "q\n"))
            break;

        fputs(input_buffer, wfp);
        fflush(wfp);
        fgets(rcv_buffer, BUFFER_SIZE, rfp);
        fputs(rcv_buffer, stdout);
        fflush(stdout);
    }

    fclose(wfp);
    fclose(rfp);

    return 0;
}
