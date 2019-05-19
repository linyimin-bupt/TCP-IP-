#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

/*
 * 多线程同时访问同一内存空间会存在问题
 */
#define NUM_THREAD 100

void * inc(void *argv);
void * des(void *argv);

int result = 0;

int main(void) {
    pthread_t t_ids[NUM_THREAD];

    // 创建100个线程
    for (int i = 0; i < NUM_THREAD; i++) {
        if (i % 2 == 0) {
            pthread_create(&t_ids[i], NULL, inc, NULL);
        } else {
            pthread_create(&t_ids[i], NULL, des, NULL);
        }
    }

    for (int i = 0; i < NUM_THREAD; i++) {
        pthread_join(t_ids[i], NULL);
    }

    printf("result: %d\n", result);
    exit(0);
}

void *inc(void *argv) {
    for (int i = 0; i < 50000000; i++) {
        result += i;
    }
    return NULL;
}

void *des(void *argv) {
    for (int i = 0; i < 50000000; i++) {
        result -= i;
    }
    return NULL;
}