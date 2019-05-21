#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#define REQUEST_LINE 64

void error_handler(char *message);

void *request_handler(void *argv);
void send_data(int sock, char *file_name, char *extension);
void send_error(int sock);
char *content_type(char *file_name);


int main(int argc, char *argv[]) {

    if (argc != 2) {
        printf("Usage: %s <port>", argv[0]);
        exit(1);
    }

    int server_sock, client_sock;

    if ((server_sock = socket(PF_INET, SOCK_STREAM, 0)) == -1) {
        error_handler("socket() error");
    }

    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_size;

    // 初始化网络
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(argv[1]));
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(server_sock, (const struct sockaddr *) &server_addr, sizeof(server_addr)) == -1) {
        error_handler("bind() error");
    }

    if (listen(server_sock, 200) == -1) {
        error_handler("listen() error");
    }

    while (1) {
        client_addr_size = sizeof(client_addr);
        if ((client_sock = accept(server_sock, (struct sockaddr *) &client_addr, &client_addr_size)) == -1) {
            error_handler("accept() error");
            exit(1);
        }

        pthread_t p_id;
        pthread_create(&p_id, NULL, request_handler, &client_sock);
        pthread_detach(p_id);
    }
    close(server_sock);
    return 0;
}

void error_handler(char *message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

void *request_handler(void *argv) {
    int sock = *((int *) argv);

    char request_line[REQUEST_LINE];    // 请求行
    char method[10];                    // 请求方法
    char file_name[20];                 // 请求文件名
    char file_extend[8];                // 请求文件扩展名

    int str_len = read(sock, request_line, REQUEST_LINE);
    if (str_len == -1) {
        error_handler("read() error");
    } else {
        if (strstr(request_line, "HTTP/") == NULL) {
            printf("no http request\n");
            printf("request_line: %s\n", request_line);
            send_error(sock);
            close(sock);
            return NULL;
        }
        strcpy(method, strtok(request_line, " /"));
        strcpy(file_name, strtok(NULL, " /"));
        strcpy(file_extend, content_type(file_name));

        if (strcmp(method, "GET") != 0 || strcmp(file_name, "favicon.ico") == 0) {
            send_error(sock);
        } else {
            send_data(sock, file_name, file_extend);
        }
    }
    close(sock);
    return NULL;

}

void send_error(int sock) {
    struct iovec vec[4];
    char protocol[] = "HTTP/1.1 400 Bad Request\r\n";
    char server[] = "Server:Linux Web Server\r\n";
    char cnt_type[] = "Content-type:text/html\r\n\r\n";
    char content[] = "<html><head><title>NETWORK</title></head>"
                     "<body><font size=+5><br>发生错误,请查看请求文件名和请求方式!</body></html>";

    vec[0].iov_len = strlen(protocol);
    vec[0].iov_base = protocol;
    vec[1].iov_len = strlen(server);
    vec[1].iov_base =  server;
    vec[2].iov_len = strlen(cnt_type);
    vec[2].iov_base = cnt_type;
    vec[3].iov_len = strlen(content);
    vec[3].iov_base = content;
    writev(sock, vec, 4);
    shutdown(sock, SHUT_WR);
}

void send_data(int sock, char *file_name, char *extension) {
    char protocol[] = "HTTP/1.1 200 OK\r\n";
    char server[] = "Server:Linux Web Server\r\n";
    char cnt_type[24];

    sprintf(cnt_type, "Content-type:%s\r\n\r\n", extension);

    write(sock, protocol, strlen(protocol));
    write(sock, server, strlen(server));
    write(sock, cnt_type, strlen(cnt_type));

    printf("file_name %s\n", file_name);

    int fd = open(file_name, O_RDONLY);

    if (fd == -1) {
        printf("errno: %d\n", errno);
        error_handler("open() error");
        send_error(sock);
    }

    int str_len = 0;
    char buf[1024];
    while ((str_len = read(fd, buf, 1024)) != 0) {
        buf[str_len] = '\0';
        printf("%s\n", buf);
        write(sock, buf, str_len);
    }
    close(fd);
    shutdown(sock, SHUT_WR);
}

char *content_type(char *file_name) {
    char file[20];
    char extend[8];
    strcpy(file, file_name);
    strtok(file, ".");
    strcpy(extend, strtok(NULL, "."));
    if (!strcmp(extend, "html") || !strcmp(extend, "htm")) {
        return "text/html";
    } else {
        return "text/plain";
    }
}