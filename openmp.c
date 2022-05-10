#include <stdio.h>
#include <omp.h>
#include <stdlib.h>

int main()
{

    int n;
    double **matrix;

    // Read the matrix size
    printf("Enter the size of the matrix: ");
    scanf("%d", &n);

    // allocate memory for gaussion matrix
    matrix = (double **)malloc(n * sizeof(double *));
    for (int i = 0; i < n; i++)
    {
        matrix[i] = (double *)malloc((n + 1) * sizeof(double));
    }

    // Read the matrix
    printf("Enter the matrix: \n");
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n + 1; j++)
        {
            scanf("%lf", &matrix[i][j]);
        }
    }

    // Gaussion elimination

    // Forward process
    // omp_set_num_threads(8);
#pragma omp parallel
    {
        int id = omp_get_thread_num();
        int nthreads = omp_get_num_threads();
        double *tmp = (double *)malloc((n + 1) * sizeof(double));

        for (int k = 0; k < n; k++)
        {
            //copy matrix[k] to tmp
            #pragma omp barrier
            for(int i = k; i < n+1; i++)
            {
                tmp[i] = matrix[k][i];
            }

            for (int i = k + 1 + id; i < n; i += nthreads)
            {
                matrix[i][k] = matrix[i][k] / tmp[k];

                for (int j = k + 1; j < n + 1; j++)
                {
                    matrix[i][j] -= matrix[i][k] * tmp[j];
                }
            }
        }

        // Backward process

        // free memory
        free(tmp);
    }
    for (int i = n - 1; i >= 0; i--)
    {
        for (int j = i + 1; j < n; j++)
            matrix[i][n] -= matrix[i][j] * matrix[j][n];
        matrix[i][n] /= matrix[i][i];
    }

    // Dumps the result
    for (int i = 0; i < n; i++)
    {
        printf("%.2f\n", matrix[i][n]);
    }

    // Free the memory
    for (int i = 0; i < n; i++)
    {
        free(matrix[i]);
    }
    free(matrix);

    return 0;
}