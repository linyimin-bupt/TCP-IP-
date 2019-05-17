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
#include <unistd.h>

int main(int argc, char *argv[]) {
    int sock;

    FILE *readfp;
    FILE *writefp;

    if ((sock = socket(PF_INET, SOCK_STREAM, 0)) == -1) {
        fputs("socket() error", stdout);
        exit(1);
    }

    struct sockaddr_in server_addr, client_addr;

    // 初始化网络
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(argv[2]));
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);

    if (connect(sock, (const struct sockaddr *) &server_addr, sizeof(server_addr)) == -1) {
        fputs("connect() error", stderr);
        exit(1);
    }

    // 创建读模式的FILE文件指针
    readfp = fdopen(sock, "r");
    // 创建写模式的FILE文件指针
    writefp = fdopen(sock, "w");

    char buf[1024] = {0,};

    while(1) {
        if (fgets(buf, 1024, readfp) == NULL) {
            break;
        }
        fputs(buf, stdout);
        fflush(stdout);
    }

    // 接收到EOF后, 再次给服务器端发送信息

    fputs("From client: Thank you!\n", writefp);
    fflush(writefp);
    fclose(writefp);
    fclose(readfp);

    exit(0);
}