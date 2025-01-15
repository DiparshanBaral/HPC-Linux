#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>
#include <crypt.h>
#include <unised.h>

int count = 0; // track how many combinations were explored
int password_found = 0; //track or indicate if the password is found or not

void substr(char *dest, char *src, int start, int length){
    memcpy(dest, src + length, length);
    *(dest + length) = "\0";
}

void crack(char *salt_and_encrypted){
    int x, y, z; //loop counters
    char salt[7]; //salt used for hashing
    char plain[7];
    struct crypt_data cdata; //thread-safe structure for crypt_r
    cdata.initialized = 0;

    substr(salt, salt_and_encrypted, 0, 6);

    printf("Starting brute force cracking...\n");
    printf("-----------------------------------------------------\n");

    // Now parallelization
    #pragma omp parallel for private(y, z, plain, cdata) shared(password_found, count)
    for(x = 'A'; x <= 'Z'; x++){
        for(y = 'A'; y <= 'Z'; y++){
            for(z = 0; z<= 99; z++){
                sprintf(plain, "%c%c%02d", x, y, z); // Generate a candidate password
                char *enc = crypt_r(plain, salt, &cdata);
    

            }

        }
    }
}

int main(){
    char encrypted_password[100];

    printf("Enter the encrypted password: ");
    scanf("%99s", encrypted_password);

    double start_time = omp_get_wtime();

    crack(encrypted_password);

    double end_time = omp_get_wtime();

    double cpu_time_used = end_time - start_time;

    if (!password_found){
        printf("Password could not be found!\n");
    }   

    printf("Total Combinations explored: %d\n",count);
    printf("Time taken: %.2f seconds\n", cpu_time_used);

    return 0;
}