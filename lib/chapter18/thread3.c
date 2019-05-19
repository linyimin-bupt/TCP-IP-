/*
 * 工作线程模式, 创建的线程执行指定任务
 * main进程进行管理
 */
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

int sum = 0;

void *thread_summation(void *argv);

int main(void) {

    int range1[] = {1, 5};
    int range2[] = {6, 10};

    pthread_t tid1, tid2;

    if (pthread_create(&tid1, NULL, thread_summation, (void*)range1) != 0) {
        fputs("thread_create() error", stderr);
        exit(1);
    }
    if (pthread_create(&tid2, NULL, thread_summation, (void*)range2) != 0) {
        fputs("thread_create() error", stderr);
        exit(1);
    }
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);


    printf("result = %d\n", sum);
    return 0;
}

void *thread_summation(void *argv) {
    int start = ((int *)argv)[0];
    int end = ((int *)argv)[1];
    sleep(2);
    while (start <= end) {
        sum += start;
        start++;
    }

    return NULL;
}