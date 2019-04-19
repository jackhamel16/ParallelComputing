#!/bin/bash

#SBATCH -n 1
#SBATCH --time 0:10:00
#SBATCH --mem= 1GB

sizes=(100 1000 10000 100000 1000000 10000000 100000000)

echo "Serial Timing Study"

for size in "${sizes[@]}"
do
  for run in {1..3}
  do 
    ./serial $size
  done
done

scontrol show job $SLURM_JOB_ID
