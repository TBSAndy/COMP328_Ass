#include <stdio.h>
#include <omp.h>
#include <stdlib.h>

int main() {
    int n;
    double **matrix;

    // Read the matrix size
    printf("Enter the size of the matrix: ");
    scanf("%d", &n);

    // allocate memory for gaussion matrix
    matrix = (double **) malloc(n * sizeof(double *));
    for (int i = 0; i < n; i++) {
        matrix[i] = (double *) malloc((n+1) * sizeof(double));
    }

    // Read the matrix
    printf("Enter the matrix: \n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n + 1; j++) {
            scanf("%lf", &matrix[i][j]);
        }
    }

    // Gaussion elimination

    // Forward process
    for(int k = 0; k < n; k++) {
        for(int i = k+ 1; i < n; i++) {
            matrix[i][k] = matrix[i][k] / matrix[k][k];
            for(int j = k + 1; j < n + 1; j ++) {
                matrix[i][j] -= matrix[i][k] * matrix[k][j];
            }
        }
    }

    // Backward process
    for(int i = n - 1; i >= 0; i--) {
        for(int j = i+1; j < n; j++)
            matrix[i][n] -= matrix[i][j] * matrix[j][n];
        matrix[i][n] /= matrix[i][i];

    }

    // Dumps the result
    for(int i = 0; i < n; i++) {
        printf("%.2f\n", matrix[i][n]);
    }

    return 0;
}