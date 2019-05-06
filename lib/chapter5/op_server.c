
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

#define OPSZ 4
#define BUF_SIZE 1024

void error_handling(char *message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

int calculate(int opnum, int opinfo[], char op) {
    int result = opinfo[0];
    switch (op) {
        case '+': {
            for (int i = 1; i < opnum; i++) {
                result += opinfo[i];
            }
            break;
        }
        case '-': {
            for (int i = 1; i < opnum; i++) {
                result -= opinfo[i];
            }
            break;
        }
        case '*': {
            for (int i = 1; i < opnum; i++) {
                result *= opinfo[i];
            }
            break;
        }
        case '/': {
            for (int i = 1; i < opnum; i++) {
                result /= opinfo[i];
            }
            break;
        }
    }

    return result;
}

int main(int argc, char *argv[]) {
    int server_sock;
    int client_sock;

    char opinfo[BUF_SIZE];

    struct sockaddr_in server_addr;

    if (argc != 2) {
        printf("Usage: %s <port>", argv[0]);
        exit(1);
    }

    server_sock = socket(PF_INET, SOCK_STREAM, 0);
    if (server_sock == -1) {
        error_handling("socket() error");
    }

    // init network address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(argv[1]));
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(server_sock, (const struct sockaddr *) &server_addr, sizeof(server_addr)) == -1) {
        error_handling("bind() error");
    }

    if (listen(server_sock, 5) == -1) {
        error_handling("listen() error");
    }

    struct sockaddr_in client_addr;
    socklen_t client_addr_size = sizeof(client_addr);

    for (int i = 0; i , 5; i++) {
        client_sock = accept(server_sock, (struct sockaddr *) &client_addr, &client_addr_size);
        if (client_sock == -1) {
            error_handling("accept() error");
        } else {
            printf("Connected ...\n");
            printf("client info: \n");
            printf("server: %s, port: %u", inet_ntoa(client_addr.sin_addr), client_addr.sin_port);
        }

        int opnd_cnt = 0;
        read(client_sock, &opnd_cnt, 1);

        int rev_len = 0;
        while ((opnd_cnt * OPSZ + 1) > rev_len) {
            int rev_cnt = read(client_sock, &opinfo[rev_len], BUF_SIZE - 1);
            rev_len += rev_cnt;
        }
        int result = calculate(opnd_cnt, (int *)opinfo, opinfo[rev_len - 1]);
        write(client_sock, &result, sizeof(result));
        close(client_sock);
    }
    close(server_sock);
    return 0;
}