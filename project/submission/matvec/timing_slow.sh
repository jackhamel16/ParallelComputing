#!/bin/bash

#SBATCH -N 1
#SBATCH --time 0:30:00
#SBATCH --mem= 90GB

sizes=(100 1000 10000 100000)

echo "Slow Timing Study"

for size in "${sizes[@]}"
do
  echo "Size = " $size
  for run in {1..3}
  do 
    ./slow $size
  done
done

scontrol show job $SLURM_JOB_ID
