#include <stdio.h>
#include <pthread.h>

int counter = 0;
int flag = 1;

int doSomething(int threadID, int count) {
    printf("Thread %d: Counter=%d\n", threadID, counter);
    return count + 1;
}

void *threadOne(void *p) {
    for (int i = 0; i < 100; i++) {
        while (flag != 1);
        counter = doSomething(1, counter);
        flag = 2;
    }
    return NULL;
}

void *threadTwo(void *p) {
    for (int i = 0; i < 100; i++) {
        while (flag != 2);
        counter = doSomething(2, counter);
        flag = 1;
    }
    return NULL;
}

void main() {
    pthread_t thrID1, thrID2;

    pthread_create(&thrID1, NULL, threadOne, NULL);
    pthread_create(&thrID2, NULL, threadTwo, NULL);

    pthread_join(thrID1, NULL);
    pthread_join(thrID2, NULL);

    printf("Final counter = %d\n", counter);
}
