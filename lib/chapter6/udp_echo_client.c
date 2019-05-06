
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

#define BUF_SIZE 30
void error_handling(char *message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(0);
}
int main(int argc, char *argv[]) {
    int server_sock;

    char message[BUF_SIZE];

    if (argc != 3) {
        printf("Usage: %s <ip> <port>\n", argv[0]);
        exit(1);
    }

    // 创建基于UPD的socket
    server_sock = socket(PF_INET, SOCK_DGRAM, 0);
    if (server_sock == -1) {
        error_handling("socket() error");
    }

    struct sockaddr_in server_addr;     // 接收方地址

    struct sockaddr_in client_addr;     // 存放发送方地址, 这个地址由recvfrom函数接收信息时获取,不需要我们自己指定, 指定了也没有用处

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(argv[2]));
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);

    int str_len = 0; // 收到信息的长度
    socklen_t client_addr_size = sizeof(client_addr);
    while(1) {
        fputs("Insert message(q to quit): ", stdout);
        fgets(message, sizeof(message), stdin);
        if (!strcmp(message, "q\n") || !strcmp(message, "Q\n")) {
            break;
        }
        sendto(server_sock, message, strlen(message), 0, (const struct sockaddr *) &server_addr, sizeof(server_addr));
        str_len = recvfrom(server_sock, message, BUF_SIZE - 1, 0, (struct sockaddr *) &client_addr, &client_addr_size);

        message[str_len] = '\0';

        printf("Message from server: %s", message);
        printf("server's info: \n");
        printf("IP: %s, port: %hu\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
    }
    close(server_sock);
    return 0;
}