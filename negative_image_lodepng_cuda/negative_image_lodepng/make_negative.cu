#include <stdio.h>
#include <stdlib.h>
#include "lodepng.h"
#include <cuda_runtime.h>

__global__ void manipulate_pixels(unsigned int h, unsigned int w, unsigned char *pixels) {
	int rows = blockIdx.y * blockDim.y + threadIdx.y;
	int cols = blockIdx.x * blockDim.x + threadIdx.x;
        int index = (rows * w + cols) * 4;
//	printf("index = %d, pixel[%d]= %d\n", index, index, pixels[index]);
	if(rows<h && cols<w){
            		pixels[index] = 255 - pixels[index];		//Red
			pixels[index + 1] = 255 - pixels[index+1];	//Green
            		pixels[index + 2] = 255 - pixels[index+2];	//Blue
            		pixels[index + 3] = 255;          // Alpha (fully opaque)
        }
}

int main() {
	unsigned char *h_pixels;
	unsigned int h, w;

	lodepng_decode32_file(&h_pixels, &w, &h,"eagle.png");

	unsigned char *d_pixels;
	int image_size = h*w*4;
	cudaMalloc(&d_pixels,image_size);
	cudaMemcpy(d_pixels, h_pixels, image_size, cudaMemcpyHostToDevice);

	dim3 gridSize(w,h,1);
	dim3 blockSize(4,1,1);

	manipulate_pixels<<<gridSize,blockSize>>>(h,w,d_pixels);
	cudaDeviceSynchronize();

	cudaMemcpy(h_pixels, d_pixels, image_size, cudaMemcpyDeviceToHost);

	lodepng_encode32_file("negative.png",h_pixels,w,h);

    	free(h_pixels);
	cudaFree(d_pixels);
    	return 0;
}
