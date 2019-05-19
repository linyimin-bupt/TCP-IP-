/*
 * 使用thread_join等待线程执行完毕
 */

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

void *thread_main(void *argv);

int main(void) {

    pthread_t p_id;
    int thread_param = 5;
    void *thread_ret;
    if (pthread_create(&p_id, NULL, thread_main, &thread_param) != 0) {
        puts("pthread_create() error");
        return -1;
    }

    if (pthread_join(p_id, &thread_ret) != 0) {
        puts("pthread_join() error");
        exit(-1);
    }

    printf("Thread return message: %s\n", (char *) thread_ret);
    free(thread_ret);
    return 0;
}

void* thread_main(void *argv) {
    int count = *(int *)argv;
    char *msg = (char *) malloc(sizeof(char) * 50);
    strcpy(msg, "Hello, I am thread~ \n");
    for (int i = 0; i < count; i++) {
        sleep(1);
        puts("running thread");
    }
    return (void*)msg;
}