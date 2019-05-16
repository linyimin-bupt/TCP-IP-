#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>

int main(void) {
    int fd;

    fd = open("data.txt", O_RDWR | O_CREAT, 0755);

    if (fd == -1) {
        printf("open file error\n");
        exit(1);
    }

    // 一个文件描述符创建多个FILE指针
    FILE *wfp = fdopen(fd, "w");
    FILE *duwfp = fdopen(fd, "w");

    // 同时创建一个读模式的FILE指针
    FILE *rfp = fdopen(fd, "r");


    if (wfp == NULL || duwfp == NULL) {
        printf("translate error\n");
        exit(1);
    }

    fputs("first", wfp);
    fclose(wfp);

    // 测试关闭掉一个FILE指针之后,是否依旧可以写入数据
    fputs("second", duwfp);

    fclose(duwfp);

    // 测试全部FILE指针关闭之后,是否仍可以将文件描述符转换成FILE指针
    FILE *after_fp = fdopen(fd, "w");

    if (after_fp == NULL) {
        printf("translate error\n");
    }
    return 0;
}