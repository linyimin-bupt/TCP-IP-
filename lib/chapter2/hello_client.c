#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void error_handling(char *message) {
    fputs (message, stderr);
    fputc('\n', stderr);
    exit(1);
}

int main(int argc, char *argv[]) {
    int sock;

    struct sockaddr_in serv_addr;

    char message[300000];

    int str_len = 0;            // 读取数据的长度

    int idx = 0, read_len = 0;

    if (argc != 3) {
        printf("Usage: %s <ip> <port>", argv[0]);
        exit(1);
    }

    sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        error_handling("sock() error");
    }

    memset(&serv_addr, 0, sizeof(serv_addr));

    serv_addr.sin_port = htons(atoi(argv[2]));
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_family = AF_INET;

    // 发出连接请求
    if (connect(sock, (const struct sockaddr *) &serv_addr, sizeof(serv_addr)) == -1) {
        error_handling("connect() error");
    }
    for (int i = 0; i < 30000; i++) {

    }
    while (read_len = read(sock, &message, 5000)) {
        if (read_len == -1) {
            error_handling("read() error");
        }
        str_len += read_len;
        printf("id: %d\n", idx);
    }

    printf("Message from server: %s\n", message);
    printf("Function read call count: %d \n",  str_len);
    close(sock);
    return 0;
}