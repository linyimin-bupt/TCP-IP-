#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#define BUF_SIZE 30
int main(void) {
    int fds[2];
    char str[] = "Who are you?";
    char buf[BUF_SIZE];

    pipe(fds);

    pid_t pid = fork();

    if (pid == 0) {
        read(fds[0], buf, BUF_SIZE);
        write(fds[1], str, strlen(str));
    } else {
        read(fds[0], buf, BUF_SIZE);
        printf("Received message: %s\n", buf);
    }

    return 0;
}