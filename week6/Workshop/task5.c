#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>

#define NUM_THREADS 5
#define NUM_ITERATIONS 10 

char *messages[NUM_THREADS] = {NULL, NULL, NULL};

void messenger(long tid) {
    char tmpbuf[100];

    for (int i = 0; i < NUM_ITERATIONS; i++) {
   
        long int dest = (tid + 1) % NUM_THREADS;
        sprintf(tmpbuf, "Hello from Thread %ld!", tid);
        char *msg = strdup(tmpbuf);

        #pragma omp critical
        {
            messages[dest] = msg;
            printf("Thread %ld sent the message to Thread %ld\n", tid, dest);
        }

        
        #pragma omp barrier

        
        #pragma omp critical
        {
            if (messages[tid] != NULL) {
                printf("Thread %ld received the message '%s'\n", tid, messages[tid]);
                free(messages[tid]);
                messages[tid] = NULL;
            }
        }

        
        #pragma omp barrier
    }
}

int main() {
    omp_set_num_threads(NUM_THREADS);

    #pragma omp parallel
    {
        long tid = omp_get_thread_num();
        messenger(tid);
    }

    return 0;
}