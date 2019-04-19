#!/bin/bash

#SBATCH -n 1
#SBATCH --cpus-per-task=32
#SBATCH --time 0:30:00
#SBATCH --mem= 1GB

threads=(1 2 4 8 16 32)
sizes=(100 1000 10000 100000 1000000 10000000 100000000)

echo "Threads Timing Study"

for n_threads in "${threads[@]}"
do
  for run in {1..3}
  do 
    ./threads 10000000 $n_threads
  done
done

for size in "${sizes[@]}"
do
  for run in {1..3}
  do 
    ./threads $size 16
  done
done


scontrol show job $SLURM_JOB_ID
