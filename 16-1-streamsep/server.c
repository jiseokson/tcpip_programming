#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUFFER_SIZE 1024

int main(int argc, char *argv[])
{
    int serv_sock, clnt_sock;
    socklen_t clnt_addr_size;
    struct sockaddr_in serv_addr, clnt_addr;

    FILE *writefp, *readfp;

    char rcv_buffer[BUFFER_SIZE] = {0,};

    (void)argc;

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    bind(serv_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    listen(serv_sock, 5);

    clnt_addr_size = sizeof(clnt_addr);
    clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_addr, &clnt_addr_size);

    writefp = fdopen(clnt_sock, "w");
    readfp = fdopen(clnt_sock, "r");

    fputs("Hello world!\n", writefp);
    fputs("This is from server\n", writefp);
    fflush(writefp);
    // fclose(writefp);
    shutdown(fileno(writefp), SHUT_WR);

    fgets(rcv_buffer, BUFFER_SIZE, readfp);
    fputs(rcv_buffer, stdout);
    fclose(readfp);

    return 0;
}
