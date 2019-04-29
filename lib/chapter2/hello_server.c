
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
void error_handling(char *message) {
    fputs (message, stderr);
    fputc('\n', stderr);
    exit(1);
}

int main(int argc, char *argv[]) {
    int serv_sock;
    int clnt_sock;

    socklen_t clnt_addr_size;
    // 服务器IP信息
    struct sockaddr_in serv_addr;
    // 客户端信息
    struct sockaddr_in clnt_addr;
    char message[] = "Hello World!";
    int str_len;

    if (argc != 2) {
        printf("Usage: %s <port>\n", argv[0]);
        exit(1);
    }

    // 创建socket
    serv_sock = socket(PF_INET, SOCK_STREAM, 0);

    if (serv_sock == -1) {
        error_handling("socket() error");
    }

    // 初始化
    memset(&serv_addr, 0, sizeof(serv_addr));

    serv_addr.sin_addr.s_addr = htons(INADDR_ANY);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(atoi(argv[1]));

    if (bind(serv_sock, (const struct sockaddr *) &serv_addr, sizeof(serv_addr)) == -1) {
        error_handling("bind() error");
    }

    if (listen(serv_sock, 5) == -1) {
        error_handling("listen() error");
    }

    clnt_addr_size = sizeof(clnt_addr);

    // 接收来自客户端的连接请求
    clnt_sock = accept(serv_sock, (struct sockaddr *) &clnt_addr, &clnt_addr_size);

    if (clnt_sock == -1) {
        error_handling("accept() error");
    }
    int count = 0;
    while(1) {
        int len = write(clnt_sock, message, sizeof(message) - 1);
        printf("len = %d\n", len);
        count ++;
        if (count > 10) {
            break;
        }
    }

    close(clnt_sock);
    close(serv_sock);

    return 0;
}