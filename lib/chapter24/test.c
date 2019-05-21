#include<stdio.h>
#include<stdlib.h>
#include<string.h>
int main(void) {
    char message[] = "GET /index.html HTTP/1.1";
    char method[100];
    char file_name[200];
    strcpy(method, strtok(message, " /"));
    printf("method: %s\n", method);
    strcpy(file_name, strtok(NULL, " /"));
    printf("file_name: %s\n", file_name);

    printf("%s\n", strcpy(file_name, strtok(NULL, " ")));
    return 0;
}