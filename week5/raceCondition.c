#include <stdio.h>
#include <pthread.h>
int counter = 0;
int doSomething(int threadID, int count)
{
    printf("Thread %d: Counter=%d\n", threadID, counter);
    return count + 1;
}
void *threadOne(void *p)
{

    for (int i = 0; i < 100; i++)
    {
        counter = doSomething(1, counter);
    }
}
void *threadTwo(void *p)
{

    for (int i = 0; i < 100; i++)
    {
        counter = doSomething(2, counter);
    }
}
void main()
{
    pthread_t thrID1, thrID2;

    pthread_create(&thrID1, NULL, threadOne, NULL);
    pthread_create(&thrID2, NULL, threadTwo, NULL);
    pthread_join(thrID1, NULL);
    pthread_join(thrID2, NULL);

    printf("Final Counter = %d\n", counter);
}