#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void *thread1(void *p)
{
    for (int i = 1; i <= 10; i++)
    {
        printf("Thread ID %ld: i=%d\n", pthread_self(), i);
        usleep(1000);
    }
    return NULL;
}

void *thread2(void *p)
{
    for (int i = 1; i <= 10; i++)
    {
        printf("Thread ID %ld: i=%d\n", pthread_self(), i);
        usleep(1000);
    }

    return NULL;
}

int main()
{
    pthread_t thrID1, thrID2;
    pthread_create(&thrID1, NULL, thread1, NULL);
    pthread_create(&thrID2, NULL, thread2, NULL);
    pthread_join(thrID1, NULL);
    pthread_join(thrID2, NULL);
}