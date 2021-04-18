#!/bin/bash
for filename in inputs/*.txt; do
    echo $filename
    for ((i=0;i<33;i++)); do
        { time mpirun -np 2 perfectSquareMPI.out $filename 2>trash.txt ; } 2>> MPITests/$(basename "$filename" )
    done

    for ((i=0;i<33;i++)); do
        { time mpirun -np 2 perfectSquareHibrid.out $filename 2>trash.txt ; } 2>> HibridTests/$(basename "$filename" )
    done
done