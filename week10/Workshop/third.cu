#include <stdio.h>
#include <cuda_runtime.h>

__global__ void display() {
    int threadIDX = threadIdx.x;
    int threadIDY = threadIdx.y;
    int threadIDZ = threadIdx.z;
    int blockID = blockIdx.x;
    printf("Block %d -> ThreadId = %d,ThreadID = %d, ThreadId = %d\n ", blockID, threadIDX, threadIDY, threadIDZ);
}

int main() {
    dim3 gridSize(1, 1, 1);   
    dim3 blockSize(5, 2, 3);  
    display<<<gridSize, blockSize>>>();
    cudaDeviceSynchronize();
    return 0;
}
