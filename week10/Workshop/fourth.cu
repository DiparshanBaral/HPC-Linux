#include <stdio.h>
#include <cuda_runtime.h>

__global__ void displayIdx()
{
    int tid = threadIdx.x + threadIdx.y * blockDim.x + threadIdx.z * blockDim.x * blockDim.y;
    printf("%d\n", tid);
}

int main()
{
    int n;
    printf("Enter n : ");
    scanf("%d", &n);
    dim3 gridSize(1, 1, 1);
    dim3 blockSize(n, n, n);
    displayIdx<<<gridSize, blockSize>>>();
    cudaDeviceSynchronize();
    return 0;
}
