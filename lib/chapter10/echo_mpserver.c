#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <signal.h>
#include <wait.h>
void error_handling(char *message);
void read_child_proc(int sig);

#define BUF_SIZE 30

int main(int argc, char *argv[]) {

    if (argc != 2) {
        printf("Usage: %s <port>", argv[0]);
        exit(1);
    }

    int server_sock;
    int client_sock;

    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;

    char message[BUF_SIZE];

    // 创建套接字
    server_sock = socket(PF_INET, SOCK_STREAM, 0);

    if (server_sock == -1) {
        error_handling("socket() error");
    }

    // 初始化网络地址
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(atoi(argv[1]));
    server_addr.sin_family = AF_INET;

    // 绑定服务器地址和端口
    if (bind(server_sock, (const struct sockaddr *) &server_addr, sizeof(server_addr)) == -1) {
        error_handling("bind() error.");
    }

    // 被动监听连接请求
    if (listen(server_sock, 5) == -1) {
        error_handling("listen(0 error");
    }

    // 循环接受链接请求并每个请求创建一个子进程进行处理
    socklen_t client_addr_size = sizeof(client_addr);
    while (1) {
        client_sock = accept(server_sock, (struct sockaddr *) &client_addr, &client_addr_size);
        if (client_sock == -1) {
            continue;
        }

        // 注册终止事件, 避免产生大量的僵尸进程
        struct sigaction act;
        act.sa_handler = read_child_proc;
        sigemptyset(&act.sa_mask);
        act.sa_flags = 0;

        sigaction(SIGCHLD, &act, 0);

        // 创建子进程处理连接请求
        pid_t pid = fork();
        // 创建失败
        if (pid == -1) {
            error_handling("fork() error");
            close(client_sock);
        }
        // 子进程
        if (pid == 0) {
            close(server_sock);
            // 接收客户端发送过来的数据
            int rev_len = 0;
            int str_len = 0;
            while ((rev_len = read(client_sock, message, BUF_SIZE)) != 0) {
                str_len += rev_len;
                // 回传数据
                write(client_sock, message, rev_len);
            }
            close(client_sock);
            printf("Received data length: %d\n", str_len);
            fputs("client unconnected ...", stdout);
            fputc('\n', stdout);
            // 终止子进程
            exit(0);
        } else {
            close(client_sock);
        }
    }
    close(server_sock);
    return 0;
}

void error_handling(char *message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

void read_child_proc(int sig) {
    int status;
    if (sig == SIGCHLD) {
        pid_t pid = wait(&status);
        printf("child pid: %d \n", pid);

        if (WIFEXITED(status)) {
            printf("child send: %d\n", WEXITSTATUS(status));
        }
    }
}