#!/bin/bash

#SBATCH --time=01:00:00
#SBATCH --ntasks=32
#SBATCH --mem-per-cpu=1GB

num_nodes=(1 2 4 8 16 32)
sizes=(100 200 300 400 500 600 700 800 900 1000)

for nodes in ${num_nodes[@]}
do 
  for size in ${sizes[@]}
  do 
    echo "Num of Nodes =" $nodes " Domain Size =" $size
    srun mill $size $size
  done
done

