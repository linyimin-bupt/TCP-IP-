#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

// 共享变量
int num;

/*
 * 定义信号量
 */

// 判断是否可读
static sem_t sem_one;
// 判断是否可写
static sem_t sem_two;

void *read_data(void *argv);
void *calculate(void *argv);
int main(void) {

    pthread_t t_read_id, t_calculate_id;

    sem_init(&sem_one, 0, 0);
    sem_init(&sem_two, 0, 1);

    pthread_create(&t_read_id, NULL, read_data, NULL);
    pthread_create(&t_calculate_id, NULL, calculate, NULL);

    pthread_join(t_read_id, NULL);
    pthread_join(t_calculate_id, NULL);

    sem_destroy(&sem_one);
    sem_destroy(&sem_two);
    return 0;
}

void *read_data(void *argv) {

    for (int i = 0; i < 5; i++) {
        fputs("Input num:", stdout);
        sem_wait(&sem_two);
        scanf("%d", &num);
        // num已写完,可读
        sem_post(&sem_one);
    }
    return NULL;
}

void *calculate(void *argv) {
    int sum = 0;
    for (int i = 0; i < 5; i++) {
        // 是否可读
        sem_wait(&sem_one);
        sum += num;
        // 已读完,num可写
        sem_post(&sem_two);
    }
    printf("result: %d\n", sum);
    return NULL;
}