#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#define MAX_CLIENTS 256
#define BUF_SIZE 100

void *client_handler(void *argv);
void send_data(char *message, int len);
void error_handler(char *message);

pthread_mutex_t mutex;
int client_socks[256];
int client_count = 0;

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <port>\n", argv[0]);
        exit(1);
    }

    int server_sock, client_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_size = sizeof(client_addr);

    // 初始化互斥量
    pthread_mutex_init(&mutex, NULL);

    pthread_t p_id;

    if ((server_sock = socket(PF_INET, SOCK_STREAM, 0)) == -1) {
        error_handler("socket() error");
    }

    // 初始化网络
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(argv[1]));
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    // 为server socket分配地址和端口
    if (bind(server_sock, (const struct sockaddr *) &server_addr, sizeof(server_addr)) == -1) {
        error_handler("bind() error");
    }

    // 将socket转为被动接收连接请求状态
    if (listen(server_sock, 5) == -1) {
        error_handler("listen() error");
    }

    // 处理连接请求
    while (1) {
        client_sock = accept(server_sock, (struct sockaddr *) &client_addr, &client_addr_size);
        if (client_sock == -1) {
            error_handler("accept() error");
            break;
        }
        // 将客户端socket放入数组中统一管理(相当于群发消息)
        pthread_mutex_lock(&mutex);
        client_socks[client_count++] = client_sock;
        pthread_mutex_unlock(&mutex);

        // 创建线程处理连接请求
        pthread_create(&p_id, NULL, client_handler, (void *) &client_sock);
        pthread_detach(p_id);

        printf("Connected client IP: %s \n", inet_ntoa(client_addr.sin_addr));
    }
    close(server_sock);
    return 0;
}

void error_handler(char *message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

void *client_handler(void *argv) {
    int sock = *(int*)argv;
    int str_len = 0;
    char buf[BUF_SIZE];
    while ((str_len = read(sock, buf, BUF_SIZE)) != 0) {
        if(str_len == -1) {
            error_handler("read() error");
        } else if (str_len == 0) {
            // 客户端断开了连接
            pthread_mutex_lock(&mutex);
            for (int i = 0; i < client_count; i++) {
                if (sock == client_socks[i]) {
                    while(i < client_count) {
                        client_socks[i] = client_socks[i + 1];
                        i++;
                    }
                    client_count--;
                    break;
                }
            }
            close(sock);
            pthread_mutex_unlock(&mutex);
        } else {
            // 给所有客户端转发, 包括自己
            printf("message: %s\n", buf);
            send_data(buf, str_len);
        }
    }
    return NULL;
}

void send_data(char *message, int len) {
    pthread_mutex_lock(&mutex);
    for (int i = 0; i < client_count; i++) {
        write(client_socks[i], message, len);
    }
    pthread_mutex_unlock(&mutex);
}