#include <stdio.h>
#include <fcntl.h>

int main()
{
    FILE *fp;
    int fd = open("data.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1)
    {
        perror("open()");
        return 1;
    }
    fp = fdopen(fd, "w");
    fputs("Network C Programming\n", fp);
    fclose(fp);
    return 0;
}