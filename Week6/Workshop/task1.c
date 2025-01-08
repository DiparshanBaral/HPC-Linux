#include <stdio.h>
#include <omp.h>
void main()
{
  #pragma omp parallel num_threads(10)
  printf("Hello world from OpenMP!\n"); 
}
