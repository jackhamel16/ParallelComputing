#!/bin/bash
#SBATCH -N 1
#SBATCH --ntasks-per-node 1
#SBATCH --mem 10gb
#SBATCH --time 01:00:00

./part1 2000

scontrol show job $SLURM_JOB_ID
