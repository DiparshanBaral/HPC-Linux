#include <stdio.h>
#include <cuda_runtime.h>

__global__ void displayIdx(){
    int idx = blockIdx.y * blockDim.y + threadIdx.y;
    printf("idx: %d\n", idx);
}

int main(){
    dim3 gridsize(1,3,1);
    dim3 blocksize(1,2,1);
    displayIdx<<<gridsize, blocksize>>>();
    cudaDeviceSynchronize();
    return 0;
}
// nvcc -o first first.cu