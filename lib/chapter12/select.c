#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#define BUF_SIZE 1024
int main(int argc, char *argv[]) {
    fd_set reads, temps;

    struct timeval timeout;

    char buf[BUF_SIZE];
    int str_len;

    FD_ZERO(&reads);
    FD_SET(0, &reads);

    while(1) {
        temps = reads;
        timeout.tv_sec = 5;
        timeout.tv_usec = 0;


        int result = select(1, &temps, 0, 0, &timeout);

        if (result == -1) {
            puts("select() error");
            break;
        } else if (result == 0) {
            puts("Time-out!");
        } else {
            if (FD_ISSET(0, &temps)) {
                str_len = read(0, buf, BUF_SIZE);
                buf[str_len] = '\0';

                printf("Received message: %s\n", buf);

                sleep(10);
            }
        }
    }
    return 0;
}