/*
 * 对于使用fdopen创建的多个FILE指针,只要调用一次fclose函数就会完全终止套接字, 不存在半关闭.
 *
 * 这种情况属于多个FILE指针对应一个文件描述符, 针对任意一个FILE指针调用fclose函数都会关闭文件描述符, 也就终止了套接字
 */

#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>

int main(int argc, char *argv[]) {
    int server_sock, client_sock;

    FILE *readfp;
    FILE *writefp;

    if ((server_sock = socket(PF_INET, SOCK_STREAM, 0)) == -1) {
        fputs("socket() error", stdout);
        exit(1);
    }

    struct sockaddr_in server_addr, client_addr;

    // 初始化网络
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(argv[1]));
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(server_sock, (const struct sockaddr *) &server_addr, sizeof(server_addr)) == -1) {
        fputs("bind() error", stderr);
        exit(1);
    }

    if (listen(server_sock, 5) == -1) {
        fputs("listen() error", stderr);
        exit(1);
    }
    socklen_t client_addr_size = sizeof(server_addr);
    if ((client_sock = accept(server_sock, (struct sockaddr *) &client_addr, &client_addr_size)) == -1) {
        fputs("accept() error", stderr);
        exit(1);
    }

    // 创建读模式的FILE文件指针
    readfp = fdopen(client_sock, "r");
    // 创建写模式的FILE文件指针
    writefp = fdopen(client_sock, "w");

    fputs("Hi~ client?\n", writefp);
    fputs("I love programming!\n", writefp);
    fputs("You are awesome!\n", writefp);
    fflush(writefp);

    fclose(writefp);

    // 如果存在半关闭的话, 应该还能接收来自客户端的消息
    char buf[1024] = {0,};
    fgets(buf, 1024, readfp);
    fputs(buf, stdout);
    fclose(readfp);

    exit(0);
}