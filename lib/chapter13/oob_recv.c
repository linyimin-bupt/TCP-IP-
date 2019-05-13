#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <signal.h>
#include <string.h>

#define BUF_SIZE 1024

int server_sock;
int client_sock;

void error_handling(char *message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

void urg_handler(int signo);

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <port>\n", argv[0]);
        exit(1);
    }

    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;

    socklen_t client_addr_size = sizeof(client_addr);

    if ((server_sock = socket(PF_INET, SOCK_STREAM, 0)) == -1) {
        error_handling("socket() error");
    }

    // 网络地址初始化
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(atoi(argv[1]));

    if (bind(server_sock, (const struct sockaddr *) &server_addr, sizeof(server_addr)) == -1) {
        error_handling("bind() error");
    }

    if (listen(server_sock, 5) == -1) {
        error_handling("listen() error");
    }

    if ((client_sock = accept(server_sock, (struct sockaddr *) &client_addr, &client_addr_size)) == -1) {
        error_handling("accept() error!");
    }

    struct sigaction act;
    act.sa_handler = urg_handler;
    act.sa_flags = 0;
    sigemptyset(&act.sa_mask);
    fcntl(client_sock, F_SETOWN, getpid());

    sigaction(SIGURG, &act, NULL);

    int str_len = 0;
    char message[BUF_SIZE];
    while ((str_len = recv(client_sock, message, BUF_SIZE, 0)) != 0) {
        if (str_len == -1) {
            continue;
        }
        message[str_len] = '\0';
        puts(message);
    }
    close(client_sock);
    close(server_sock);
    return 0;
}

void urg_handler(int signo) {
    int str_len = 0;
    char buf[BUF_SIZE];
    str_len = recv(client_sock, buf, BUF_SIZE, MSG_OOB);
    buf[str_len] = '\0';
    printf("Urgent message: %s \n", buf);
}