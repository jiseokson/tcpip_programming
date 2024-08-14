#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>

void print_in_addr_t_bytes(in_addr_t in_addr)
{
    if (in_addr == INADDR_NONE)
    {
        fprintf(stderr, "Error occured\n");
        return;
    }
    printf("address in host\'s endianess:    %#x\n", ntohl(in_addr));
    printf("address in network\'s endianess: %#x\n", in_addr);
}

int main()
{
    char dot_dec_addr1[] = "1.2.3.4";
    char dot_dec_addr2[] = "1.2.3.256"; // error

    in_addr_t in_addr_t_1;
    in_addr_t in_addr_t_2;

    struct in_addr in_addr_1;
    struct in_addr in_addr_2;

    struct in_addr in_addr_ex = {inet_addr("132.23.72.42")};
    char * dot_dec_addr_ex, *temp;

    /*
     * in_addr_t inet_addr(const char *)
     * 실패 => return = IPADDR_NONE
     * 
     * dotted decimal IPv4 주소 => in_addr_t (32bit) 타입으로 변환
     * 네트워크 바이트 오더로 변경 (big endian)
     */
    printf("in_addr_t inet_addr(const char *)\n");
    in_addr_t_1 = inet_addr(dot_dec_addr1);
    in_addr_t_2 = inet_addr(dot_dec_addr2);
    print_in_addr_t_bytes(in_addr_t_1);
    print_in_addr_t_bytes(in_addr_t_2);

    putchar('\n');

    /*
     * struct in_addr inet_aton(const char *)
     * 성공 => return = 1
     * 실패 => return = 0
     * 
     * dotted decimal IPv4 주소 => struct in_addr 타입으로 변환
     */
    printf("struct in_addr inet_aton(const char *)\n");
    if (inet_aton(dot_dec_addr1, &in_addr_1) == 0)
        fprintf(stderr, "Error occured\n");
    else
        print_in_addr_t_bytes(in_addr_1.s_addr);
    if (inet_aton(dot_dec_addr2, &in_addr_2) == 0)
        fprintf(stderr, "Error occured\n");
    else
        print_in_addr_t_bytes(in_addr_2.s_addr);

    putchar('\n');

    /*
     * char *inet_ntoa(struct in_addr)
     * 실패 => return = -1
     * 
     * struct in_addr => dotted decimal IPv4 주소 변환
     */
    temp = inet_ntoa(in_addr_ex);
    if (!temp) goto ERR;
    dot_dec_addr_ex = strdup(temp);
    if (!dot_dec_addr_ex) goto ERR;
    printf("address in dotted decimal: %s\n", dot_dec_addr_ex);
    goto DONE;
    ERR:
    fprintf(stderr, "Error occured\n");
    DONE:

    return 0;
}
