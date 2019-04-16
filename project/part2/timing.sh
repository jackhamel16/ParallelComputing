#!/bin/bash

#SBATCH -N 32
#SBATCH 1:00:00

threads=(1 2 4 8 16 32)
sizes=(1000 10000 100000 1000000 10000000)

for n_threads in "${threads[@]}"
do
  ./threads 100000000 $n_threads
done
