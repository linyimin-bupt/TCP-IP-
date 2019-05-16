#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUF_SIZE 1024

void error_handling(char *message);

int main(int argc, char *argv[]) {
    int sock = 0;

    int idx = 0;
    char message[BUF_SIZE];
    int str_len = 0;
    struct sockaddr_in server_addr;

    if (argc != 3) {
        printf("Usage: %s <ip> <port>", argv[0]);
    }

    // init network address

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);
    server_addr.sin_port = htons(atoi(argv[2]));
    server_addr.sin_family = AF_INET;

    // create socket

    sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        error_handling("socket() error");
    }

    socklen_t server_addr_size = sizeof(server_addr);
    if (connect(sock, (const struct sockaddr *) &server_addr, server_addr_size) == -1) {
        error_handling("connect() error");
    } else {
        printf("Connected ...\n");
    }

    FILE *read_fp, *write_fp;

    read_fp = fdopen(sock, "r");
    write_fp = fdopen(sock, "w");

    while (1) {
        fputs("Input message(Q to quit): ", stdout);
        fgets(message, BUF_SIZE, stdin);

        if (!strcmp(message, "q\n") || !strcmp(message, "Q\n")) {
            break;
        }

        fputs(message, write_fp);
        fflush(write_fp);
        fgets(message, BUF_SIZE, read_fp);
        printf("Message from server: %s", message);
    }
    return 0;
}

void error_handling(char *message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}