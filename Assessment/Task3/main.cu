#include <stdio.h>
#include <stdlib.h>
#include <cuda_runtime.h>

// Decryption logic
__device__ char* CudaDecrypt(char* encryptedPassword) {
    char* decryptedPassword = (char*)malloc(sizeof(char) * 5);

    decryptedPassword[0] = encryptedPassword[0] - 2;
    decryptedPassword[1] = encryptedPassword[3] - 3;
    decryptedPassword[2] = encryptedPassword[6] - 2;
    decryptedPassword[3] = encryptedPassword[8] - 4;
    decryptedPassword[4] = '\0';

    return decryptedPassword;
}

// Kernel function for decryption
__global__ void decryptPassword(char* inputEncryptedPassword, char* decryptedPassword) {
    char* result = CudaDecrypt(inputEncryptedPassword);
    for (int i = 0; i < 5; i++) {
        decryptedPassword[i] = result[i];
    }
}

// Main function
int main() {
    char inputEncryptedPassword[11];
    printf("Enter the 10-character encrypted password to decrypt: ");
    scanf("%10s", inputEncryptedPassword);

    char* gpuInputEncryptedPassword;
    char* gpuDecryptedPassword;
    char decryptedPassword[5];

   
    cudaMalloc((void**)&gpuInputEncryptedPassword, sizeof(char) * 11);
    cudaMalloc((void**)&gpuDecryptedPassword, sizeof(char) * 5);

    
    cudaMemcpy(gpuInputEncryptedPassword, inputEncryptedPassword, sizeof(char) * 11, cudaMemcpyHostToDevice);

    
    decryptPassword<<<1, 1>>>(gpuInputEncryptedPassword, gpuDecryptedPassword);

    cudaMemcpy(decryptedPassword, gpuDecryptedPassword, sizeof(char) * 5, cudaMemcpyDeviceToHost);

    
    printf("Decrypted password: %s\n", decryptedPassword);

   
    cudaFree(gpuInputEncryptedPassword);
    cudaFree(gpuDecryptedPassword);

    return 0;
}
