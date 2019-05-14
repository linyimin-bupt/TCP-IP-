#include <stdlib.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>

#define TTL 64
#define BUF_SIZE 1024

void error_handler(char *message);

int main(int argc, char *argv[]) {

    int sock;
    struct sockaddr_in mul_addr;

    if ((sock = socket(PF_INET, SOCK_DGRAM, 0)) == -1) {
        error_handler("socket() error");
    }


    memset(&mul_addr, 0, sizeof(mul_addr));
    mul_addr.sin_family = AF_INET;
    mul_addr.sin_port = htons(atoi(argv[2]));           // 多播端口，不是本进程端口， 本进程端口由sendto函数自动分配
    mul_addr.sin_addr.s_addr = inet_addr(argv[1]);      // 必须是多播地址

    // 设置存活时间(路由器的跳数)
    int time_live = TTL;
    setsockopt(sock, IPPROTO_IP, IP_MULTICAST_TTL, (void*)&time_live, sizeof(time_live));

    FILE *fp;
    if ((fp = fopen("news.txt", "r")) == NULL) {
        error_handler("fopen() error");
    }

    char buf[BUF_SIZE];
    while (1) {
        printf("test");
        fgets(buf, BUF_SIZE, fp);
        if (strlen(buf) == 0) {
            break;
        }
        printf("1\n");
        sendto(sock, buf, strlen(buf), 0, (const struct sockaddr *) &mul_addr, sizeof(mul_addr));
        printf("2\n");
        printf("%lu\n", strlen(buf));
        sleep(2);
    }

    fclose(fp);
    close(sock);
    return 0;
}

void error_handler(char *message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}