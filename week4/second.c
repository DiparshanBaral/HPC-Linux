#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <math.h>

void *find_primes(void *arg) {
    int *range = (int *)arg;
    int start = range[0];
    int end = range[1];
    free(range); // Free allocated memory for range

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
            printf("%d\n", i);
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

    // Create threads and assign ranges
    for (int i = 0; i < num_threads; i++) {
        int *range = malloc(2 * sizeof(int));
        range[0] = i * range_size + 1;
        range[1] = (i == num_threads - 1) ? upper_limit : (i + 1) * range_size;

        pthread_create(&threads[i], NULL, find_primes, range);
    }

    // Join threads
    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    // Free allocated memory for threads
    free(threads);

    return 0;
}
