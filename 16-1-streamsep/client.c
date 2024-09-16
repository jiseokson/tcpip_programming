#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUFFER_SIZE 1024

int main(int argc, char *argv[])
{
    int sock;
    struct sockaddr_in serv_addr;
    FILE *writefp, *readfp;
    char buffer[BUFFER_SIZE] = {0,};

    (void)argc;

    sock = socket(PF_INET, SOCK_STREAM, 0);
    
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    writefp = fdopen(sock, "w");
    readfp = fdopen(sock, "r");

    while (1)
    {
        if (fgets(buffer, BUFFER_SIZE, readfp) == NULL)
            break;
        fputs(buffer, stdout);
        fflush(stdout);
    }

    fputs("This is from client!\n", writefp);
    fflush(writefp);

    fclose(writefp);
    fclose(readfp);
    
    return 0;
}
