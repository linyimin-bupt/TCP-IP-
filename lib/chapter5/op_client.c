#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

#define BUF_SIZE 1024
#define RLT_SIZE 4
#define OPSZ 4

void error_handling(char *message) {
    fputs(message, stderr);
    exit(0);
}

int main(int argc, char *argv[]) {
    int client_sock;
    struct sockaddr_in server_addr;

    char opmsg[BUF_SIZE];   // 存储操作数
    int result, opnd_cnt;

    char message[BUF_SIZE];
    if (argc != 3) {
        printf("Usage: %s <ip> <port>\n", argv[0]);
        exit(1);
    }

    // init network address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(inet_addr(argv[1]));
    server_addr.sin_port = htons(atoi(argv[2]));

    client_sock = socket(PF_INET, SOCK_STREAM, 0);

    if (client_sock == -1) {
        error_handling("socket() error");
    }

    if (connect(client_sock, (const struct sockaddr *) &server_addr, sizeof(server_addr)) == -1) {
        error_handling("connect() error");
    }

    fputs("Operand count: ", stdout);
    scanf("%d", &opnd_cnt);
    opmsg[0] = (char)opnd_cnt;

    for (int i = 0; i < opnd_cnt; i++) {
        printf("Operand: %d", i+1);
        scanf("%d", (int *)&opmsg[OPSZ * i + 1]);
    }
    fgetc(stdin);
    fputs("Operator: ", stdout);
    scanf("%c", &opmsg[OPSZ * opnd_cnt + 1]);
    // send data to server
    write(client_sock, opmsg, opnd_cnt * OPSZ + 2);
    // received data from server
    read(client_sock, &result, RLT_SIZE);
    printf("Operation result: %d\n", result);
    close(client_sock);
    return 0;
}