#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>

void read_childproc(int sig) {
    int status;     // 保存中止的子进程相关信息
    /*
     * -1表示可以等待任意子进程中止
     * WNOHANG表示即使没有终止的子进程waitpid也不会进入阻塞状态,而是直接返回0,不再等待子进程终止
     */
    pid_t pid = waitpid(-1, &status, WNOHANG);
    if (WIFEXITED(status)) {
        printf("Removed proc id: %d \n", pid);
        printf("Child send: %d \n", WEXITSTATUS(status));
    }
}
int main(void) {
    struct sigaction act;
    // 指定信号注册函数, 当有相关信号发生时,执行该函数
    act.sa_handler = read_childproc;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;

    // 注册SIGCHLD事件

    /*
     * sigaction比signal的兼容性要好
     */
    sigaction(SIGCHLD, &act, 0);

    pid_t pid = fork();

    if (pid == 0) {
        puts("Hi! I'm child process");
        sleep(10);
        return 12;
    } else {
        printf("Child proc id: %d\n", pid);
        pid = fork();

        if (pid == 0) {
            puts("Hi! I'm child process.");
            sleep(10);
            // 触发SIGCHLD事件
            exit(24);
        } else {
            printf("Child proc id: %d \n", pid);
            for (int i = 0; i < 5; i++) {
                puts("Wait ...");
                sleep(5);
            }
        }
    }
    return 0;
}