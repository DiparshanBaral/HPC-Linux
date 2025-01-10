#include <stdio.h>
#include <stdlib.h>
#include "lodepng.h"
#include <cuda_runtime.h>
#include <string.h>

// Error checking macro for CUDA functions
#define CUDA_CHECK(call) do { \
    cudaError_t err = call; \
    if (err != cudaSuccess) { \
        fprintf(stderr, "CUDA Error: %s\n", cudaGetErrorString(err)); \
        exit(EXIT_FAILURE); \
    } \
} while (0)

// Function to check if a file exists
int file_exists(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (file) {
        fclose(file);
        return 1;  // File exists
    }
    return 0;  // File does not exist
}

// Function to check if the file has a .png extension
int has_png_extension(const char *filename) {
    size_t len = strlen(filename);
    if (len > 4 && strcmp(&filename[len - 4], ".png") == 0) {
        return 1;  // Valid .png file
    }
    return 0;  // Invalid extension
}

__global__ void apply_box_blur(unsigned int h, unsigned int w, unsigned char *input_pixels, unsigned char *output_pixels) {
    int row = blockIdx.y * blockDim.y + threadIdx.y;
    int col = blockIdx.x * blockDim.x + threadIdx.x;
    int index = (row * w + col) * 4;

    if (row < h && col < w) {
        int r_sum = 0, g_sum = 0, b_sum = 0, count = 0;

        // Iterate over the 3x3 box surrounding the current pixel
        for (int i = -1; i <= 1; i++) {
            for (int j = -1; j <= 1; j++) {
                int neighbor_row = row + i;
                int neighbor_col = col + j;

                // Ensure neighbors are within image bounds
                if (neighbor_row >= 0 && neighbor_row < h && neighbor_col >= 0 && neighbor_col < w) {
                    int neighbor_index = (neighbor_row * w + neighbor_col) * 4;
                    r_sum += input_pixels[neighbor_index];
                    g_sum += input_pixels[neighbor_index + 1];
                    b_sum += input_pixels[neighbor_index + 2];
                    count++;
                }
            }
        }

        // Set the new pixel value based on the average of surrounding pixels
        output_pixels[index] = r_sum / count;         // Red
        output_pixels[index + 1] = g_sum / count;    // Green
        output_pixels[index + 2] = b_sum / count;    // Blue
        output_pixels[index + 3] = input_pixels[index + 3];  // Alpha (unchanged)
    }
}

int main() {
    unsigned char *h_pixels, *h_output_pixels;
    unsigned int h, w;
    char input_filename[256], output_filename[256];

    // Ask the user for the input image file name
    do {
        printf("Enter the name of the image to be blurred (e.g., hck.png): ");
        scanf("%255s", input_filename);

        // Check if the file exists and if it's a valid PNG file
        if (!file_exists(input_filename)) {
            printf("Error: The input image file '%s' was not found. Please try again.\n", input_filename);
        } else if (!has_png_extension(input_filename)) {
            printf("Error: The input image must have a .png extension. Please try again.\n");
        }
    } while (!file_exists(input_filename) || !has_png_extension(input_filename));  // Repeat until the file exists and is a .png

    // Decode the input image
    if (lodepng_decode32_file(&h_pixels, &w, &h, input_filename) != 0) {
        fprintf(stderr, "Error: Failed to decode image %s\n", input_filename);
        return EXIT_FAILURE;
    }

    // Ask the user for the output image file name
    do {
        printf("Enter the name for the output image (e.g., blurred_image.png): ");
        scanf("%255s", output_filename);

        // Check if the output file has a valid .png extension
        if (!has_png_extension(output_filename)) {
            printf("Error: The output image must have a .png extension. Please try again.\n");
        }
    } while (!has_png_extension(output_filename));  // Repeat until a valid .png output file name is provided

    int image_size = h * w * 4;
    h_output_pixels = (unsigned char *)malloc(image_size);
    if (!h_output_pixels) {
        fprintf(stderr, "Error: Failed to allocate memory for output pixels\n");
        free(h_pixels);
        return EXIT_FAILURE;
    }

    // Allocate GPU memory
    unsigned char *d_input_pixels, *d_output_pixels;
    CUDA_CHECK(cudaMalloc(&d_input_pixels, image_size));
    CUDA_CHECK(cudaMalloc(&d_output_pixels, image_size));

    // Copy input image to device
    CUDA_CHECK(cudaMemcpy(d_input_pixels, h_pixels, image_size, cudaMemcpyHostToDevice));

    // Configure block and grid size
    dim3 blockSize(16, 16, 1);  // 16x16 threads per block
    dim3 gridSize((w + blockSize.x - 1) / blockSize.x, (h + blockSize.y - 1) / blockSize.y, 1);

    // Launch kernel to apply box blur
    apply_box_blur<<<gridSize, blockSize>>>(h, w, d_input_pixels, d_output_pixels);
    CUDA_CHECK(cudaDeviceSynchronize());  // Ensure all threads complete

    // Check for any errors after kernel execution
    CUDA_CHECK(cudaGetLastError());

    // Copy the blurred image back to the host
    CUDA_CHECK(cudaMemcpy(h_output_pixels, d_output_pixels, image_size, cudaMemcpyDeviceToHost));

    // Encode the output image and save it to file
    if (lodepng_encode32_file(output_filename, h_output_pixels, w, h) != 0) {
        fprintf(stderr, "Error: Failed to encode image %s\n", output_filename);
        free(h_pixels);
        free(h_output_pixels);
        CUDA_CHECK(cudaFree(d_input_pixels));
        CUDA_CHECK(cudaFree(d_output_pixels));
        return EXIT_FAILURE;
    }

    printf("Blurred image saved as %s\n", output_filename);

    // Free memory
    free(h_pixels);
    free(h_output_pixels);
    CUDA_CHECK(cudaFree(d_input_pixels));
    CUDA_CHECK(cudaFree(d_output_pixels));

    return 0;
}
