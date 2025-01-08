#include <stdio.h>
#include <omp.h>

int main()
{
    // omp_set_num_threads(5);
    omp_set_nested(1);
#pragma omp parallel num_threads(5)
    {
#pragma omp parallel num_threads(3)
        {
            int id = omp_get_thread_num();
            printf("Hello World from %d: \n",id);
        }
    }

    return 0;
}