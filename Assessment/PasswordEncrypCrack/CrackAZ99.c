#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <crypt.h>
#include <unistd.h>
#include <omp.h>

/******************************************************************************
  Demonstrates how to crack an encrypted password using a simple
  "brute force" algorithm. Works on passwords that consist only of 2 uppercase
  letters and a 2-digit integer.

  Compile with:
    cc -o CrackAZ99 CrackAZ99.c -lcrypt -fopenmp

  If you want to analyze the output, use the redirection operator to send
  output to a file:
    ./CrackAZ99 > output.txt
******************************************************************************/

int count = 0;          // Tracks the number of combinations explored
int password_found = 0; // Flag to indicate the password is found

void substr(char *dest, char *src, int start, int length)
{
    memcpy(dest, src + start, length);
    *(dest + length) = '\0';
}

void crack(char *salt_and_encrypted)
{
    int x, y, z;             // Loop counters
    char salt[7];            // Salt used for hashing
    char plain[7];           // The combination of letters and numbers being checked
    struct crypt_data cdata; // Thread-safe structure for crypt_r
    cdata.initialized = 0;

    substr(salt, salt_and_encrypted, 0, 6);

    printf("Starting brute force cracking...\n");
    printf("-----------------------------------------------------\n");

// Parallelize the outermost loop
#pragma omp parallel for private(y, z, plain, cdata) shared(password_found, count)
    for (x = 'A'; x <= 'Z'; x++)
    {
        if (password_found)
            continue; // Exit early if another thread found the password

        for (y = 'A'; y <= 'Z'; y++)
        {
            for (z = 0; z <= 99; z++)
            {
                if (password_found)
                    break;

                sprintf(plain, "%c%c%02d", x, y, z); // Generate a candidate password
                char *enc = crypt_r(plain, salt, &cdata);

#pragma omp atomic
                count++;

                if (strcmp(salt_and_encrypted, enc) == 0)
                {
#pragma omp critical
                    {
                        if (!password_found)
                        {
                            password_found = 1;
                            printf("Password Found! \n");
                            printf("-----------------------------------------------------\n");
                            printf("Encrypted Password : %s\n", salt_and_encrypted);
                            printf("Decrypted Password : %s\n", plain);
                            printf("Hash Match         : %s\n", enc);
                            printf("-----------------------------------------------------\n");
                        }
                    }
                }
            }
        }
    }
}

int main()
{
    char encrypted_password[100];

    printf("-----------------------------------------------------\n");
    printf("           SHA-512 Password Cracker Tool            \n");
    printf("-----------------------------------------------------\n\n");

    printf("Enter the encrypted password (SHA-512 hash with salt): ");
    scanf("%99s", encrypted_password);

    printf("\nProcessing your input...\n\n");

    double start_time = omp_get_wtime();

    crack(encrypted_password);

    double end_time = omp_get_wtime();
    double cpu_time_used = end_time - start_time;

    if (!password_found)
    {
        printf("-----------------------------------------------------\n");
        printf("PASSWORD COULD NOT BE FOUND!!!\n");
        printf("-----------------------------------------------------\n");
    }

    printf("Total Combinations Explored : %d\n", count);
    printf("Time Taken                  : %.2f seconds\n", cpu_time_used);
    printf("-----------------------------------------------------\n");

    return 0;
}
