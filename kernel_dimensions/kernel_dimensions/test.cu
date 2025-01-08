#include <stdio.h>
#include <stdlib.h>

__global__ void display(){
      int i = 
    threadIdx.x +
    (threadIdx.y * blockDim.x) +
    (threadIdx.z * blockDim.x * blockDim.y) + 
    (blockIdx.x * blockDim.x * blockDim.y * blockDim.z) +
    (blockIdx.y * blockDim.x * blockDim.y * blockDim.z * gridDim.x) +
    (blockIdx.z * blockDim.x * blockDim.y * blockDim.z * gridDim.x * gridDim.y);
    printf("tdx: %d\n", i);

}

int main(){
    dim3 gridDim(13,13, 2);
    dim3 blockDim(10,10, 2);
    display<<<gridDim, blockDim>>>();
    cudaDeviceSynchronize();
    return 0;
}