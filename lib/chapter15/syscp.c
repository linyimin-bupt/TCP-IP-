#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#define BUF_SIZE 3

int main(void) {
    int src, des;

    src = open("news.txt", O_RDONLY);
    des = open("news_cp.txt", O_WRONLY | O_CREAT | O_TRUNC, 0775);

    int len = 0;
    char buf[BUF_SIZE];
    while ((len = read(src, buf, BUF_SIZE)) > 0) {
        write(des, buf, len);
    }

    close(src);
    close(des);
    return 0;
}