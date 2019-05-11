#include <stdlib.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <sys/time.h>
#include <unistd.h>

#define BUF_SIZE 1024

void error_handling(char *message);

int main(int argc, char *argv[]) {

    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;

    int server_sock;
    int client_sock;

    char message[BUF_SIZE];

    if (argc != 2) {
        printf("Usage: %s <port>\n", argv[0]);
        exit(1);
    }

    server_sock = socket(PF_INET, SOCK_STREAM, 0);

    if (server_sock == -1) {
        error_handling("socket() error!");
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(argv[1]));
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(server_sock, (const struct sockaddr *) &server_addr, sizeof(server_addr)) == -1) {
        error_handling("bind() error");
    }

    if (listen(server_sock, 5) == -1) {
        error_handling("listen() error");
    }

    // 使用多路复用处理来自客户端的请求
//    fd_set reads, temps;
//    struct timeval timeout;
//
//    FD_ZERO(&reads);
//    FD_SET(server_sock, &reads);
//    int fd_max = server_sock;

    sleep(1000);


//    while (1) {
//        temps = reads;
//        timeout.tv_usec = 200;
//        timeout.tv_sec = 200;
//        int result = select(fd_max + 1, &temps, 0, 0, &timeout);
//
//        if (result == -1) {
//            error_handling("select() error");
//            break;
//        }
//
//        if (result == 0) {
//            puts("Time-out");
//            continue;
//        } else {
//            for (int i = 0; i < fd_max + 1; i++) {
//                if (FD_ISSET(i, &temps)) {
//                    // connection request
//
//                    if (i == server_sock) {
//                        socklen_t client_addr_size = sizeof(client_addr);
//                        client_sock = accept(server_sock, (struct sockaddr *) &client_addr, &client_addr_size);
//                        FD_SET(client_sock, &reads);
//                        if (client_sock > fd_max) {
//                            fd_max = client_sock;
//                        }
//
//                        printf("Connected client: %d \n", client_sock);
//                    } else {
//                        int str_len = read(i, message, BUF_SIZE);
//
//                        if (str_len == 0) {
//                            FD_CLR(i, &reads);
//                            close(i);
//                            printf("closed client: %d \n", i);
//                        } else {
//                            // echo!
//                            write(i, message, str_len);
//                        }
//                    }
//                }
//            }
//        }
//    }
    close(server_sock);
    return 0;
}

void error_handling(char *message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}