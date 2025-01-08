#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <cuda_runtime.h>

__global__ void quad(int *A, int *B, int *C, double *X1, double *X2, long lines) {
    long tid = blockIdx.x * blockDim.x + threadIdx.x;
    if (tid < lines) { 
        double a = A[tid];
        double b = B[tid];
        double c = C[tid];
        double discriminant = b * b - 4 * a * c;

        if (discriminant >= 0) {
            X1[tid] = (-b + sqrt(discriminant)) / (2 * a);
            X2[tid] = (-b - sqrt(discriminant)) / (2 * a);
        } else { 
            X1[tid] = X2[tid] = NAN; 
        }
    }
}

int main() {
    FILE *fp = NULL;
    int a, b, c;
    int *A, *B, *C;
    int *d_A, *d_B, *d_C;
    double *X1, *X2;
    double *d_X1, *d_X2;
    long lines = 0;
    long line = 0;

    // Open the file to count lines
    fp = fopen("QuadData.txt", "r");
    if (fp == NULL) {
        printf("Error opening QuadData.txt\n");
        return -1;
    }

    // Count the number of lines in the file
    while (fscanf(fp, "%d,%d,%d\n", &a, &b, &c) != EOF) {
        lines++;
    }
    fclose(fp);

    // Allocate memory on the host
    A = (int *)malloc(lines * sizeof(int));
    B = (int *)malloc(lines * sizeof(int));
    C = (int *)malloc(lines * sizeof(int));
    X1 = (double *)malloc(lines * sizeof(double));
    X2 = (double *)malloc(lines * sizeof(double));

    // Allocate memory on the device
    cudaMalloc(&d_A, lines * sizeof(int));
    cudaMalloc(&d_B, lines * sizeof(int));
    cudaMalloc(&d_C, lines * sizeof(int));
    cudaMalloc(&d_X1, lines * sizeof(double));
    cudaMalloc(&d_X2, lines * sizeof(double));

    // Read the file again to fill A, B, and C
    fp = fopen("QuadData.txt", "r");
    if (fp == NULL) {
        printf("Error opening QuadData.txt\n");
        return -1;
    }

    while (fscanf(fp, "%d,%d,%d\n", &a, &b, &c) != EOF) {
        A[line] = a;
        B[line] = b;
        C[line] = c;
        line++;
    }
    fclose(fp);

    // Copy data from host to device
    cudaMemcpy(d_A, A, lines * sizeof(int), cudaMemcpyHostToDevice);
    cudaMemcpy(d_B, B, lines * sizeof(int), cudaMemcpyHostToDevice);
    cudaMemcpy(d_C, C, lines * sizeof(int), cudaMemcpyHostToDevice);

    // Launch kernel
    int threadsPerBlock = 256;
    int blocksPerGrid = (lines + threadsPerBlock - 1) / threadsPerBlock;
    quad<<<blocksPerGrid, threadsPerBlock>>>(d_A, d_B, d_C, d_X1, d_X2, lines);

    // Copy results back to host
    cudaMemcpy(X1, d_X1, lines * sizeof(double), cudaMemcpyDeviceToHost);
    cudaMemcpy(X2, d_X2, lines * sizeof(double), cudaMemcpyDeviceToHost);

    // Print results
    for (long i = 0; i < lines; i++) {
        if (!isnan(X1[i]) && !isnan(X2[i])) {
            printf("Row %ld: X1 = %lf, X2 = %lf\n", i, X1[i], X2[i]);
        } else {
            printf("Row %ld: Roots are complex or undefined\n", i);
        }
    }

    // Free memory
    free(A);
    free(B);
    free(C);
    free(X1);
    free(X2);
    cudaFree(d_A);
    cudaFree(d_B);
    cudaFree(d_C);
    cudaFree(d_X1);
    cudaFree(d_X2);

    return 0;
}