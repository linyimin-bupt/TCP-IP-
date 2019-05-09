#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>

#define BUF_SIZE 1024

void error_handling(char *message);
void write_routine(int sock, char *message);
void read_routine(int sock, char *message);

int main(int argc, char *argv[]) {

    if (argc != 3) {
        printf("Usage: %s <ip> <port>\n", argv[0]);
        exit(1);
    }

    int sock;

    struct sockaddr_in server_addr;

    char message[BUF_SIZE];

    // 初始化网络地址
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);
    server_addr.sin_port = htons(atoi(argv[2]));

    // 创建套接字
    sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        error_handling("socket() error");
    }

    // 发起连接请求
    if(connect(sock, (const struct sockaddr *) &server_addr, sizeof(server_addr)) == -1) {
        error_handling("connect() error");
    }

    // 创建子进程处理写操作

    pid_t pid = fork();

    if (pid == -1) {
        error_handling("fork() error");
    }

    // 子进程
    if (pid == 0) {
        read_routine(sock, message);
    } else {
        write_routine(sock, message);
    }
    // 父进程关闭连接
    close(sock);
    return 0;
}

void error_handling(char *message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

void write_routine(int sock, char *message) {
    while (1) {
        fgets(message, BUF_SIZE, stdin);
        if (!strcmp("q\n", message) || !strcmp("Q\n", message)) {
            // 子进程关闭连接
            shutdown(sock, SHUT_WR);
            exit(0);
        }

        write(sock, message, strlen(message));
    }
}

void read_routine(int sock, char *message) {
    while(1) {
        int str_len = read(sock, message, BUF_SIZE);
        if (str_len == 0) {
            exit(0);
        }
        message[str_len] = '\0';
        printf("Received Message: %s\n", message);
    }
}