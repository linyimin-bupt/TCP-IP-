// 同一时刻只与一个客户端相连,并提供回声服务
// 依次向5个客户端提供服务
// 客户端接收用户输入的字符串并发送到服务器
// 服务器端将收到的字符数据回传给客户端
// 服务器端和客户端的字符串回声一直执行直到客户端输入Q为止

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define BUF_SIZE 1024

void error_handling(char *message);

int main(int argc, char *argv[]) {

    char message[BUF_SIZE];
    int server_sock;

    int client_sock;

    int str_len = 0;
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;

    if (argc != 2) {
        printf("Usage: %s <port>", argv[0]);
        exit(0);
    }

    // init network address
    memset(&server_addr, 0, sizeof(server_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(atoi(argv[1]));

    // create socket

    server_sock = socket(PF_INET, SOCK_STREAM, 0);

    if (server_sock == -1) {
        error_handling("socket() error");
    }

    // bind address

    if (bind(server_sock, (const struct sockaddr *) &server_addr, sizeof(server_addr)) == -1) {
        error_handling("bind() error");
    }

    // listen connection request

    if (listen(server_sock, 5) == -1) {
        error_handling("listen() error");
    }

    // accept client request
    socklen_t client_addr_size = sizeof(client_addr);
    FILE *read_fp, *write_fp;
    for (int i = 1; i <= 5; i++) {
        client_sock = accept(server_sock, (struct sockaddr *) &client_addr, &client_addr_size);

        if (client_sock == -1){
            error_handling("accept() error");
        } else {
            printf("Connected client %d \n", i);
        }

        // IO分离, 一个写模式的FILE指针,和一个读模式的FILE指针
        read_fp = fdopen(client_sock, "r");
        write_fp = fdopen(client_sock, "w");

        if (read_fp == NULL || write_fp == NULL) {
            error_handling("fdopen error");
        }

        while (!feof(read_fp)) {
            fgets(message, BUF_SIZE, read_fp);
            fputs(message, write_fp);
            fflush(write_fp);
            printf("Message from client: %s\n", message);
        }

        fclose(read_fp);
        fclose(write_fp);
    }
    close(server_sock);
    return 0;
}

void error_handling(char *message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}