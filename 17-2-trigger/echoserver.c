#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/epoll.h>

#define BUFFER_SIZE 4
#define EPOLL_SIZE 50

int main(int argc, char *argv[])
{
    int serv_sock, clnt_sock;
    struct sockaddr_in serv_addr, clnt_addr;
    socklen_t clnt_addr_size;

    struct epoll_event *ep_events;
    struct epoll_event event;
    int epfd, event_cnt;

    unsigned char buffer[BUFFER_SIZE] = {0,};
    ssize_t rcv_size;

    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s port\n", basename(argv[0]));
        return 1;
    }

    serv_sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serv_sock == -1)
    {
        perror("socket()");
        return 1;
    }

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

    epfd = epoll_create(EPOLL_SIZE);
    ep_events = malloc(sizeof(struct epoll_event) * EPOLL_SIZE);

    event.events = EPOLLIN;
    event.data.fd = serv_sock;
    epoll_ctl(epfd, EPOLL_CTL_ADD, serv_sock, &event);

    while (1)
    {
        event_cnt = epoll_wait(epfd, ep_events, EPOLL_SIZE, -1);
        if (event_cnt == -1)
        {
            perror("epoll_wait()");
            break;
        }
        puts("epoll_wait() returned");

        for (int i = 0; i < event_cnt; ++i)
        {
            /* new connection request */
            if (ep_events[i].data.fd == serv_sock)
            {
                clnt_addr_size = sizeof(clnt_addr);
                clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_addr, &clnt_addr_size);
                event.events = EPOLLIN;
                event.data.fd = clnt_sock;
                epoll_ctl(epfd, EPOLL_CTL_ADD, clnt_sock, &event);
            }
            else
            {
                clnt_sock = ep_events[i].data.fd;
                rcv_size = read(clnt_sock, buffer, BUFFER_SIZE);
                if (rcv_size == 0)
                {
                    epoll_ctl(epfd, EPOLL_CTL_DEL, clnt_sock, NULL);
                    close(clnt_sock);
                }
                else
                {
                    write(clnt_sock, buffer, rcv_size);
                }
            }
        }
    }
    return 0;
}
