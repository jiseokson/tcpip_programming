#include <stdio.h>
#include <arpa/inet.h>
#include <sys/types.h>

int main()
{
    const int16_t int16data = (int16_t)124000;
    const int32_t int32data = (int32_t)-3017;
    const int64_t int64data = (int64_t)214748346800LL;
    const int64_t pi = (int64_t)3.1415926;

    printf("%15s %#06hx\n",   "host int16:",  int16data);
    printf("%15s %#06hx\n",   "net int16:",   htons(int16data));
    printf("%15s %#010x\n",   "host int32:",  int32data);
    printf("%15s %#010x\n",   "net int32:",   htonl(int32data));
    printf("%15s %#018llx\n", "host int64:",  int64data);
    printf("%15s %#018llx\n", "net int64:",   htonll(int64data));
    printf("%15s %#018llx\n", "host double:", pi);
    printf("%15s %#018llx\n", "net double:",  htonll(pi));

    return 0;
}
