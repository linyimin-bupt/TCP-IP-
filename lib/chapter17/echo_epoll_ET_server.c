#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <errno.h>
#include <fcntl.h>

#define BUF_SIZE 100
#define EPOLL_SIZE 50

void error_handler(char *message);
void set_noblocking_mode(int fd);

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <port>\n", argv[1]);
        exit(1);
    }

    int server_sock, client_sock;
    struct sockaddr_in server_addr, client_addr;

    socklen_t client_addr_size;

    if ((server_sock = socket(PF_INET, SOCK_STREAM, 0)) == -1) {
        error_handler("socket() error");
    }

    // 初始化网络
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(argv[1]));
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(server_sock, (const struct sockaddr *) &server_addr, sizeof(server_addr)) == -1) {
        error_handler("bind() error");
    }

    if (listen(server_sock, 5) == -1) {
        error_handler("listen() error");
    }

    // 使用epoll
    struct epoll_event *ep_events;
    struct epoll_event event;
    int epfd, event_cnt;

    if ((epfd = epoll_create(EPOLL_SIZE)) == -1) {
        error_handler("epoll_create() error");
    }

    // 监听连接请求
    event.events = EPOLLIN;
    event.data.fd = server_sock;

    // 将套接字设为非阻塞的形式
    set_noblocking_mode(client_addr_size);

    epoll_ctl(epfd, EPOLL_CTL_ADD, server_sock, &event);
    ep_events = (struct epoll_event *)malloc(sizeof(struct epoll_event) * EPOLL_SIZE);

    int str_len = 0;
    char buf[BUF_SIZE];
    while (1) {
        event_cnt = epoll_wait(epfd, ep_events, EPOLL_SIZE, -1);

        if (event_cnt == -1) {
            error_handler("epoll_wait() error");
            break;
        }

        // 如果不进行读取事件会一直堆积
        printf("return epoll_wait, and event_cnt = %d\n", event_cnt);

        for (int i = 0; i < event_cnt; i++) {
            printf("fd = %d\n", ep_events[i].data.fd);
            if (ep_events[i].data.fd == server_sock) {
                // 接收连接请求
                if((client_sock = accept(server_sock, (struct sockaddr *) &client_addr, &client_addr_size)) == -1) {
                    error_handler("accept() error");
                }

                // 监听客户端
                event.data.fd = client_sock;
                // 边缘触发
                event.events = EPOLLIN | EPOLLET;
                // 将套接字设为非阻塞的形式
                set_noblocking_mode(client_sock);
                epoll_ctl(epfd, EPOLL_CTL_ADD, client_sock, &event);

                printf("connected client: %d\n", client_sock);
            } else {
                while (1) {
                    str_len = read(ep_events[i].data.fd, buf, BUF_SIZE);
                    // 客户端断开连接
                    if (str_len == 0) {
                        // 从epoll中移除
                        epoll_ctl(epfd, EPOLL_CTL_DEL, ep_events[i].data.fd, NULL);
                        close(ep_events[i].data.fd);
                        printf("close client: %d\n", ep_events[i].data.fd);
                        break;
                    } else if (str_len < 0) {
                        if (errno == EAGAIN) {
                            break;
                        }
                    } else {
                        // echo!
                        write(ep_events[i].data.fd, buf, str_len);
                    }
                }
            }
        }
    }
    close(epfd);
    close(server_sock);

    return 0;
}

void error_handler(char *message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

void set_noblocking_mode(int fd) {
    int flag = fcntl(fd, F_GETFL, 0);
    fcntl(fd, F_SETFL, flag | O_NONBLOCK);
}