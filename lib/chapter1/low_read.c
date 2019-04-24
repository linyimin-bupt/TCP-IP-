#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>

#define BUF_SIZE 100

void error_handling(char* message);


int main(void) {
  // 文件描述符
  int fd;
  char buf[BUF_SIZE];

  fd = open("data.txt", O_RDONLY);
  if (fd == -1) {
    error_handling("open() error!");
  }
  // 读取文件数据，并保存在数组buf中
  if (read(fd, buf, sizeof(buf)) == -1) {
    error_handling("read() error!");
  }
  printf("file data: %s\n", buf);
  close(fd);
  return 0;
  
}

void error_handling(char *message) {
  fputs(message, stderr);
  fputc('\n', stderr);
  exit(1);
}