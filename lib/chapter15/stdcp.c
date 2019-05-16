#include <stdlib.h>
#include <stdio.h>

#define BUF_SIZE 3

int main(void) {

    FILE *src, *des;

    src = fopen("news.txt", "r");
    des = fopen("news_cp.txt", "w");

    if (src == NULL || des == NULL) {
        printf("open file error\n");
        exit(1);
    }

    int len = 0;
    char buf[BUF_SIZE];
    while (fgets(buf, BUF_SIZE, src) != NULL) {
        fputs(buf, des);
    }

    fclose(src);
    fclose(des);
    return 0;
}