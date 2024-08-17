#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 1024

static void print_host(struct hostent *host)
{
    printf("Canonical name: %s\n", host->h_name);
    printf("Address type: %s\n", host->h_addrtype == AF_INET ? "AF_INET" : "AF_INET6");
    for (int i = 0; host->h_aliases[i] != NULL; ++i)
        printf("Alias %d: %s\n", i + 1, host->h_aliases[i]);
    for (int i = 0; host->h_addr_list[i] != NULL; ++i)
        printf("Address %d: %s\n", i + 1,
            inet_ntoa(*(struct in_addr *)host->h_addr_list[i]));
}

int main()
{
    unsigned char input_buffer[BUFFER_SIZE] = {0,};
    struct hostent *host;
    struct in_addr addr;

    printf("Hostname: ");
    scanf("%s", input_buffer);

    host = gethostbyname((const char *)input_buffer);
    if (!host)
    {
        herror("gethostbyname()");
        exit(1);
    }
    print_host(host);

    // Hostname: www.youtube.com
    // Canonical name: youtube-ui.l.google.com
    // Address type: AF_INET
    // Alias 1: www.youtube.com
    // Address 1: 142.250.207.110
    // Address 2: 142.250.206.206
    // Address 3: 172.217.175.110
    // Address 4: 172.217.161.238
    // Address 5: 142.250.206.238
    // Address 6: 172.217.25.174
    // Address 7: 172.217.26.238
    // Address 8: 142.250.76.142
    // IP address: 172.217.161.238

    memset(input_buffer, 0, sizeof(BUFFER_SIZE));
    printf("IP address: ");
    scanf("%s", input_buffer);

    addr.s_addr = inet_addr((const char *)input_buffer);
    host = gethostbyaddr((const void *)&addr, 4, AF_INET);
    if (!host)
    {
        herror("gethostbyaddr()");
        exit(1);
    }
    print_host(host);

    // IP address: 172.217.161.238
    // Canonical name: kix06s05-in-f14.1e100.net
    // Address type: AF_INET
    // Alias 1: 238.161.217.172.in-addr.arpa
    // Address 1: 172.217.161.238

    return 0;
}
