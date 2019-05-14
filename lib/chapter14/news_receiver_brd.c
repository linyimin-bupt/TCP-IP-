#include <stdlib.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/socket.h>
9
#define BUF_SIZE 1024

void error_handling(char *message);

int main(int argc, char *argv[]) {
    int sock;

    struct sockaddr_in addr;
    struct ip_mreq join_addr;

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(atoi(argv[1]));

    if ((sock = socket(PF_INET, SOCK_DGRAM, 0)) == -1) {
        error_handling("socket() error");
    }

    if (bind(sock, (const struct sockaddr *) &addr, sizeof(addr)) == -1) {
        error_handling("bind() error");
    }

    // 接收广播信息
    int str_len = 0;
    char buf[BUF_SIZE];
    while(1) {
        str_len = recvfrom(sock, buf, BUF_SIZE - 1, 0, NULL, 0);
        if (str_len < 0) {
            break;
        }
        buf[str_len] = '\0';
        fputs(buf, stdout);
    }

    return 0;
}

void error_handling(char *message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}