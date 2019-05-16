#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main(void) {

    FILE *fp = fopen("news.txt", "r");

    if (fp == NULL) {
        printf("fopen error\n");
        exit(1);
    }

    int fd = fileno(fp);

    char buf[1024];

    int str_len = read(fd, buf, 1024);
    buf[str_len] = 0;

    printf("Message: %s\n", buf);

    // 使用fclose和close的作用是一样的
    fclose(fp);

    if (read(fd, buf, 1024) == -1) {
        printf("read error\n");
    }

    return 0;
}