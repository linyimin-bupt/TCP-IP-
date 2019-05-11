#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>

int main(int argc, char *argv[]) {
    fd_set reads, temps;

    struct timeval timeout;

    FD_ZERO(&reads);
    FD_SET(0, &reads);

    while(1) {
        temps = reads;
        timeout.tv_sec = 5;
        timeout.tv_usec = 0;
    }
    return 0;
}