#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_THREADS 3

char *messages[NUM_THREADS] = {NULL, NULL, NULL};
int ready_to_receive[NUM_THREADS] = {0, 0, 0}; // Flags for message readiness

void *messenger(void *p)
{
    long tid = (long)p;
    char tmpbuf[100];

    for (int i = 0; i < 10; i++)
    {
        /* Sending a message */
        long dest = (tid + 1) % NUM_THREADS;
        sprintf(tmpbuf, "Hello from Thread %ld!", tid);
        char *msg = strdup(tmpbuf);

        // Busy-wait until the receiver is ready
        while (ready_to_receive[dest] == 1)
            ;

        messages[dest] = msg;
        ready_to_receive[dest] = 1; // Notify the receiver
        printf("Thread %ld sent the message to Thread %ld\n", tid, dest);

        /* Receiving a message */
        // Busy-wait until a message is available
        while (ready_to_receive[tid] == 0)
            ;

        printf("Thread %ld received the message '%s'\n", tid, messages[tid]);

        free(messages[tid]);
        messages[tid] = NULL;
        ready_to_receive[tid] = 0; // Mark as processed
    }
    return NULL;
}

int main()
{
    pthread_t thrID[NUM_THREADS];
    for (long i = 0; i < NUM_THREADS; i++)
    {
        pthread_create(&thrID[i], NULL, messenger, (void *)i);
    }
    for (int i = 0; i < NUM_THREADS; i++)
    {
        pthread_join(thrID[i], NULL);
    }
    return 0;
}
