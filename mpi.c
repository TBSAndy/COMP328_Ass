// programme principal
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "mpi.h"

#define CHECK(x) \
  do { \
    if (!(x)) { \
      fprintf(stderr, "%s:%d: ", __func__, __LINE__); \
	  if(errno==0) errno=ECANCELED; \
      perror(#x); \
      exit(EXIT_FAILURE); \
    } \
  } while (0)

#define VERBOSE 0

int size;
int rank;


int main(int argc, char **argv)
{

    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s <input filename>\n", argv[0]);
        exit(1);
    }
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (VERBOSE)
        printf("%d %d\n", rank, size);

    int n;
    double **matrix;

    if (rank == 0)
    {
        // input n and allocate memory

        FILE *f;

        f = fopen(argv[1], "r");
        CHECK(f != NULL);

        CHECK(fscanf(f, "%d", &n) == 1);

        matrix = (double **)malloc(n * sizeof(double *));
        for (int i = 0; i < n; i++)
        {
            CHECK((matrix[i] = (double *)malloc((n + 1) * sizeof(double))));
        }

        // Read the matrix
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n + 1; j++)
            {
                CHECK(fscanf(f, "%lf", &matrix[i][j]) == 1);
            }
        }
    }

    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
    if (rank != 0)
    {
        matrix = (double **)malloc(n * sizeof(double *));
        for (int i = 0; i < n; i++)
        {
            matrix[i] = (double *)malloc((n + 1) * sizeof(double));
        }
    }

    for (int i = 0; i < n; i++)
    {
        MPI_Bcast(matrix[i], n + 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    }


    // ============ Algorithm part =======

    // Forward process
    for (int k = 0; k < n; k++)
    {
        // broadcast the row k to other process
        MPI_Bcast(matrix[k], n + 1, MPI_DOUBLE, k % size, MPI_COMM_WORLD);
        for (int i = ((k + 1) / size + ( (k+1) % size > rank )) * size + rank; i < n; i+= size)
        {
            matrix[i][k] = matrix[i][k] / matrix[k][k];
            for (int j = k + 1; j < n + 1; j++)
            {
                matrix[i][j] -= matrix[i][k] * matrix[k][j];
            }
        }
    }


    if(rank == 0) {
        // Backward process
        for (int i = n - 1; i >= 0; i--)
        {
            for (int j = i + 1; j < n; j++)
                matrix[i][n] -= matrix[i][j] * matrix[j][n];
            matrix[i][n] /= matrix[i][i];
        }

        // Dumps the result
        for(int i = 0; i < n; i++) {
            printf("%.2f\n", matrix[i][n]);
        }
    }

    // Free allocated memory

    for (int i = 0; i < n; i++)
    {
        free(matrix[i]);
    }
    free(matrix);

    MPI_Finalize();
    return 0;
}