#include <stdio.h>
#include <stdlib.h>
#include <cuda_runtime.h>

// Device-side encryption logic
__device__ void CudaEncrypt(const char* rawPassword, char* encryptedPassword) {
    encryptedPassword[0] = rawPassword[0] + 2;
    encryptedPassword[1] = rawPassword[0] - 2;
    encryptedPassword[2] = rawPassword[0] + 1;
    encryptedPassword[3] = rawPassword[1] + 3;
    encryptedPassword[4] = rawPassword[1] - 3;
    encryptedPassword[5] = rawPassword[1] - 1;
    encryptedPassword[6] = rawPassword[2] + 2;
    encryptedPassword[7] = rawPassword[2] - 2;
    encryptedPassword[8] = rawPassword[3] + 4;
    encryptedPassword[9] = rawPassword[3] - 4;
    encryptedPassword[10] = '\0';
}

// Device-side decryption logic
__device__ void CudaDecrypt(const char* encryptedPassword, char* decryptedPassword) {
    decryptedPassword[0] = encryptedPassword[0] - 2; // Reverse +2
    decryptedPassword[1] = encryptedPassword[3] - 3; // Reverse +3
    decryptedPassword[2] = encryptedPassword[6] - 2; // Reverse +2
    decryptedPassword[3] = encryptedPassword[8] - 4; // Reverse +4
    decryptedPassword[4] = '\0'; // Null-terminate the string
}

// Kernel function for encryption
__global__ void encryptKernel(const char* rawPassword, char* encryptedPassword) {
    CudaEncrypt(rawPassword, encryptedPassword);
    printf("Raw Password: %s -> Encrypted Password: %s\n", rawPassword, encryptedPassword);
}

// Kernel function for decryption
__global__ void decryptKernel(const char* inputEncryptedPassword, char* decryptedPassword) {
    CudaDecrypt(inputEncryptedPassword, decryptedPassword);
}

// Main function
int main() {
    // Alphabet and number set for encryption
    char cpuAlphabet[26] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};
    char cpuNumbers[10] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};

    // Allocate and copy alphabet and numbers to the GPU
    char* gpuAlphabet;
    char* gpuNumbers;
    cudaMalloc((void**)&gpuAlphabet, sizeof(cpuAlphabet));
    cudaMalloc((void**)&gpuNumbers, sizeof(cpuNumbers));
    cudaMemcpy(gpuAlphabet, cpuAlphabet, sizeof(cpuAlphabet), cudaMemcpyHostToDevice);
    cudaMemcpy(gpuNumbers, cpuNumbers, sizeof(cpuNumbers), cudaMemcpyHostToDevice);

    // Prompt the user to enter a raw password
    printf("Enter a Raw password (e.g., Hp45) to search for its encryption code: ");
    char rawPassword[5];
    scanf("%4s", rawPassword);

    // Allocate and copy the raw password to the GPU
    char* gpuRawPassword;
    char* gpuEncryptedPassword;
    cudaMalloc((void**)&gpuRawPassword, sizeof(rawPassword));
    cudaMalloc((void**)&gpuEncryptedPassword, sizeof(char) * 11);
    cudaMemcpy(gpuRawPassword, rawPassword, sizeof(rawPassword), cudaMemcpyHostToDevice);

    // Dynamically set the number of blocks and threads
    int threadsPerBlock = 256;
    int blocksPerGrid = (26 * 26 * 10 + threadsPerBlock - 1) / threadsPerBlock;

    // Launch the encryption kernel
    printf("\nEncrypting the provided password:\n");
    encryptKernel<<<1, 1>>>(gpuRawPassword, gpuEncryptedPassword);
    cudaDeviceSynchronize();

    // Retrieve and display the encrypted password
    char encryptedPassword[11];
    cudaMemcpy(encryptedPassword, gpuEncryptedPassword, sizeof(encryptedPassword), cudaMemcpyDeviceToHost);
    printf("Encrypted Password: %s\n", encryptedPassword);

    // Allocate memory for decryption
    char* gpuInputEncryptedPassword;
    char* gpuDecryptedPassword;
    cudaMalloc((void**)&gpuInputEncryptedPassword, sizeof(encryptedPassword));
    cudaMalloc((void**)&gpuDecryptedPassword, sizeof(char) * 5);

    // Copy the encrypted password to the device for decryption
    cudaMemcpy(gpuInputEncryptedPassword, encryptedPassword, sizeof(encryptedPassword), cudaMemcpyHostToDevice);

    // Launch the decryption kernel
    printf("\nDecrypting the encrypted password:\n");
    decryptKernel<<<1, 1>>>(gpuInputEncryptedPassword, gpuDecryptedPassword);

    // Retrieve the decrypted password
    char decryptedPassword[5];
    cudaMemcpy(decryptedPassword, gpuDecryptedPassword, sizeof(decryptedPassword), cudaMemcpyDeviceToHost);

    printf("Decrypted Password: %s\n", decryptedPassword);

    // Free GPU memory
    cudaFree(gpuAlphabet);
    cudaFree(gpuNumbers);
    cudaFree(gpuRawPassword);
    cudaFree(gpuEncryptedPassword);
    cudaFree(gpuInputEncryptedPassword);
    cudaFree(gpuDecryptedPassword);

    return 0;
}
