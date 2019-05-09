/*
 * 这个程序很好理解
 * 但是需要注意的是:
 * 产生信号时,为了调用信号处理器,,将唤醒由于调用sleep函数进入阻塞状态的进程,而且一旦进程被唤醒,
 * 便不再进入睡眠状态,所以这个程序不会等到300s才执行完,相反在10s内就能执行完毕.
 */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void timeout(int sig) {
    if (sig == SIGALRM) {
        puts("Time out");
    }

    alarm(2);
}

void keycontrol(int sig) {
    if (sig == SIGINT) {
        puts("CTRL+C pressed");
    }
}

int main(void) {
    /*
     * 注册相关信号及对应信号处理函数
     */
    signal(SIGALRM, timeout);
    signal(SIGINT, keycontrol);
    // 两秒后发布SIGALRM信号
    alarm(2);

    for (int i = 0; i < 3; i++) {
        puts("wait ...");
        sleep(100);
    }
    return 0;
}