#include <stdio.h>
#include <omp.h>
#include <stdlib.h>

int max = 1000;
int sum = 0;

int main()
{
    // int no_threads = omp_get_max_threads();
    int array[max];
#pragma omp parallel for
    for (int i = 0; i < max; i++)
    {
        array[i] = i+1;
    }
#pragma omp parallel for reduction(+ : sum)
    for (int i = 0; i < max; i++)
    {
        sum += array[i];
    }

    for (int i = 0; i < max; i++)
    {
        printf("Array : %d  = \t %d\n",i,array[i]);
    }
    printf("Sum is : %d\n", sum);
    return 0;
}