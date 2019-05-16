
#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#define BUF_SIZE 64

void error_handler(char *message);

int main(int argc, char *argv[]) {

    int server_sock, client_sock;

    if (argc != 2) {
        printf("Usage: %s <port>", argv[0]);
    }

    if ((server_sock = socket(PF_INET, SOCK_STREAM, 0)) == -1) {
        error_handler("socket() error");
    }

    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;

    // 初始化网络
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(argv[1]));
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(server_sock, (const struct sockaddr *) &server_addr, sizeof(server_addr)) == -1) {
        error_handler("bind() error");
    }

    if (listen(server_sock, 5) == -1) {
        error_handler("listen() error");
    }

    socklen_t client_addr_size = sizeof(client_addr);
    client_sock = accept(server_sock, (struct sockaddr *) &client_addr, &client_addr_size);

    if (client_sock == -1) {
        error_handler("accept() error");
    }

    int fd = open("./news.txt", O_RDONLY);

    if (fd == -1) {
        error_handler("open file error");
    }

    char buf[BUF_SIZE];
    while (1) {
        int str_len = read(fd, buf, BUF_SIZE);
        if (str_len < BUF_SIZE) {
            write(client_sock, buf, str_len);
            break;
        }
        write(client_sock, buf, str_len);
    }

    shutdown(client_sock, SHUT_WR);

    // 半断开之后读取客户端发送的消息
    int str_len = read(client_sock, buf, BUF_SIZE);
    buf[str_len] = '\0';

    printf("Message from client: %s\n", buf);

    close(fd);
    close(client_sock);
    close(server_sock);

    return 0;
}

void error_handler(char *message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}