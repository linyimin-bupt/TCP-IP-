#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#define BUF_SIZE 64

void error_handler(char *message);

int main(int argc, char *argv[]) {

    if (argc != 3) {
        printf("Usage: %s <ip> <port>\n", argv[0]);
        exit(1);
    }

    int sock;

    if ((sock = socket(PF_INET, SOCK_STREAM, 0)) == -1) {
        error_handler("socket() error");
    }

    // 初始化网络
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(argv[2]));
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);

    if (connect(sock, (const struct sockaddr *) &server_addr, sizeof(server_addr)) == -1) {
        error_handler("connect() error");
    }

    char buf[BUF_SIZE];
    int str_len;
    int fd = open("./news_cp.txt", O_WRONLY | O_CREAT, 0644);
    printf("%d\n", errno);
    if (fd == -1) {
        error_handler("open file error");
    }
    while (1) {
        str_len = read(sock, buf, BUF_SIZE);
        if (str_len < BUF_SIZE) {
            write(fd, buf, str_len);
            break;
        }
        write(fd, buf, str_len);
    }

    printf("Completed copy!");
    // 结束
    write(sock, "Thank you!", strlen("Thank you!"));

    close(fd);

    close(sock);

    return 0;
}

void error_handler(char *message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}