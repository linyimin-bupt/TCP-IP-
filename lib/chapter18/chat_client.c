#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>

#define BUF_SIZE 100
#define NAME_SIZE 20

void error_handler(char *message);

void *send_data(void *argv);

void *recv_data(void *argv);

char name[NAME_SIZE] = "[DEFAULT]";
char msg[BUF_SIZE];

int main(int argc, char *argv[]) {

    if (argc != 4) {
        printf("Usage: %s <ip> <port> <name>\n", argv[0]);
        exit(1);
    }

    int sock;

    if ((sock = socket(PF_INET, SOCK_STREAM, 0)) == -1) {
        error_handler("socket() error");
    }

    sprintf(name, "[%s]", argv[3]);
    // 初始化网络
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);
    server_addr.sin_port = htons(atoi(argv[2]));

    if (connect(sock, (const struct sockaddr *) &server_addr, sizeof(server_addr)) == -1) {
        error_handler("connect() error");
    }

    // 创建两个线程分别用于读操作和写操作
    pthread_t pread_id, pwrite_id;
    pthread_create(&pread_id, NULL, recv_data, (void *) &sock);
    pthread_create(&pwrite_id, NULL, send_data, (void *) &sock);

    // 这里使用join不用detach的原因是,join是阻塞的,避免main主线程退出后, 其他线程也结束了
    pthread_join(pread_id, NULL);
    pthread_join(pwrite_id, NULL);

    return 0;
}

void error_handler(char *message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

void *send_data(void *argv) {
    int sock = *((int *) argv);
    char name_msg[NAME_SIZE + BUF_SIZE];
    while (1) {
        fgets(msg, BUF_SIZE, stdin);
        if (!strcmp(msg, "q\n") || !strcmp(msg, "Q\n")) {
            close(sock);
            exit(0);
        }
        sprintf(name_msg, "%s %s", name, msg);
        write(sock, name_msg, strlen(name_msg));
    }
    return NULL;
}

void *recv_data(void *argv) {
    int sock = * ((int*)argv);
    char name_msg[NAME_SIZE + BUF_SIZE];

    int str_len = 0;

    while (1) {
        if ((str_len = read(sock, name_msg, NAME_SIZE + BUF_SIZE - 1)) == -1) {
            return (void *)-1;
        }
        name_msg[str_len] = '\0';
        fputs(name_msg, stdout);
    }
    return NULL;
}