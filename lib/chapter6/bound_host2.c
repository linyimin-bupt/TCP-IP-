
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

    char msg1[] = "Hi!";
    char msg2[] = "I'm another UDP host!";
    char msg3[] = "Nice to meet you!";

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
    sendto(server_sock, msg1, strlen(msg1), 0, (const struct sockaddr *) &server_addr, sizeof(server_addr));
    sleep(1);
    server_addr.sin_addr.s_addr = inet_addr("10.108.211.136");
    sendto(server_sock, msg2, strlen(msg2), 0, (const struct sockaddr *) &server_addr, sizeof(server_addr));
    sendto(server_sock, msg3, strlen(msg3), 0, (const struct sockaddr *) &server_addr, sizeof(server_addr));

    close(server_sock);
    return 0;
}