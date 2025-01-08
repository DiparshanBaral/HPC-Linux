#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>

#define NUM_THREADS 3
#define ITERATIONS 10

char *messages[NUM_THREADS] = {NULL, NULL, NULL};
sem_t semaphores[NUM_THREADS]; // One semaphore per thread

void *messenger(void *p)
{
    long tid = (long)p;
    char tmpbuf[100];

    for (int i = 0; i < ITERATIONS; i++)
    {
        // Wait for this thread's turn
        sem_wait(&semaphores[tid]);

        /* Sending a message */
        long dest = (tid + 1) % NUM_THREADS;
        sprintf(tmpbuf, "Hello from Thread %ld!", tid);
        messages[dest] = strdup(tmpbuf);
        printf("Thread %ld sends Thread %ld a message\n", tid, dest);

        /* Receiving a message */
        if (messages[tid] != NULL)
        {
            printf("Thread %ld receives the message '%s'\n", tid, messages[tid]);
            free(messages[tid]);
            messages[tid] = NULL;
        }

        // Signal the next thread
        sem_post(&semaphores[dest]);
    }

    return NULL;
}

int main()
{
    pthread_t threads[NUM_THREADS];

    // Initialize semaphores
    for (int i = 0; i < NUM_THREADS; i++)
    {
        sem_init(&semaphores[i], 0, 0); // Initialize all semaphores to 0
    }

    // Allow Thread 0 to start
    sem_post(&semaphores[0]);

    // Create threads
    for (long i = 0; i < NUM_THREADS; i++)
    {
        pthread_create(&threads[i], NULL, messenger, (void *)i);
    }

    // Join threads
    for (int i = 0; i < NUM_THREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }

    // Destroy semaphores
    for (int i = 0; i < NUM_THREADS; i++)
    {
        sem_destroy(&semaphores[i]);
    }

    return 0;
}
