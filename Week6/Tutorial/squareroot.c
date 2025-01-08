#include <stdio.h>
#include <math.h>
#include <omp.h>

int count = 1;
int main()
{
    int total = 1000;
    double start, end, time_taken;
    start = omp_get_wtime();
#pragma omp parallel
    {

#pragma omp for
        for (int i = 1; i < total; i++)
        {
            printf("sqrt(%d) = %f\n", i, sqrt(i));
            #pragma omp critical
            count++;
        }
    }
    end = omp_get_wtime();
    time_taken = end - start;
    printf("Time Taken to execute %lf \t count = %d\n", time_taken, count);
    return 0;
}