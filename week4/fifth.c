#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <math.h>

#define MAX_PRIMES 5 // Stop once 5 primes are found

// Shared variable to track the number of primes found
int prime_count_found = 0;
pthread_mutex_t lock; // Mutex for thread synchronization

// Structure to hold the range for each thread
typedef struct {
    int start;
    int end;
} ThreadData;

// Thread function to find primes
void *find_primes(void *arg) {
    ThreadData *data = (ThreadData *)arg;
    int start = data->start;
    int end = data->end;
    free(data); // Free allocated memory for the range

    for (int i = start; i <= end; i++) {
        if (i < 2) continue; // Skip numbers less than 2
        int is_prime = 1;
        for (int j = 2; j <= sqrt(i); j++) { // Check divisors up to sqrt(i)
            if (i % j == 0) {
                is_prime = 0;
                break;
            }
        }

        if (is_prime) {
            // Lock the mutex before updating the shared prime_count_found variable
            pthread_mutex_lock(&lock);
            if (prime_count_found < MAX_PRIMES) {
                prime_count_found++;
                printf("Prime number %d found: %d\n", prime_count_found, i);
            }
            pthread_mutex_unlock(&lock);

            // If 5 primes are found, cancel all threads
            if (prime_count_found >= MAX_PRIMES) {
                printf("5th prime found. Cancelling all threads.\n");
                pthread_exit(NULL); // Exit thread immediately if 5 primes are found
            }
        }
    }
    return NULL;
}

int main() {
    int num_threads;

    // Ask user for the number of threads
    printf("Enter the number of threads: ");
    scanf("%d", &num_threads);

    // Ask user for the upper limit of the range
    int upper_limit = 10000; // You can change this if desired
    int range_size = upper_limit / num_threads;

    pthread_t *threads = malloc(num_threads * sizeof(pthread_t));
    ThreadData *thread_data[num_threads]; // Array to hold thread data

    // Initialize the mutex
    pthread_mutex_init(&lock, NULL);

    // Create threads and assign ranges
    for (int i = 0; i < num_threads; i++) {
        thread_data[i] = malloc(sizeof(ThreadData)); // Allocate memory for each thread's data
        thread_data[i]->start = i * range_size + 1;
        thread_data[i]->end = (i == num_threads - 1) ? upper_limit : (i + 1) * range_size;

        pthread_create(&threads[i], NULL, find_primes, (void *)thread_data[i]);
    }

    // Wait for all threads to finish
    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    // Free allocated memory for threads
    free(threads);

    // Destroy the mutex
    pthread_mutex_destroy(&lock);

    return 0;
}
