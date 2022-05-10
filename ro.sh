# for i in range(1, 20)
for i in {1..20}; do
    echo threads: $i
    export OMP_NUM_THREADS=$i
    time ./openmp < in.txt > oout.txt
done

for i in {1..20}; do
    echo processes: $i
    export OMP_NUM_THREADS=$i
    time mpirun -np $i --map-by none --bind-to none ./mpi in.txt > mout
done



