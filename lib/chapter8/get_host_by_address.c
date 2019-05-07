
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <arpa/inet.h>

void error_handling(char *message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

int main(int argc, char *argv[]) {

    if (argc != 2) {
        printf("Usage: %s <address>", argv[0]);
        exit(1);
    }

    struct hostent *host = gethostbyaddr(argv[1], 4, AF_INET);

    if (host == NULL) {
        error_handling("gethostbyname() error");
    }

    printf("name: %s\n", host->h_name);
    printf("alias: \n");
    for (int i = 0; host->h_aliases[i]; i++) {
        printf("%s ", host->h_aliases[i]);
    }

    printf("\n");

    printf("address type: %s\n", host->h_addrtype == AF_INET ? "AF_INET" : "Other");
    printf("adress length; %d\n", host->h_length);

    printf("address list:\n");
    for (int i = 0; host->h_addr_list[i]; i++) {
        printf("%s ", inet_ntoa(*(struct in_addr*)host->h_addr_list[i]));
    }

    printf("\n");
    return 0;
}