
# Generate Data

`python generate.py > $FILENAME`
 and input data size


# Compile code

gcc -o baseline baseline.c
gcc -o openmp openmp.c
mpicc -o mpi mpi.c

# Run code
`./baseline < $FILENAME`
`./openmp < $FILENAME`
`mpirun -np $i --map-by none --bind-to none ./mpi $FILENAME`

