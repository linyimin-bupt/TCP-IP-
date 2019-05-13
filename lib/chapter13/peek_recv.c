#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <signal.h>
#include <string.h>

#define BUF_SIZE 1024

void error_handling(char *message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <port>\n", argv[0]);
        exit(1);
    }

    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;

    int server_sock;
    int client_sock;

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

    int str_len = 0;
    char message[BUF_SIZE];
    while (1) {
        str_len = recv(client_sock, message, BUF_SIZE, MSG_PEEK | MSG_DONTWAIT);
        if (str_len > 0) {
            break;
        }
    }

    message[str_len] = '\0';
    printf("Buffering %d bytes: %s \n", str_len, message);

    str_len = read(client_sock, message, BUF_SIZE);
    message[str_len] = '\0';
    printf("Read agin: %s\n", message);
    close(client_sock);
    close(server_sock);
    return 0;
}