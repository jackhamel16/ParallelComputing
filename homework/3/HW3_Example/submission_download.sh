#!/bin/bash
#SBATCH -N 1
#SBATCH --ntasks-per-node 1
#SBATCH --mem 10gb
#SBATCH --time 04:00:00

./getGiabData.sh

scontrol show job $SLURM_JOB_ID
