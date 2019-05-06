
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
    if (argc != 2) {
        printf("Usage: %s <port>\n", argv[0]);
        exit(1);
    }

    // 创建基于UPD的socket
    server_sock = socket(PF_INET, SOCK_DGRAM, 0);
    if (server_sock == -1) {
        error_handling("socket() error");
    }

    struct sockaddr_in server_addr;
    struct sockaddr_in from_addr;

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(argv[1]));

//    // 分配IP地址和端口
    if (bind(server_sock, (const struct sockaddr *) &server_addr, sizeof(server_addr)) == -1) {
        error_handling("bind() error");
    }

    int str_len = 0;
    socklen_t from_addr_size = sizeof(from_addr);
    while (1) {
        // 接收到消息后,recvfrom函数会将发送方的地址写入from_addr
        str_len = recvfrom(server_sock, message, BUF_SIZE, 0, (struct sockaddr *) &from_addr, &from_addr_size);
        if (str_len == -1) {
            error_handling("recvfrom() error");
            break;
        }

        // 转发
        sendto(server_sock, message, str_len, 0, (const struct sockaddr *) &from_addr, from_addr_size);

        printf("Received message from %s:%hu, and forward it.\n", inet_ntoa(from_addr.sin_addr), ntohs(from_addr.sin_port));
    }
    close(server_sock);

    return 0;
}