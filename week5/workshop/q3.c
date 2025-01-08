#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define NUM_THREADS 3
#define ITERATIONS 10

char *messages[NUM_THREADS] = {NULL, NULL, NULL};
pthread_mutex_t control_mutex; // Mutex to control thread synchronization
pthread_cond_t conditions[NUM_THREADS]; // One condition variable per thread
int current_thread = 0; // Tracks which thread should execute

void *messenger(void *p)
{
    long tid = (long)p;
    char tmpbuf[100];

    for (int i = 0; i < ITERATIONS; i++)
    {
        pthread_mutex_lock(&control_mutex);

        // Wait until it's this thread's turn
        while (current_thread != tid)
        {
            pthread_cond_wait(&conditions[tid], &control_mutex);
        }

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

        // Update the current thread to the next one
        current_thread = dest;

        // Signal the next thread
        pthread_cond_signal(&conditions[dest]);
        pthread_mutex_unlock(&control_mutex);
    }

    return NULL;
}

int main()
{
    pthread_t threads[NUM_THREADS];

    // Initialize mutex and condition variables
    pthread_mutex_init(&control_mutex, NULL);
    for (int i = 0; i < NUM_THREADS; i++)
    {
        pthread_cond_init(&conditions[i], NULL);
    }

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

    // Destroy mutex and condition variables
    pthread_mutex_destroy(&control_mutex);
    for (int i = 0; i < NUM_THREADS; i++)
    {
        pthread_cond_destroy(&conditions[i]);
    }

    return 0;
}
