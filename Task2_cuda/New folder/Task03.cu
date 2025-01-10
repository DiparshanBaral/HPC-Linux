#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <cuda.h>
#include <cuda_runtime.h>
#include <cuda_runtime_api.h>


// Function to compare two strings on the device
__device__ bool compareStrings(char *stringA, char *stringB)
{
    int index = 0;
    while (stringA[index] == stringB[index])
    {
        if (stringA[index] == '\0' || stringB[index] == '\0')
            break;
        index++;
    }
    return (stringA[index] == '\0' && stringB[index] == '\0');
}

// Device function to encrypt the password
__device__ void CudaCrypt(char *rawPassword, char *newPassword)
{
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

    for (int i = 0; i < 10; i++)
    {
        if (i < 6)
        {
            if (newPassword[i] > 122)
                newPassword[i] = (newPassword[i] - 122) + 97;
            else if (newPassword[i] < 97)
                newPassword[i] = (97 - newPassword[i]) + 97;
        }
        else
        {
            if (newPassword[i] > 57)
                newPassword[i] = (newPassword[i] - 57) + 48;
            else if (newPassword[i] < 48)
                newPassword[i] = (48 - newPassword[i]) + 48;
        }
    }
}

// Kernel function to decrypt the password
__global__ void decrypt(char *alphabet, char *numbers, char *encPass, char *outputDevice)
{
    char genRawPass[4];
    genRawPass[0] = alphabet[blockIdx.x];
    genRawPass[1] = alphabet[blockIdx.y];
    genRawPass[2] = numbers[threadIdx.x];
    genRawPass[3] = numbers[threadIdx.y];

    char enc[11]; // Space to hold encrypted password
    CudaCrypt(genRawPass, enc);

    if (compareStrings(enc, encPass))
    {
        printf("Password found: %s\n", genRawPass);
        for (int i = 0; i < 4; i++)
            outputDevice[i] = genRawPass[i];
    }
}

int main(int argc, char **argv)
{
    char *outputHost = (char *)malloc(sizeof(char) * 4);
    char *outputDevice;

    cudaMalloc((void **)&outputDevice, sizeof(char) * 4);

    // Input encrypted password
    char inputHost[11];
    printf("Enter the encrypted password: ");
    scanf("%s", inputHost);
    inputHost[10] = '\0';

    char *inputDevice;
    cudaMalloc((void **)&inputDevice, sizeof(char) * 11);
    cudaMemcpy(inputDevice, inputHost, sizeof(char) * 11, cudaMemcpyHostToDevice);

    // Alphabet and number arrays
    char cpuAlphabet[26] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};
    char cpuNumbers[10] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};

    char *gpuAlphabet, *gpuNumbers;
    cudaMalloc((void **)&gpuAlphabet, sizeof(char) * 26);
    cudaMemcpy(gpuAlphabet, cpuAlphabet, sizeof(char) * 26, cudaMemcpyHostToDevice);

    cudaMalloc((void **)&gpuNumbers, sizeof(char) * 10);
    cudaMemcpy(gpuNumbers, cpuNumbers, sizeof(char) * 10, cudaMemcpyHostToDevice);

    // Launch kernel with multiple blocks and threads
    dim3 grid(26, 26, 1);
    dim3 block(10, 10, 1);
    decrypt<<<grid, block>>>(gpuAlphabet, gpuNumbers, inputDevice, outputDevice);
    cudaDeviceSynchronize();

    // Copy decrypted password back to host
    cudaMemcpy(outputHost, outputDevice, sizeof(char) * 4, cudaMemcpyDeviceToHost);

    printf("The final password is: %s\n", outputHost);

    // Free memory
    cudaFree(outputDevice);
    cudaFree(inputDevice);
    cudaFree(gpuAlphabet);
    cudaFree(gpuNumbers);
    free(outputHost);

    return 0;
}