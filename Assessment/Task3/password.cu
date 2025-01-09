#include <stdio.h>
#include <stdlib.h>
#include <cuda_runtime.h>

// Encryption logic
__device__ char* CudaCrypt(char* rawPassword) {
    char* newPassword = (char*)malloc(sizeof(char) * 11);

    newPassword[0] = rawPassword[0] + 2;
    newPassword[1] = rawPassword[0] - 2;
    newPassword[2] = rawPassword[0] + 1;
    newPassword[3] = rawPassword[1] + 3;
    newPassword[4] = rawPassword[1] - 3;
    newPassword[5] = rawPassword[1] - 1;
    newPassword[6] = rawPassword[2] + 2;
    newPassword[7] = rawPassword[2] - 2;
    newPassword[8] = rawPassword[3] + 4;
    newPassword[9] = rawPassword[3] - 4;
    newPassword[10] = '\0';

    for (int i = 0; i < 10; i++) {
        if (i >= 0 && i < 6) { // Checking all lower case letter limits
            if (newPassword[i] > 122) {
                newPassword[i] = (newPassword[i] - 122) + 97;
            } else if (newPassword[i] < 97) {
                newPassword[i] = (97 - newPassword[i]) + 97;
            }
        } else { // Checking number section
            if (newPassword[i] > 57) {
                newPassword[i] = (newPassword[i] - 57) + 48;
            } else if (newPassword[i] < 48) {
                newPassword[i] = (48 - newPassword[i]) + 48;
            }
        }
    }
    return newPassword;
}

// Kernel function for encryption
__global__ void encryptPassword(char* inputPassword, char* encryptedPassword) {
    char* result = CudaCrypt(inputPassword);
    for (int i = 0; i < 11; i++) {
        encryptedPassword[i] = result[i];
    }
}

// Main function
int main() {
    char inputPassword[5];
    printf("Enter a 4-character password to encrypt: ");
    scanf("%4s", inputPassword);

    char* gpuInputPassword;
    char* gpuEncryptedPassword;
    char encryptedPassword[11];

    // Allocate memory on the GPU
    cudaMalloc((void**)&gpuInputPassword, sizeof(char) * 5);
    cudaMalloc((void**)&gpuEncryptedPassword, sizeof(char) * 11);

    // Copy input password to GPU
    cudaMemcpy(gpuInputPassword, inputPassword, sizeof(char) * 5, cudaMemcpyHostToDevice);

    // Launch kernel
    encryptPassword<<<1, 1>>>(gpuInputPassword, gpuEncryptedPassword);

    // Copy encrypted password back to CPU
    cudaMemcpy(encryptedPassword, gpuEncryptedPassword, sizeof(char) * 11, cudaMemcpyDeviceToHost);

    // Print the encrypted password
    printf("Encrypted password: %s\n", encryptedPassword);

    // Free GPU memory
    cudaFree(gpuInputPassword);
    cudaFree(gpuEncryptedPassword);

    return 0;
}
