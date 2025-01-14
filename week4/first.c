#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <math.h>

// Thread function for calculating primes in a given range
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
    pthread_t threads[3];
    int ranges[3][2] = {{1, 3500}, {3501, 7000}, {7001, 10000}};

    // Create threads
    for (int i = 0; i < 3; i++) {
        int *range = malloc(2 * sizeof(int)); // Allocate memory for range
        range[0] = ranges[i][0];
        range[1] = ranges[i][1];
        pthread_create(&threads[i], NULL, find_primes, range);
    }

    // Join threads
    for (int i = 0; i < 3; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}
