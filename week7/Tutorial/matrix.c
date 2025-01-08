#include <stdio.h>
#include <omp.h>

void fill_matrix(int rows, int cols, int matrix[rows][cols])
{
#pragma omp paralel for collapse(2)
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            matrix[i][j] = i + j;
        }
    }
}

// void display_matrix(int rows, int cols, int matrix[rows][cols])
// {
//     printf("%d x %d matrix: \n", rows, cols);
//     for (int i = 0; i < rows; i++)
//     {
//         for (int j = 0; j < cols; j++)
//         {
//             printf("%d\t", matrix[i][j]);
//         }
//         printf("\n");
//     }
// }

int main()
{
    int m = 500;
    int n = 500;
    int A[m][n], B[n][m], C[m][m];

    fill_matrix(m, n, A);
    //  display_matrix(m,n,A);
    fill_matrix(n, m, B);
    //  display_matrix(n,m,B);

    double start = omp_get_wtime();
#pragma omp parallel for collapse(2)
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < m; j++)
        {
            C[i][j] = 0;
        }
    }

#pragma omp parallel for collapse(3)
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < m; j++)
        {
            for (int k = 0; k < n; k++)
            {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }

    double end = omp_get_wtime();

    printf("Execution Time = %lf\n", end - start);

    //  display_matrix(m,m,C);

    return 0;
}