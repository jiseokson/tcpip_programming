#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/time.h>

#define BUFFER_SIZE 32

static inline time_t duration_ms(struct timeval beign, struct timeval end);

int main()
{
    int fd1, fd2;
    FILE *fp1, *fp2;
    ssize_t read_size;
    char buffer[BUFFER_SIZE];
    struct timeval begin, end;

    fd1 = open("src.txt", O_RDONLY);
    fd2 = open("dst.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);

    gettimeofday(&begin, NULL);
    while ((read_size = read(fd1, buffer, BUFFER_SIZE)) > 0)
        write(fd1, buffer, read_size);
    gettimeofday(&end, NULL);
    printf("syscpy: %ldms\n", duration_ms(begin, end));

    close(fd1);
    close(fd2);

    fp1 = fopen("src.txt", "r");
    fp2 = fopen("dst.txt", "w");

    gettimeofday(&begin, NULL);
    while (fgets(buffer, BUFFER_SIZE, fp1) != NULL)
        fputs(buffer, fp2);
    gettimeofday(&end, NULL);
    printf("stdcpy: %ldms\n", duration_ms(begin, end));

    fclose(fp1);
    fclose(fp2);

    return 0;
}

static inline long duration_ms(struct timeval begin, struct timeval end)
{
    if (end.tv_usec < begin.tv_usec)
        return (end.tv_sec - begin.tv_sec - 1) * 1000 + (end.tv_usec + 1000000 - begin.tv_usec) / 100;
    else
        return (end.tv_sec - begin.tv_sec) * 1000 + (end.tv_usec - begin.tv_usec) / 100;
}
