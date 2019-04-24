// 同时创建文件和socket
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include <sys/stat.h>
#include<sys/socket.h>

void error_handling(char* message);


int main(void) {
  // 文件描述符
  int fd1, fd2, fd3;
  fd1 = socket(PF_INET, SOCK_STREAM, 0);
  fd2 = open("test.txt", O_RDWR | O_CREAT, 0755);
  fd3 = socket(PF_INET, SOCK_STREAM, 0);  
  printf("file descriptor 1: %d\n", fd1);
  printf("file descriptor 2: %d\n", fd2);
  printf("file descriptor 3: %d\n", fd3);
  
  close(fd1);  
  close(fd2); 
  close(fd3);
  return 0;
  
}

void error_handling(char *message) {
  fputs(message, stderr);
  fputc('\n', stderr);
  exit(1);
}