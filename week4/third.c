#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <math.h>

// Structure to hold the range and result count for each thread
typedef struct {
    int prime_count;
    int start;
    int end;
} ThreadData;

// Thread function to find primes and return the count
void *find_primes(void *arg) {
    ThreadData *data = (ThreadData *)arg;
    int start = data->start;
    int end = data->end;
    data->prime_count = 0; // Initialize prime count to 0

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
            data->prime_count++; // Increment prime count for each prime found
        }
    }

    pthread_exit((void *)data); // Return the result
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

    // Create threads and assign ranges
    for (int i = 0; i < num_threads; i++) {
        thread_data[i] = malloc(sizeof(ThreadData)); // Allocate memory for each thread's data
        thread_data[i]->start = i * range_size + 1;
        thread_data[i]->end = (i == num_threads - 1) ? upper_limit : (i + 1) * range_size;

        pthread_create(&threads[i], NULL, find_primes, (void *)thread_data[i]);
    }

    // Wait for all threads to finish and collect the results
    for (int i = 0; i < num_threads; i++) {
        void *result;
        pthread_join(threads[i], &result);

        ThreadData *data = (ThreadData *)result; // Get the result from thread
        printf("Thread %d found %d prime numbers.\n", i + 1, data->prime_count);

        free(data); // Free the memory allocated for thread's data
    }

    // Free allocated memory for threads
    free(threads);

    return 0;
}
