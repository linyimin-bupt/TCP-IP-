#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>

#define BUF_SIZE 30

void error_handling(char *message);

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <ip> <port>", argv[0]);
        exit(1);
    }

    int sock;
    struct sockaddr_in server_addr;

    if ((sock = socket(PF_INET, SOCK_STREAM, 0)) == -1) {
        error_handling("socket() error");
    }

    // 初始化网络地址
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(argv[2]));
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);

    if (connect(sock, (const struct sockaddr *) &server_addr, sizeof(server_addr)) == -1) {
        error_handling("connect() error!");
    }

    write(sock, "123", strlen("123"));
    close(sock);
    return 0;
}

void error_handling(char *message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}