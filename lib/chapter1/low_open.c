#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
// 错误处理函数
void error_handling(char *message);

// 创建文件并写入数据
int main(void) {
  int fd;
  char buf[] = "let's go!\n";
  fd = open("data.txt", O_TRUNC | O_CREAT | O_RDWR);
  if (fd == -1) {
    error_handling("open() error!");
  }
  
  printf("file descriptor: %d \n", fd);
  if (write(fd, buf, sizeof(buf)) == -1) {
    error_handling("write() error!\n");
  }
  close(fd);
  exit(0);
}

void error_handling(char *message) {
  fputs(message, stderr);
  fputc('\n', stderr);
  exit(1);
}