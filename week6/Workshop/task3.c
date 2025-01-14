#include <stdio.h>
#include <omp.h>

void main() {
    int i, c;
    printf("Prime numbers between 1 and 1000 are :\n");

    #pragma omp parallel for num_threads(5)
    for (i = 1; i <= 1000; i++) {
        int is_prime = 1;
        if (i <= 1) {
            is_prime = 0;
        } else {
            for (c = 2; c <= i - 1; c++) {
                if (i % c == 0) {
                    is_prime = 0;
                    break;
                }
            }
        }
        if (is_prime) {
            printf("Thread %d: %d\n", omp_get_thread_num(), i);
        }
    }
}
