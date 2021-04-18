#!/bin/bash
for filename in inputs/*.txt; do
    for ((i=0;i<31;i++)); do
        echo $filename
        mpirun -np 4 --mca plm_base_verbose 10 stuff_MPI_stuff.out $filename
    done
done

