#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
void error_handling(char *message);

int main(void) {
    int sock;

    int send_buf;
    int receive_buf;

    int state;

    socklen_t len;

    sock = socket(PF_INET, SOCK_STREAM, 0);

    if (sock == -1) {
        error_handling("socket() error");
    }

    len = sizeof(receive_buf);

    state = getsockopt(sock, SOL_SOCKET, SO_RCVBUF, &send_buf, &len);

    if (state) {
        error_handling("getsockopt() error");
    }

    printf("SO_RCV_BUF: %d\n", receive_buf);

    state = getsockopt(sock, SOL_SOCKET, SO_SNDBUF, &send_buf, &len);

    if (state) {
        error_handling("getsockopt() error");
    }

    printf("SO_SND_BUF: %d\n", send_buf);

    send_buf = 1024 * 1024;
    receive_buf = 4 * 1024;

    state = setsockopt(sock, SOL_SOCKET, SO_RCVBUF, &receive_buf, sizeof(receive_buf));

    if (state) {
        error_handling("setsockopt() error");
    }

    state = setsockopt(sock, SOL_SOCKET, SO_SNDBUF, &send_buf, sizeof(send_buf));

    if (state) {
        error_handling("setsockopt() error");
    }

    state = getsockopt(sock, SOL_SOCKET, SO_RCVBUF, &send_buf, &len);

    if (state) {
        error_handling("getsockopt() error");
    }

    printf("SO_RCV_BUF: %d\n", receive_buf);

    state = getsockopt(sock, SOL_SOCKET, SO_SNDBUF, &send_buf, &len);

    if (state) {
        error_handling("getsockopt() error");
    }

    printf("SO_SND_BUF: %d\n", send_buf);

    return 0;
}

void error_handling(char *message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}